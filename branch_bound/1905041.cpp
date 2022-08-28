#include <iostream>
#include <algorithm>
#include <cmath>
#include <queue>

#define ll long long

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
        matrix(const dsa2::matrix &);
        ~matrix();
        void input();
        void print() const;
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
        dsa2::matrix &operator=(const dsa2::matrix &);
        friend bool operator<(const dsa2::matrix &lhs, const dsa2::matrix &rhs);
    };

    matrix::matrix(ll sz, ll ord) : sz(sz), r(0), c(0), b(0), level(0), order(ord)
    {
        mat = new ll *[sz];
        for (ll i = 0; i < sz; i++)
            mat[i] = new ll[sz];
    }

    matrix::matrix(const dsa2::matrix &a)
    {
        this->b = a.b;
        this->level = a.level;
        this->order = a.order;
        this->r = a.r;
        this->c = a.c;
        this->sz = a.sz;
        this->mat = new ll *[this->sz];
        for (ll i = 0; i < sz; i++)
            mat[i] = new ll[this->sz];
        for (ll i = 0; i < sz; i++)
            for (ll j = 0; j < sz; j++)
                this->mat[i][j] = a.mat[i][j];
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

    void matrix::print() const
    {
        std::cout << b << '\n';
        for (ll i = 0; i < sz; i++)
        {
            for (ll j = 0; j < sz; j++)
                // std::cout << (mat[i][j] == 1) ? 'X' : 'O';
                if (this->mat[i][j])
                    std::cout << 'X' << ' ';
                else
                    std::cout << 'O' << ' ';
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
        {
            if (isRow)
            {
                this->mat[dest][i] = mat[src][i];
            }
            else
            {
                this->mat[i][dest] = mat[i][src];
            }
        }
    }

    dsa2::matrix &matrix::operator=(const dsa2::matrix &a)
    {
        this->b = a.b;
        this->level = a.level;
        this->order = a.order;
        this->r = a.r;
        this->c = a.c;
        this->sz = a.sz;
        this->mat = new ll *[this->sz];
        for (ll i = 0; i < sz; i++)
            mat[i] = new ll[this->sz];
        for (ll i = 0; i < sz; i++)
            for (ll j = 0; j < sz; j++)
                this->mat[i][j] = a.mat[i][j];
        return *this;
    }

    bool operator<(const dsa2::matrix &lhs, const dsa2::matrix &rhs)
    {
        if (lhs.getB() == rhs.getB() && lhs.getLevel() == rhs.getLevel())
            return lhs.getOrder() > rhs.getOrder();
        else if (lhs.getB() == rhs.getB())
            return lhs.getLevel() < lhs.getLevel();
        else
            return lhs.getB() > rhs.getB();
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

    pmat.setB(dsa2::bound(pmat.getMat(), pmat.getR(), pmat.getC()));
    pmat.setLevel(1);

    pq.push(pmat);

    ll creation_order = 1;

    while (true)
    {
        dsa2::matrix temp = pq.top();
        pq.pop();
        ll sz = temp.getSz(), fixedRow = temp.getR(), fixedCol = temp.getC();

        if ((fixedCol == fixedRow) && (fixedCol == sz - 1))
            return temp;

        // branch, calculate bound, set ++order and level = p.level+1, push to pq
        if (fixedCol != fixedRow) // row
        {
            for (ll remainingRow = fixedRow; remainingRow < sz; remainingRow++)
            {
                // create one branch and do the works
                dsa2::matrix brMat(sz, ++creation_order);
                brMat.setLevel(temp.getLevel() + 1);
                brMat.setMatrix(temp.getMat());

                // swap the row under change;
                brMat.setMatrix(temp.getMat(), fixedRow, remainingRow);

                // put other rows in order
                for (ll j = fixedRow + 1, i = fixedRow; j < sz; j++, i++)
                {
                    if (i == remainingRow)
                    {
                        i++;
                        continue;
                    }
                    brMat.setMatrix(temp.getMat(), j, i, true);
                }

                brMat.setR(1 + fixedRow), brMat.setC(fixedCol);
                brMat.setB(dsa2::bound(brMat.getMat(), brMat.getR(), brMat.getC()));

                pq.push(brMat);
            }
            ++fixedRow;
        }
        else // col:
        {
            for (ll remainingCol = fixedCol; remainingCol < sz; remainingCol++)
            {
                dsa2::matrix brMat(sz, ++creation_order);
                brMat.setLevel(temp.getLevel() + 1);
                brMat.setMatrix(temp.getMat());

                // swap the column under change;
                brMat.setMatrix(temp.getMat(), fixedCol, remainingCol, false);

                // put other columns in order
                for (ll j = fixedCol + 1, i = fixedCol; j < sz; j++, i++)
                {
                    if (i == remainingCol)
                    {
                        i++;
                        continue;
                    }
                    brMat.setMatrix(temp.getMat(), j, i, false);
                }

                brMat.setC(fixedCol + 1), brMat.setR(fixedRow);
                brMat.setB(dsa2::bound(brMat.getMat(), brMat.getR(), brMat.getC()));

                pq.push(brMat);
            }
            ++fixedCol;
        }
    }
}

int main()
{
    std::cin >> dsa2::n;

    dsa2::matrix pmat(dsa2::n, 1);
    pmat.input();
    dsa2::run(pmat).print();
    return 0;
}