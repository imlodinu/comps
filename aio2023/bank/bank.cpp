#include <fstream>
#include <vector>

#define IN "bankin.txt"
#define OUT "bankout.txt"

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

int main()
{
    ifstream in(IN);
    ofstream out(OUT);

    i64 days;
    in >> days;

    i64 skill = 1;
    i64 money = 0;
    vec<bool> schedule(days);

    in.get();
    for (i64 index = 0; index < days; index++)
    {
        char day_type = in.get();
        schedule[index] = day_type == 'C';
    }

    for (i64 index = 0; index < days; index++)
    {
        const i64 days_left = days - index;
        i64 value_no_class = skill * days_left;
        i64 value_class = (skill + 1) * (days_left - 1);

        if (schedule[index] && value_class > value_no_class)
        {
            skill++;
        }
        else
        {
            money += skill;
        }
    }

    out << money;

    return 0;
}
