#include <fstream>
#include <cmath>
#include "graph.h"

using namespace std;

int main()
{
    //file input part
    std::ifstream inputFileStream("input2.txt", std::ios::in);

    int v, e, source, dest;
    bool has_neg = false;
    //inputFileStream >> v >> e;
    cin >> v >> e;
    graph t(v, e);
    for (int i = 0; i < e; i++)
    {
        int u, v;
        double w;
        //inputFileStream >> u >> v >> w;
        cin >> u >> v >> w;
        // w = -log(w);
        if (w < 0)
            has_neg = true;
        // mahabhu's test case vertices are 1 indexed, so minusing 1 here
        t.add_edge(u-1, v-1, w);
    }

//    inputFileStream >> source >> dest;
    // cin >> source >> dest;

    if (!has_neg)
    {
        // t.dijkstra(source);
        // cout << "Shortest path cost: " << t.cost(source, dest, DIJKSTRA) << "\n";
        // t.print_path(source, dest, DIJKSTRA);
    }
    else
    {
        // if (t.bellman_ford(source))
        // {
        //     cout << "The graph does not contain a negative cycle\nShortest path cost: " << t.cost(source, dest, BELLMAN_FORD) << "\n";
        //     t.print_path(source, dest, BELLMAN_FORD);
        // }
        // else
        // {
        //     cout << "The graph contains a negative cycle\n";
        // }
    }

    // t.print_vector_adjList();
    // t.print_shortest_path_data(source, dest, BELLMAN_FORD);
    // t.print_adj_mat();
    // t.floyd_warshal();
    t.print_allPair_SP(MATRIX_MUL);

    // close file
    inputFileStream.close();

    return 0;
}
