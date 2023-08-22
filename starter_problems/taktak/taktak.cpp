#include <fstream>
#include <vector>

#define IN "taktakin.txt"
#define OUT "taktakout.txt"

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

int main()
{
    std::ifstream in(IN);
    std::ofstream out(OUT);

    i64 f;
    in >> f;

    i64 m;
    for (m = 0; (f - 1) % 11 != 0; ++m)
    {
        f *= 2;
    }
    out << m << ' ' << f;
    
    return 0;
}
