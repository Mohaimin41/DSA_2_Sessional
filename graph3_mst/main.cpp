#include <iostream>
#include <fstream>
#include "graph.h"

int main()
{
    //file input part
    std::ifstream inputFileStream("mst.txt", std::ios::in);
    
    int ver, edge;
    inputFileStream >> ver >> edge;
    
    // std::cout <<"v: " << ver << "E: "  << edge << "\n";

    graph t(ver);

    for (int i = 0; i < edge; i++)
    {
        int u, v;
        double weight;
        inputFileStream >> u >> v >> weight;

        
        // std::cout << "U: " << u << " v: " << v << " w: " << weight << "\n";

        t.addEdge(u, v, weight);
    }
    
    //close file
    inputFileStream.close();
    

    std::cout << "Cost of the minimum spanning tree: " << t.cost_mst(PRIM) << "\n";
    t.print_mst(PRIM);
    std::cout << "\n";
    t.print_mst(KRUSKAL);
    return 0;
}