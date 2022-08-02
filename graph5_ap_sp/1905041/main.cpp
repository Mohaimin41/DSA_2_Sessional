#include <fstream>
#include <cmath>
#include "graph.h"

using namespace std;

int main()
{
    //file input part
    std::ifstream inputFileStream("input.txt", std::ios::in);

    int v, e;
    inputFileStream >> v >> e;
    // cin >> v >> e;
    graph t(v, e);
    for (int i = 0; i < e; i++)
    {
        int u, v;
        double w;
        inputFileStream >> u >> v >> w;
        // cin >> u >> v >> w;
       
        // Input vertices are 1 indexed, so minusing 1 here
        t.add_edge(u-1, v-1, w);
    }

    t.print_allPair_SP(MATRIX_MUL);

    // close file
    inputFileStream.close();

    return 0;
}
