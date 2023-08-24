#include <fstream>
#include <vector>
#include <set>

#define IN "rafflein.txt"
#define OUT "raffleout.txt"

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

    i64 N, K;
    in >> N >> K;

    set<i64> valid;
    set<i64> invalid;

    for (i64 index = 0; index < N; index++)
    {
        i64 number = 0;
        in >> number;

        if (valid.find(number) == valid.end() && invalid.find(number) == invalid.end())
        {
            valid.emplace(number);
        }
        else
        {
            valid.erase(number);
            invalid.emplace(number);
        }
    }

    i64 min = 100001;
    for (i64 number : valid)
    {
        min = number < min ? number : min;
    }

    out << (min == 100001 ? -1 : min);

    return 0;
}
