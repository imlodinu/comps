#include <fstream>
#include <vector>

#define IN "dishin.txt"
#define OUT "dishout.txt"

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

template <typename T>
using vec = std::vector<T>;

using namespace std;

int main()
{
    ifstream in(IN);
    ofstream out(OUT);

    i64 n, c = 0;
    i64 l = 1000001, h = 0;
    in >> n;
    for (i64 i = 0; i < n; ++i)
    {
        i64 x;
        in >> x;

        c += x;
        l = min(l, x);
        h = max(h, x);
    }

    out << l << " " << h << " " << (c/n) << '\n';

    return 0;
}
