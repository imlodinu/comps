#include <fstream>
#include <vector>

#define IN "countin.txt"
#define OUT "countout.txt"

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

    i64 n;
    in >> n;

    for (i64 i = 1; i <= n; ++i)
    {
        out << i << '\n';
    }

    return 0;
}
