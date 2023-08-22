const std = @import("std");

var stored_b: *std.Build = undefined;
var allocator: std.mem.Allocator = undefined;
var project_res: *ProjectSet = undefined;

pub fn build(b: *std.Build) !void {
    stored_b = b;
    var arena = std.heap.ArenaAllocator.init(b.allocator);
    defer arena.deinit();
    allocator = b.allocator; //arena.allocator();

    // Standard target options allows the person running `zig build` to choose
    // what target to build for. Here we do not override the defaults, which
    // means any target is allowed, and the default is native. Other options
    // for restricting supported target set are available.
    // const target = b.standardTargetOptions(.{});

    // Standard optimization options allow the person running `zig build` to select
    // between Debug, ReleaseSafe, ReleaseFast, and ReleaseSmall. Here we do not
    // set a preferred release mode, allowing the user to decide how to optimize.
    // const optimize = b.standardOptimizeOption(.{});

    var made_project_res = try getSetFromArgs();
    project_res = try allocator.create(ProjectSet);
    project_res.* = made_project_res;

    const generate_step = b.step("generate", "generates a project");
    generate_step.makeFn = generateProject;

    const build_step = b.step("build", "builds a project");

    var exe = buildProject();
    build_step.dependOn(&stored_b.addInstallArtifact(exe, .{
        .dest_dir = .{
            .override = .{
                .custom = project_res.exe_file_path,
            },
        },
    }).step);

    const run_step = b.step("run", "runs a project");
    run_step.makeFn = runProject;
    run_step.dependOn(build_step);
    b.default_step = run_step;
}

const template_path = "./template/";

const default_args = [_][]const u8{};
pub fn generateProject(self: *std.build.Step, progress: *std.Progress.Node) !void {
    _ = progress;
    _ = self;
    try copyRecursiveDir(
        project_res.project_name,
        try std.fs.cwd().openIterableDir(template_path, std.fs.Dir.OpenDirOptions{}),
        try std.fs.cwd().makeOpenPath(project_res.path, std.fs.Dir.OpenDirOptions{}),
    );
}

pub fn buildProject() *std.Build.Step.Compile {
    const target = stored_b.standardTargetOptions(.{});
    const mode = stored_b.standardOptimizeOption(.{});

    var exe = stored_b.addExecutable(.{
        .name = project_res.project_name,
        // .root_source_file = .{ .path = "src/main.zig" },
        .optimize = mode,
        .target = target,
    });
    exe.addCSourceFile(std.Build.Step.Compile.CSourceFile{
        .file = .{ .path = project_res.cpp_file_path },
        .flags = &[_][]const u8{ "-O2", "-lm", "-std=c++11" },
    });
    exe.linkLibCpp();
    // exe.out_filename = project_info.exe_file_path;
    return exe;
}

pub fn runProject(self: *std.build.Step, progress: *std.Progress.Node) !void {
    _ = progress;
    _ = self;
    _ = try std.ChildProcess.exec(.{
        .allocator = allocator,
        .cwd = project_res.path,
        .argv = &[_][]const u8{project_res.exe_with_extension},
    });
}

const ProjectSet = struct {
    project_path: []const []const u8,
    project_name: []const u8,
    path: []const u8,
    cpp_file_path: []const u8,
    exe_file_path: []const u8,
    exe_with_extension: []const u8,
};
fn getSetFromArgs() !ProjectSet {
    var args = stored_b.args orelse &default_args;
    if (args.len < 1) {
        return error.NotEnoughArgs;
    }
    const project_path = args[0 .. args.len - 1];
    const project_name = args[args.len - 1];
    const path = try std.fs.path.join(allocator, args);
    const cpp_name = try std.mem.join(allocator, ".", &[_][]const u8{ project_name, "cpp" });
    const cpp_file_path = try std.fs.path.join(allocator, &[_][]const u8{ path, cpp_name });
    var exe_file_path = try std.fs.path.join(allocator, &[_][]const u8{ "../", path });
    exe_file_path = try std.mem.join(allocator, "", &[_][]const u8{ exe_file_path, "\\" });
    var exe_with_extension = try std.fs.path.join(allocator, &[_][]const u8{
        path,
        project_name,
    });
    exe_with_extension = try std.mem.join(allocator, ".", &[_][]const u8{ project_name, "exe" });
    return ProjectSet{
        .project_path = project_path,
        .project_name = project_name,
        .path = path,
        .cpp_file_path = cpp_file_path,
        .exe_file_path = exe_file_path,
        .exe_with_extension = exe_with_extension,
    };
}

fn copyRecursiveDir(project: []const u8, src_dir: std.fs.IterableDir, dest_dir: std.fs.Dir) anyerror!void {
    var iter = src_dir.iterate();
    while (true) {
        const entry = try iter.next();
        if (entry == null) {
            break;
        } else {
            switch (entry.?.kind) {
                .file => {
                    const name = try std.mem.replaceOwned(u8, allocator, entry.?.name, "{project}", project);
                    // replace {project} with the project name
                    var file = try src_dir.dir.openFile(entry.?.name, .{ .mode = .read_only });
                    defer file.close();

                    var out = try dest_dir.createFile(name, .{});
                    defer out.close();

                    var buf_reader = std.io.bufferedReader(file.reader());
                    var in_stream = buf_reader.reader();

                    var buffer = [_]u8{0} ** 1024;
                    while (true) {
                        var buf = in_stream.readUntilDelimiter(&buffer, '\n') catch |err| {
                            switch (err) {
                                error.EndOfStream => break,
                                else => return err,
                            }
                        };
                        // replace {project} with the project name
                        var line = try std.mem.replaceOwned(u8, allocator, buf, "{project}", project);
                        try out.writeAll(line);
                        _ = try out.write("\n");
                    }
                },
                .directory => {
                    // Create destination directory
                    dest_dir.makeDir(entry.?.name) catch |e| {
                        switch (e) {
                            std.os.MakeDirError.PathAlreadyExists => {},
                            else => return e,
                        }
                    };
                    // Open destination directory
                    var dest_entry_dir = try dest_dir.openDir(entry.?.name, std.fs.Dir.OpenDirOptions{ .access_sub_paths = true, .no_follow = true });
                    defer dest_entry_dir.close();
                    // Open directory we're copying files from
                    var src_entry_dir = try src_dir.dir.openIterableDir(entry.?.name, std.fs.Dir.OpenDirOptions{ .access_sub_paths = true, .no_follow = true });
                    defer src_entry_dir.close();
                    // Begin the recursive descent!
                    try copyRecursiveDir(project, src_entry_dir, dest_entry_dir);
                },
                else => {}, // ¯\_(ツ)_/¯ Don't want any of that stuff, anyway
            }
        }
    }
}
