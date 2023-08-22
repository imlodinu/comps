#include <fstream>
#include <vector>
#include <map>

#define IN "dictin.txt"
#define OUT "dictout.txt"

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

    i64 dn, tn;
    in >> dn >> tn;
    map<i64, i64> d;

    for (i64 i = 0; i < dn; ++i)
    {
        i64 x, y;
        in >> x >> y;
        d.emplace(x, y);
    }

    for (i64 i = 0; i < tn; ++i)
    {
        i64 x;
        in >> x;
        if (d.find(x) == d.end())
            out << "C?" << '\n';
        else
            out << d[x] << '\n';
    }

        return 0;
}
