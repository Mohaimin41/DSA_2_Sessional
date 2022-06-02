#include "graph.h"

using namespace std;

int main()
{
    int v, e, source, dest;
    bool has_neg = false;
    cin >> v >> e;
    graph t(v, e);
    for (int i = 0; i < e; i++)
    {
        int u, v, w;
        cin >> u >> v >> w;
        if (w < 0)
            has_neg = true;
        t.add_edge(u, v, w);
    }

    cin >> source >> dest;

    if (has_neg)
    {
        t.dijkstra(source);
        cout << "Shortest path cost: " << t.cost(source, dest, DIJKSTRA) << "\n";
        t.print_path(source, dest, DIJKSTRA);
    }
    else
    {
        if (t.bellman_ford(source))
        {
            cout << "The graph does not contain a negative cycle\nShortest path cost: " << t.cost(source, dest, BELLMAN_FORD) << "\n";
            t.print_path(source, dest, BELLMAN_FORD);
        }
        else
        {
            cout << "The graph contains a negative cycle\n";
        }
    }

    // t.print_vector_adjList();
    t.print_shortest_path_data(source, dest, BELLMAN_FORD);
    return 0;
}