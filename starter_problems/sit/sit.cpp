#include <fstream>
#include <vector>

#define IN "sitin.txt"
#define OUT "sitout.txt"

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

    u64 r, w, p;
    in >> r >> w >> p;
    u64 sitting = std::min(r * w, p);
    u64 standing = std::max(p - sitting, 0ull);
    out << sitting << " " << standing;

    return 0;
}
