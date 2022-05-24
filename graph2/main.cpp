#include <iostream>
#include "graph.h"

using namespace std;

int main()
{
    int e, v;
    cin >> v >> e;
    input(e, v);
    printAdjacent(e, v);
    // graphTraverse(DFS, v);
    cout << "cycle: " << hasCycle(v) << "\n"
         << "bipartite: " << isBipartite(v) << "\n"
         << "connected: " << isConnectedUndirected(v) << "\n"
         << "components: \n";
    printConnectedComponentUndirected(v);
    cout << "topological sort: ";
    topoSort(v);
    return 0;
}