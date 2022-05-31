#include <iostream>
#include <fstream>
#include "graph.h"

int main()
{
    // graph test(4);
    // test.addEdge(0,1,3);
    // test.addEdge(1,2,8);
    // test.addEdge(2,3,9);
    // test.addEdge(3,1,6);
    // std::freopen("mst.txt", "r", stdin);

    //file input part
    std::ifstream inputFileStream("mst.txt", std::ios::in);
    std::streambuf *stream_buffer_file_in = inputFileStream.rdbuf();

    // Backup streambuffers of cin cout
    std::streambuf *stream_buffer_cin = std::cin.rdbuf();

    //redirection part
    std::cin.rdbuf(stream_buffer_file_in);


    int ver, edge;
    std::cin >> ver >> edge;
    std::cout <<"v: " << ver << "E: "  << edge << "\n";

    graph t(ver);

    for (int i = 0; i < edge; i++)
    {
        int u, v, weight;
        std::cin >> u >> v >> weight;

        
        std::cout << "U: " << u << "v: " << v << "w: " << weight << "\n";

        t.addEdge(u, v, weight);
    }

    // sets cin streambuffer and returns the old
    std::cin.rdbuf(stream_buffer_cin);
    //close file
    inputFileStream.close();
    

    std::cout << "Cost of the minimum spanning tree: " << t.cost_mst() << "\n";
    t.print_mst(KRUSKAL);
    return 0;
}