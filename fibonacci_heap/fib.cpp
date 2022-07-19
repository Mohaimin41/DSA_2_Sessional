#include <iostream>
#include "myQueue.h"

using namespace std;

int main()
{
    fibonacci_heap *pq1 = new fibonacci_heap();
    fibonacci_heap::node *t[10];
    for (int i = 0; i < 10; i++)
    {

        t[i] = pq1->insert(make_pair(100 - i, i));

        // if (t != nullptr)
        //         std::cout << "successful insert of: " << i << ", " << 100-i
        //                   << " top:" << pq1->top().second << ", " << pq1->top().first << "\n";
        //     else
        //         std::cout << "fail\n";
    }

    for (int i = 0; i < 10; i++)
    {
        pq1->decrease_key(t[i], 10-i);

        if (pq1->top().first == 10-i && pq1->top().second == i)
            std::cout << "success: " << pq1->top().second << ", " << pq1->top().first  << "\n";
        else
            std::cout << "fail\n";
    }

    return 0;
}