#include <fstream>
#include <vector>

#define IN "bendin.txt"
#define OUT "bendout.txt"

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

inline i64 get_rect_area(i64 x1, i64 y1, i64 x2, i64 y2)
{
    return (x2 - x1) * (y2 - y1);
}

int main()
{
    ifstream in(IN);
    ofstream out(OUT);

    i64 axl, ayl, axr, ayr, bxl, byl, bxr, byr;
    in >> axl >> ayl >> axr >> ayr >> bxl >> byl >> bxr >> byr;

    i64 ox = min(axr, bxr) - max(axl, bxl);
    i64 oy = min(ayr, byr) - max(ayl, byl);
    i64 overlap = max(ox, 0ll) * max(oy, 0ll);

    out << get_rect_area(axl, ayl, axr, ayr) + get_rect_area(bxl, byl, bxr, byr) - overlap;

    return 0;
}
