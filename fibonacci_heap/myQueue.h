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
    virtual void push(const std::pair<int, int> &item) = 0;
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
    void push(const std::pair<int, int> &item) override
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
private:
    struct node
    {
        std::pair<int, int> item;
        node *right;
        node *left;
        node *parent;
        node *child;
        bool mark;
        int degree;
        node() : item{}, right(nullptr), left(nullptr), parent(nullptr),
                 child(nullptr), mark(false), degree(0) {}
        node(const std::pair<int, int> &i, node *r = nullptr, node *l = nullptr,
             node *p = nullptr, node *c = nullptr)
            : item(i), right(r), left(l),
              parent(p), child(c), mark(false), degree(0) {}
    };
    node *Hmin;
    int Hn;

    void LINK(node *y, node *x)
    {
        //remove y from the root list of H
        y->right->left = y->left;
        y->left->right = y->right;
        //make y a child of x, incrementing x : degree
        y->parent = x;
        y->left = x->child;
        y->right = x->child->right;
        x->right->left = y;
        x->right = y;
        
        x->degree++;

        y->mark = false;
    }

    void CONSOLIDATE()
    {
        int DHn = logPhi(Hn);
        node *A[DHn] = {nullptr};

        // for each node w in the root list of H
        node *r = Hmin;
        node *next = r->right;
        do
        {
            node *x = r;
            int d = x->degree;

            while (A[d] != nullptr)
            {
                node *y = A[d];

                if (x->item.first > y->item.first)
                    std::swap(*x, *y);

                LINK(y, x);
                A[d] = nullptr;
                d++;
            }
            A[d] = x;
            
            r = next;
            next = next->right;

        } while (next != r);

        Hmin = nullptr;
        for (int i = 0; i < logPhi(Hn); i++)
        {
            if (A[i] != nullptr)
            {
                if (Hmin == nullptr)
                {
                    Hmin = A[i];
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

public:
    fibonacci_heap() : Hmin(new node()), Hn(0) {} // MAKE-FIB-HEAP

    ~fibonacci_heap() {}
    
    void clear() override {}
    
    void push(const std::pair<int, int> &item) override // FIB-HEAP-INSERT
    {
        node *toInsert = new node(item);
        if (!Hn)
        {
            // there is hmin, so change it to toInsert
            Hmin = toInsert;
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
    }
    
    std::pair<int, int> top() const override
    { // MINIMUM
        return Hmin->item;
    }
    
    void pop() override
    { // FIB-HEAP-EXTRACT-MIN
        node *z = Hmin;
        if (Hn)
        {
            // for each child of z, add them to root list
            node *kid = z->child;
            node *next = kid->right;
            do
            {

                kid->left->right = kid->right; // step 1: removed kid
                kid->right->left = kid->left;
                kid->right = Hmin->right; // step 2: kid has two new side node
                kid->left = Hmin;
                Hmin->right->left = kid; // step 3: kid's side nodes now have kid
                Hmin->right = kid;
                kid->parent = nullptr; // step 4
                // proceed pointer
                kid = next;
                next = next->right;
            } while (next != kid);

            // remove z from root list
            z->right->left = z->left; // z's neighbours have lost z;
            z->left->right = z->right;

            if (z->right == z)
                Hn = 0;
            else
            {
                Hmin = z->right;
                CONSOLIDATE();
            }
        }
    }
    
    bool empty() override { return Hn; }
};