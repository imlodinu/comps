#include <fstream>
#include <vector>

#define IN "rainin.txt"
#define OUT "rainout.txt"

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

    i64 n, c, f = 0, i = 0;
    in >> n >> c;
    for (; i < n && f < c; ++i)
    {
        i64 x;
        in >> x;

        f += x;
    }
    out << i;

    return 0;
}
