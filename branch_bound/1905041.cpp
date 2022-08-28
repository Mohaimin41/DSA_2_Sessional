#include <iostream>
#include <algorithm>
#include <tuple>
#include <cmath>

//----------------------DEBUGGER---------------------//

void __print(int x) { std::cerr << x; }
void __print(long x) { std::cerr << x; }
void __print(long long x) { std::cerr << x; }
void __print(unsigned x) { std::cerr << x; }
void __print(unsigned long x) { std::cerr << x; }
void __print(unsigned long long x) { std::cerr << x; }
void __print(float x) { std::cerr << x; }
void __print(double x) { std::cerr << x; }
void __print(long double x) { std::cerr << x; }
void __print(char x) { std::cerr << '\'' << x << '\''; }
void __print(const char *x) { std::cerr << '\"' << x << '\"'; }
void __print(const std::string &x) { std::cerr << '\"' << x << '\"'; }
void __print(bool x) { std::cerr << (x ? "true" : "false"); }

template <typename T, typename V>
void __print(const std::pair<T, V> &x)
{
    std::cerr << '{';
    __print(x.first);
    std::cerr << ',';
    __print(x.second);
    std::cerr << '}';
}
template <typename T, typename V, typename Z>
void __print(const std::tuple<T, V, Z> &x)
{
    std::cerr << '{';
    __print(std::get<0>(x));
    std::cerr << ',';
    __print(std::get<1>(x));
    std::cerr << ',';
    __print(std::get<2>(x));
    std::cerr << '}';
}
template <typename T>
void __print(const T &x)
{
    int f = 0;
    std::cerr << '{';
    for (auto &i : x)
        std::cerr << (f++ ? "," : ""), __print(i);
    std::cerr << "}";
}
void _print() { std::cerr << "]\n"; }
template <typename T, typename... V>
void _print(T t, V... v)
{
    __print(t);
    if (sizeof...(v))
        std::cerr << ", ";
    _print(v...);
}

#ifdef LOCAL
#define debug(x...)                    \
    std::cerr << "[" << #x << "] = ["; \
    _print(x)
#else
#define debug(x...)
#endif

//----------------------------Debugger End--------------------------//

#define ll long long
#define pb push_back
#define mp make_pair

namespace dsa2
{
    ll r, c, n;
    ll bound(ll **mat);
    int run();

} // namespace dsa2

ll dsa2::bound(ll **mat)
{
    // unfixed region bound is max of floor((count_of_1+1)/2)
    // rows:
    ll m1 = 0;
    for (ll i = r; i < n; i++)
    {
        ll r1 = 0;
        // traverse the row:
        for (ll j = 0; j < n; j++)
        {
            if (mat[i][j])
                r1++;
        }
        r1 = std::floor((r1 + 1) / 2);
        m1 = std::max(m1, r1);
    }
    // columns:
    for (ll i = c; i < n; i++)
    {
        ll c1 = 0;
        // traverse the column:
        for (ll j = 0; j < n; j++)
        {
            if (mat[j][i])
                c1++;
        }
        c1 = std::floor((c1 + 1) / 2);
        m1 = std::max(m1, c1);
    }
    // bound for fixed region is the highest bandwidth of all fixed rows and columns
    // No change for fixed region, unfixed region 1 values are moved as close as
    // possible to fixed region
    ll m2 = 0;
    // rows:
    for (ll rows = 0; rows < r; rows++)
    {
        // fixed left region of diagonal entry (mat[rows][0] to mat[rows][rows])
        ll rfl = rows;
        // store the first position of 1 value at the end of loop
        for (ll i = rows; i >= 0; i--)
        {
            if (mat[rows][i])
                rfl = i;
        }
        // bandwidth for fixed region = number of traversed cell(s)
        rfl = rows - rfl + 1;

        // fixed right region of diagonal entry (mat[rows][rows] to mat[rows][c-1])
        ll rfr = rows;
        for (ll i = rows; i < c; i++)
        {
            if (mat[rows][i])
                rfr = i;
        }
        rfr = rfr - rows + 1;

        // unfixed right region (mat[rows][c] to mat[rows][n-1])
        ll rur = 0;
        for (ll i = c; i < n; i++)
        {
            if (mat[rows][i])
                rur++;
        }

        // bandwidth for fixed region: max(number of traversed cell to last 1)
        if (rur)
            m2 = std::max(m2, c - rows + rur);
        m2 = std::max(rfl, std::max(m2, rfr));
    }

    // columns:
    for (ll cols = 0; cols < c; cols++)
    {
        // fixed upper region of diagonal entry (from mat[cols][cols] to mat[0][cols])
        ll rfu = cols;
        // store the first position of 1 value at the end of loop
        for (ll i = cols; i >= 0; i--)
        {
            if (mat[i][cols])
                rfu = i;
        }
        // bandwidth for fixed region = number of traversed cell(s)
        rfu = cols - rfu + 1;

        // fixed lower region of diagonal entry (mat[cols][cols] to mat[r-1][cols])
        ll rfd = cols;
        for (ll i = cols; i < r; i++)
        {
            if (mat[i][cols])
                rfd = i;
        }
        rfd = rfd - cols + 1;

        // unfixed lower region (mat[r][cols] to mat[n-1][cols])
        ll rul = 0;
        for (ll i = r; i < n; i++)
        {
            if (mat[i][cols])
                rul++;
        }

        // bandwidth for fixed region: max(number of traversed cell to last 1)
        if (rul)
            m2 = std::max(m2, r - cols + rul);
        m2 = std::max(rfu, std::max(m2, rfd));
    }

    return std::max(m1, m2);
}

int dsa2::run()
{
    return 0;
}

int main()
{
    std::cin >> dsa2::n;

    ll **mat = new ll *[dsa2::n];
    for (ll i = 0; i < dsa2::n; i++)
        mat[i] = new ll[dsa2::n];

    for (ll i = 0; i < dsa2::n; i++)
        for (ll j = 0; j < dsa2::n; j++)
        {
            char c;
            std::cin >> c;
            mat[i][j] = c == 'X' ? 1 : 0;
        }

    std::cout << dsa2::bound(mat) << '\n';
    ll state = dsa2::run();
    if (state)
    {
        dsa2::r = 0, dsa2::c = 0;
        ll b = dsa2::bound(mat);
        std::cout << b << '\n';
        for (ll i = 0; i < dsa2::n; i++)
        {
            for (ll j = 0; j < dsa2::n; j++)
                std::cout << mat[i][j];

            std::cout << "\n";
        }
    }

    for (ll i = 0; i < dsa2::n; i++)
        delete[] mat[i];
    delete[] mat;
    return 0;
}