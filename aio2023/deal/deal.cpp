#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <map>

#define IN "dealin.txt"
#define OUT "dealout.txt"

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

    i64 N, M;
    in >> N >> M;

    vec<pair<i64, i64>> votes(M, pair<i64, i64>(-1, 0));
    vec<pair<i64, i64>> deals(M);

    for (i64 i = 0; i < M; ++i)
    {
        i64 candidate;
        in >> candidate;
        votes[i] = votes[i].first == -1 ? pair<i64, i64>(candidate, 1) : pair<i64, i64>(candidate, votes[i].second + 1);
    }

    for (i64 i = 0; i < M; ++i)
    {
        i64 price;
        in >> price;
        if (votes[i].second != 1)
        {
            deals.push_back({i, price});
        }
    }

    i64 bought = 0;
    do
    {
        sort(votes.begin(), votes.end(), [](const pair<i64, i64> &a, const pair<i64, i64> &b)
             { return a.second > b.second; });
        pair<i64, i64> votes_for_first = *find_if(votes.begin(), votes.end(), [](const pair<i64, i64> &a)
                                                  { return a.first == 1; });
        pair<i64, i64> top_votes = votes[0];
        if (votes[0].first == 1)
        {
            break;
        }

        i64 value_of_buying_from_current_highest =

    } while (true);

    out << bought;

    return 0;
}
