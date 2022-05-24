#include <iostream>
#include "graph.h"

using namespace std;

int main()
{
    int e, v;
    cin >> v >> e;
    graph g(e, v);
    g.inputUndirected();
    g.printAdjacent();
    // graphTraverse(DFS, v);
    cout << "cycle: " << g.hasCycleUndirected() << "\n"
         << "bipartite: " << g.isBipartite() << "\n"
         << "connected: " << g.isConnectedUndirected() << "\n"
         << "components: \n";
    g.printConnectedComponentUndirected();
    // cout << "topological sort: ";
    // g.topoSort();
    return 0;
}