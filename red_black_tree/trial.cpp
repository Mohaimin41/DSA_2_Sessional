#include <iostream>
#include <fstream>

#include "rbtree.h"

int main()
{
    int j = 1000000;
    rbtree *tr = new rbtree();
    std::ofstream out("tr_out.txt", std::ios::out);
    std::cout << "start\n";

    // tr->insert(101);
    // tr->insert(j);

    out << "start test\n";
    // for (int i = 1; i <= j; i++)
    for (int i = j; i >= 1; i--)
    {
        // int k;
        // if (i % 2)
        // {
        //     tr->insert(i);
        //     k = i;
        //     if (tr->search(k) == false)
        //         out << "fail at : " << k << "\n";
        // }
        // else
        // {
        //     tr->insert(j - i);
        //     k = j - i;
        //     if (false == tr->search(j - i))
        //         out << "fail at : " << j - i << "\n";
        // }

        tr->insert(i);
        int f = tr->lesserKeyCount(i);
        // if (f != i - 1)
        if (tr->lesserKeyCount(i) != 0)
            out << "fail counting at :" << i << ", " << f << "\n";
        if (!tr->search(i))
            out << "fail insertion or search at: " << i << "\n";
    }
    // tr->printInOrder(tr->getRoot());
    for (int i = 1; i <= j; i++)
    {
        tr->remove(i);
        // out << tr->search(i) << " at " << i << "\n";
        if (tr->search(i))
            out << "fail removing " << i << "\n";
    }

    out << "end";
    out.close();
    std::cout << "end";
    return 0;
}