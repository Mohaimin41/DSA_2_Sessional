#include <iostream>
#include <fstream>

#include "rbtree.h"

int main()
{
    rbtree *rb = new rbtree();

    std::ifstream input("input.txt", std::ios::in);
    std::ofstream output("output.txt", std::ios::out);
    int t;
    input >> t;
    output << t << "\n";
    while (t--)
    {
        int e, x, r;
        input >> e >> x;
        output << e << " " << x << " ";

        if (e == 1)
            r = rb->insert(x);
        else if (e == 0)
            r = rb->remove(x);
        else if (e == 2)
            r = rb->search(x);
        else if (e == 3)
            r = rb->lesserKeyCount(x);

        output << r << "\n";
    }
    input.close();
    output.close();

    return 0;
}