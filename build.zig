const std = @import("std");

var stored_b: *std.Build = undefined;
var allocator: std.mem.Allocator = undefined;

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

    const generate_step = b.step("generate", "generates a project");
    generate_step.makeFn = generateProject;
    b.default_step = generate_step;
}

const template_path = "./template/";

const default_args = [_][]const u8{};
pub fn generateProject(self: *std.build.Step, progress: *std.Progress.Node) !void {
    _ = progress;
    _ = self;
    var args = stored_b.args orelse &default_args;
    if (args.len < 1) {
        return;
    }
    const project_name = args[0 .. args.len - 1];
    _ = project_name;
    const last_name = args[args.len - 1];
    const path = try std.fs.path.join(allocator, args);
    // std.debug.print("{s}\n", .{path});
    try copyRecursiveDir(
        last_name,
        try std.fs.cwd().openIterableDir(template_path, std.fs.Dir.OpenDirOptions{}),
        try std.fs.cwd().makeOpenPath(path, std.fs.Dir.OpenDirOptions{}),
    );
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
