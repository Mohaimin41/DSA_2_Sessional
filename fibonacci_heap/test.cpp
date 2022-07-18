#include <fstream>
#include "graph.h"

using namespace std;

int main()
{
    // file input part
    std::ifstream inputFileStream("input.txt", std::ios::in);

    int v, e, source, dest;
    bool has_neg = false;
    // inputFileStream >> v >> e;
    cin >> v >> e;
    graph t(v, e);
    for (int i = 0; i < e; i++)
    {
        int u, v, w;
        // inputFileStream >> u >> v >> w;
        cin >> u >> v >> w;
        if (w < 0)
            has_neg = true;
        t.add_edge(u, v, w);
    }

    // inputFileStream >> source >> dest;
    cin >> source >> dest;

    if (!has_neg)
    {
        t.dijkstra(source, BINARY);
        // t.print_vector_adjList();
        // t.print_shortest_path_data(source, dest, DIJKSTRA, BINARY);

        cout << "Shortest path cost: " << t.cost(source, dest, DIJKSTRA, BINARY) << "\n";
        t.print_path(source, dest, DIJKSTRA, BINARY);
    }

    // t.print_vector_adjList();
    // t.print_shortest_path_data(source, dest, DIJKSTRA, BINARY);

    // close file
    inputFileStream.close();

    return 0;
}