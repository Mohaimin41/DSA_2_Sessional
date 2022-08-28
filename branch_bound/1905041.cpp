#include <iostream>
#include <algorithm>
#include <tuple>
#include <cmath>
#include <queue>

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
    class matrix
    {
    private:
        /* data */
        ll **mat;
        ll sz;
        ll r;
        ll c;
        ll b;
        ll level;
        ll order;

    public:
        matrix(ll, ll);
        ~matrix();
        void input();
        void print();
        ll **getMat() { return mat; }
        ll getSz() const { return sz; }
        ll getR() const { return r; }
        ll getC() const { return c; }
        ll getB() const { return b; }
        ll getLevel() const { return level; }
        ll getOrder() const { return order; }
        void setB(ll bnd) { b = bnd; }
        void setLevel(ll lvl) { level = lvl; }
        void setOrder(ll ord) { order = ord; }
        void setR(ll fr) { r = fr; }
        void setC(ll fc) { c = fc; }
        void setMatrix(ll **);
        void setMatrix(ll **, ll, ll, bool);
        friend bool operator<(const dsa2::matrix &lhs, const dsa2::matrix &rhs);
    };

    matrix::matrix(ll sz, ll ord) : sz(sz), r(0), c(0), b(0), level(0), order(ord)
    {
        mat = new ll *[sz];
        for (ll i = 0; i < sz; i++)
            mat[i] = new ll[sz];
    }

    matrix::~matrix()
    {
        for (ll i = 0; i < sz; i++)
            delete[] mat[i];
        delete[] mat;
    }

    void matrix::input()
    {
        for (ll i = 0; i < sz; i++)
            for (ll j = 0; j < sz; j++)
            {
                char c;
                std::cin >> c;
                mat[i][j] = c == 'X' ? 1 : 0;
            }
    }

    void matrix::print()
    {
        std::cout << b << '\n';
        for (ll i = 0; i < sz; i++)
        {
            for (ll j = 0; j < sz; j++)
                std::cout << mat[i][j];

            std::cout << "\n";
        }
    }

    void matrix::setMatrix(ll **mat)
    {
        for (ll i = 0; i < sz; i++)
            for (ll j = 0; j < sz; j++)
                this->mat[i][j] = mat[i][j];
    }

    void matrix::setMatrix(ll **mat, ll dest, ll src, bool isRow = true)
    {
        for (ll i = 0; i < sz; i++)
            if (isRow)
            {
                this->mat[dest][i] = mat[src][i];
            }
            else
            {
                this->mat[i][dest] = mat[i][src];
            }
    }

    bool operator<(const dsa2::matrix &lhs, const dsa2::matrix &rhs)
    {
        // lhs.setB(dsa2::bound(lhs.getMat(), lhs.getR(), lhs.getC()));
        if (lhs.getB() > rhs.getB())
            return true;
        else if (lhs.getB() == rhs.getB())
        {
            if (lhs.getLevel() > rhs.getLevel())
                return true;
            else if (lhs.getLevel() == rhs.getLevel())
            {
                if (lhs.getOrder() > rhs.getOrder())
                    return true;
            }
        }
        return false;
    }

    ll n;
    ll bound(ll **mat, ll r, ll c);
    dsa2::matrix run(dsa2::matrix pmat);

} // namespace dsa2

ll dsa2::bound(ll **mat, ll r, ll c)
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

dsa2::matrix dsa2::run(dsa2::matrix pmat)
{
    // special priority queue of matrices
    std::priority_queue<dsa2::matrix> pq;
    pq.push(pmat);

    ll creation_order = 1;
    while (true)
    {
        dsa2::matrix temp = pq.top();
        pq.pop();
        ll sz = temp.getSz(), fixedRow = temp.getR(), fixedCol = temp.getC();
        ll **parentMat = temp.getMat();

        if ((fixedCol == fixedRow) && (fixedCol = sz - 1))
            return temp;

        // branch, calculate bound, set ++order and level = p.level+1, push to pq
        // row:
        if (fixedCol == fixedRow)
        {
            for (ll remainingRow = fixedRow; remainingRow < sz; remainingRow++)
            {
                // create one branch and do the works
                dsa2::matrix brMat(sz, ++creation_order);
                brMat.setLevel(temp.getLevel() + 1);
                brMat.setMatrix(parentMat);
                // swap the column under change;
                brMat.setMatrix(parentMat, fixedCol, remainingRow);
                // put other columns in order
                for (ll j = fixedCol + 1; j < sz; j++)
                {
                    if (j == remainingRow)
                        continue;
                    brMat.setMatrix(parentMat, j, j, false);
                }
                brMat.setR(++fixedRow);
                brMat.setB(dsa2::bound(brMat.getMat(), brMat.getR(), brMat.getC()));
                pq.push(brMat);
            }
        }
        else // col:
        {
            for (ll remainingCol = fixedCol; remainingCol < sz; remainingCol++)
            {
                dsa2::matrix brMat(sz, ++creation_order);
                brMat.setLevel(temp.getLevel() + 1);
                brMat.setMatrix(parentMat);
                // swap the column under change;
                brMat.setMatrix(parentMat, fixedCol, remainingCol, false);
                // put other columns in order
                for (ll j = fixedCol + 1; j < sz; j++)
                {
                    if (j == remainingCol)
                        continue;
                    brMat.setMatrix(parentMat, j, j, false);
                }
                brMat.setC(++fixedCol);
                brMat.setB(dsa2::bound(brMat.getMat(), brMat.getR(), brMat.getC()));
                pq.push(brMat);
            }
        }
    }
}

int main()
{
    std::cin >> dsa2::n;

    dsa2::matrix mat(dsa2::n, 1);
    mat.input();
    // dsa2::run(mat);
    ll fr, fc;
    std::cin >> fr >> fc;
    std::cout << dsa2::bound(mat.getMat(), fr, fc) << '\n';
    mat.print();
    return 0;
}