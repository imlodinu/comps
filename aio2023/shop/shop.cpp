#include <fstream>
#include <vector>
#include <algorithm>

#define IN "shopin.txt"
#define OUT "shopout.txt"

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

i64 wrap(i64 i, i64 i_max)
{
    return ((i % i_max) + i_max) % i_max;
}

int main()
{
    ifstream in(IN);
    ofstream out(OUT);

    i64 N, M;
    in >> N >> M;

    vector<i64> houses(N);
    vector<i64> positions(M);
    vector<i64> shared(M);
    vector<i64> price_factors(M);

    for (i64 index = 0; index < N; index++)
    {
        in >> houses[index];
    }

    for (i64 index = 0; index < M; index++)
    {
        in >> positions[index];
        shared[index] = index;
    }

    for (i64 index = 0; index < M; index++)
    {
        in >> price_factors[index];
    }

    std::sort(shared.begin(), shared.end(), [&](i64 a, i64 b)
              { return price_factors[a] < price_factors[b]; });

    i64 house_index = 0;
    for (i64 house : houses)
    {
        i64 min_badness = -1;

        // i64 supermarket_close_index = -1;

        // for (i64 needle = 0; needle < M; needle++)
        // {
        //     if (positions[needle] >= house)
        //     {
        //         if (needle == 0)
        //             supermarket_close_index = needle;
        //         else
        //             supermarket_close_index = needle - 1;
        //         break;
        //     }
        // }

        for (i64 supermarket = 0; supermarket < M; supermarket++)
        {
            i64 badness = price_factors[shared[supermarket]] + abs(house - positions[shared[supermarket]]);

            if (min_badness == -1 || badness < min_badness)
            {
                min_badness = badness;
            }
        }

        out << min_badness << ((house_index != N - 1) ? " " : "");
        house_index++;
    }

    return 0;
}
