#include <iostream>
#include <ostream>
#include <cmath>
#include <climits>

#define Ninf -INF
#define phi (1 + sqrt(5)) / 2.0
#define logPhi(x) log(x) / log(phi)

class my_priority_queue
{
private:
    my_priority_queue(const my_priority_queue &) {}
    void operator=(const my_priority_queue &) {}

public:
    my_priority_queue() {}
    virtual void clear() = 0;
    virtual void *push(const std::pair<int, int> &item) = 0;
    virtual std::pair<int, int> top() const = 0;
    virtual void pop() = 0;
    virtual bool empty() = 0;
};

class binary_pq : public my_priority_queue
{
private:
    // data
    int capacity = 4001; // actual size + 1 for 1-indexed tree
    int curr_pos;
    std::pair<int, int> *arr;

    int left(int pos) const { return pos << 1; }

    int right(int pos) const { return (pos << 1) | 1; }

    int parent(int pos) const
    {
        //(pos > 0) ? return (pos >> 1) : return 0;
        int par;
        (pos > 0) ? par = (pos >> 1) : par = 0;
        return par;
    }

    void siftUp_inArray(int pos) // sifts down in tree
    {
        // first we compare pos with its children, if they be smaller than pos
        // we swap, then we do same again, till reaching half of array

        while (pos < curr_pos / 2) // curr_pos / 2 is penultimate level, next level all nodes are leaves
        {
            int larger_child = (arr[left(pos)].first >= arr[right(pos)].first)
                                   ? left(pos)
                                   : right(pos);

            if (arr[pos].first >= arr[larger_child].first)
            {
                std::swap(arr[pos], arr[larger_child]);
                pos = larger_child;
            }
            else
            {
                return; // break out of loop and function as the value reached proper place
            }
        }
    }

public:
    binary_pq() : curr_pos(0), arr(new std::pair<int, int>[capacity])
    {
    }
    ~binary_pq() { delete[] arr; }
    void clear() override
    {
        curr_pos = 0;
        delete[] arr;
        arr = new std::pair<int, int>[capacity];
    }
    void *push(const std::pair<int, int> &item) override
    {
        // increase size and insert
        arr[++curr_pos] = item;
        int it = curr_pos;

        // now sift it up in tree, down in array
        while (parent(it) && arr[parent(it)].first > arr[it].first)
        {
            std::swap(arr[parent(it)], arr[it]);
            it = parent(it);
        }
        return nullptr;
    }
    std::pair<int, int> top() const override
    {
        return arr[1];
    }
    void pop() override
    {
        if (!curr_pos)
        {
            std::cout << "ERROR: priority queue empty";
        }
        else
        {
            arr[1] = arr[curr_pos--];
            siftUp_inArray(1);
        }
    }
    bool empty() override { return curr_pos == 0; }
};

class fibonacci_heap : public my_priority_queue
{
public:
    struct node
    {
        std::pair<int, int> item;
        node *right;
        node *left;
        node *parent;
        node *child;
        bool mark;
        int degree;

        node() : item{}, right(this), left(this), parent(nullptr),
                 child(nullptr), mark(false), degree(0) {}

        node(const std::pair<int, int> &i, node *r = nullptr, node *l = nullptr,
             node *p = nullptr, node *c = nullptr)
            : item(i), right(r), left(l), parent(p), child(c), mark(false), degree(0)
        {
            if (r == nullptr)
                right = this;
            if (l == nullptr)
                left = this;
        }

        void print()
        {
            std::cout << "value: " << item.second << ", key: " << item.first << ", mark: " << mark << ", degree: " << degree << ", ";
            if (right != nullptr)
            {
                std::cout << "right: " << right->item.second << ", " << right->item.first << ", ";
            }
            if (left != nullptr)
            {

                std::cout << "left: " << left->item.second << ", " << left->item.first;
            }
            std::cout << "\n";
        }
    };

private:
    node *Hmin;
    int Hn;

    void LINK(node *y, node *x)
    {

        // remove y from the root list of H
        y->right->left = y->left;
        y->left->right = y->right;
        // make y a child of x, incrementing x : degree
        if (x->child != nullptr)
        {
            y->parent = x;
            y->left = x->child;
            y->right = x->child->right;
            x->child->right->left = y;
            x->child->right = y;
        }
        else
        {
            x->child = y;
            y->parent = x;
            y->left = y->right = y;
        }
        x->degree++;

        y->mark = false;
    }

    void CONSOLIDATE()
    {
        int DHn = floor(logPhi(Hn)) + 1;
        node *A[DHn];
        for (int i = 0; i < DHn; i++)
            A[i] = nullptr;

        // for each node w in the root list of H
        node *r = Hmin;
        node *next = r;

        // first count and put them on a array
        int t = 0;
        do
        {
            t++;
            next = next->right;
        } while (r != next);

        node *container[t];

        int i = 0;
        next = r;

        do
        {
            container[i++] = next;
            next = next->right;

        } while (r != next);

        for (i = 0; i < t; i++)
        {
            node *x = container[i];
            int d = x->degree;

            while (A[d] != nullptr)
            {
                node *y = A[d];

                if (x->item.first > y->item.first)
                    std::swap(x, y);

                LINK(y, x);

                A[d] = nullptr;
                d++;
            }
            A[d] = x;
        }

        Hmin = nullptr;

        for (int i = 0; i < DHn; i++)
        {
            if (A[i] != nullptr)
            {
                if (Hmin == nullptr)
                {
                    Hmin = A[i];
                    A[i]->right = A[i]->left = A[i];
                }
                else
                {
                    A[i]->right = Hmin->right; // step 1: put A[i] in middle
                    A[i]->left = Hmin;
                    Hmin->right->left = A[i]; // step 2: change pointers of both side nodes
                    Hmin->right = A[i];

                    // check if hmin change needed
                    if (Hmin->item.first > A[i]->item.first)
                        Hmin = A[i];
                }
            }
        }
    }

    void CUT(node *x, node *y)
    {
        // remove x from the child list of y, decrementing y:degree
        if (x->right == x && x->left == x)
            y->child = nullptr;
        else
            y->child = x->right;

        x->right->left = x->left;
        x->left->right = x->right;

        y->degree--;

        // add x to the root list of H, right of Hmin
        x->left = Hmin;
        x->right = Hmin->right;
        Hmin->right->left = x;
        Hmin->right = x;

        x->parent = nullptr;
        x->mark = false;
    }

    void CASCADING_CUT(node *y)
    {
        node *z = y->parent;

        if (z != nullptr)
        {
            if (y->mark == false)
                y->mark = true;
            else
            {
                CUT(y, z);
                CASCADING_CUT(z);
            }
        }
    }

public:
    fibonacci_heap() : Hmin(new node()), Hn(0) {} // MAKE-FIB-HEAP

    ~fibonacci_heap() {}

    void clear() override {}

    void *push(const std::pair<int, int> &item) override { return nullptr; }

    node *insert(const std::pair<int, int> &item) // FIB-HEAP-INSERT
    {
        node *toInsert = new node(item);
        if (!Hn)
        {
            // there is hmin, so change it to toInsert
            Hmin = toInsert;
            Hmin->left = Hmin->right = Hmin;
        }
        else
        {
            // insertion in root list at right of Hmin
            toInsert->right = Hmin->right; // step 1: put toInsert in middle
            toInsert->left = Hmin;
            Hmin->right->left = toInsert; // step 2: change pointers of both side nodes
            Hmin->right = toInsert;

            // check if hmin change needed
            if (Hmin->item.first > toInsert->item.first)
                Hmin = toInsert;
        }

        Hn++;
        return toInsert;
    }

    /*
     *@brief    returns nullptr for empty heap, should check with empty() before calling it
     */
    std::pair<int, int> top() const override
    { // MINIMUM
        // std::cout << "Hn: " << Hn << "\n";
        // if (Hmin == nullptr)
        //     std::cout << "ERROR: heap empty.\n";
        // else
        //     std::cout << "returning: " << Hmin->item.second << ", " << Hmin->item.first << "\n";
        return Hmin->item;
    }

    void pop() override
    { // FIB-HEAP-EXTRACT-MIN
        node *z = Hmin;

        if (Hn > 0)
        {
            // std::cout << "@HN : " << Hn << ", popping :" << z->item.second << ", " << z->item.first << "\n";
            // z->print();
            // z->left->print();
            // z->right->print();
            // std::cout << "====================================================\n";
            // // for each child of z, add them to root list
            int tc = 0;
            if (z->child != nullptr)
            {
                node *kid = z->child;
                node *nextKid = kid;

                for (int i = 0; i < z->degree; i++)
                {
                    node *temp = nextKid;
                    // proceed pointer
                    nextKid = nextKid->right;
                    // move this child to root list
                    // temp->left->right = temp->right; // step 1: removed temp
                    // temp->right->left = temp->left;
                    temp->right = Hmin->right; // step 2: temp has two new side node
                    temp->left = Hmin;
                    Hmin->right->left = temp; // step 3: temp's side nodes now have temp
                    Hmin->right = temp;
                    temp->parent = nullptr; // step 4

                    tc++;
                }
            }

            // remove z from root list
            z->right->left = z->left; // z's neighbours have lost z;
            z->left->right = z->right;

            if (z->right == z) // big mistakes here
            {
                Hn = 0;
            }
            else
            {
                z->right->left = z->left; // removing z = Hmin from root list
                z->left->right = z->right;
                Hmin = z->right;

                CONSOLIDATE();
            }
            Hn--;
        }
    }

    bool empty() override
    {
        // std::cout << "Hn now: " << Hn << "\n";
        return Hn <= 0;
    }

    void decrease_key(node *x, int k) // FIB-HEAP-DECREASE-KEY
    {

        if (k > x->item.first)
            std::cout << "Given key k: " << k << " larger than existing key for node: " << x->item.second << ", " << x->item.first << "\n";
        else
        {
            x->item.first = k;
            node *y = x->parent;

            if (y != nullptr && x->item.first < y->item.first)
            {
                CUT(x, y);
                CASCADING_CUT(y);
            }

            if (x->item.first < Hmin->item.first)
                Hmin = x;
        }
    }
};