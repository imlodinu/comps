#include <fstream>
#include <vector>
#include <iostream>
#include <map>

#define IN "telein.txt"
#define OUT "teleout.txt"

using u8 = unsigned char;
using u16 = unsigned short;
using u32 = unsigned int;
using u64 = unsigned long long;

using i8 = signed char;
using i16 = signed short;
using i32 = signed int;
using i64 = signed long long;

using f32 = float;
using f64 = double;

using namespace std;

template <typename T>
using vec = vector<T>;

enum instruction_t
{
    left,
    right,
    teleport,
};

int main()
{
    ifstream in(IN);
    ofstream out(OUT);

    i64 instructions;
    in >> instructions;

    map<i64, bool> visited;
    const i64 intitial_position = 0;
    visited.emplace(intitial_position, true);
    i64 position = intitial_position;
    i64 visited_count = 1;

    in.get();
    for (i64 index = 0; index < instructions; index++)
    {
        i64 instruction = in.get();
        instruction_t t;
        if (instruction == 'L')
        {
            t = instruction_t::left;
        }
        else if (instruction == 'R')
        {
            t = instruction_t::right;
        }
        else if (instruction == 'T')
        {
            t = instruction_t::teleport;
        }

        if (t == instruction_t::left)
        {
            position--;
        }
        else if (t == instruction_t::right)
        {
            position++;
        }
        else if (t == instruction_t::teleport)
        {
            position = intitial_position;
        }
        if (visited.find(position) == visited.end())
        {
            visited.emplace(position, true);
            visited_count++;
        }
    }

    out << visited_count;

    return 0;
}
