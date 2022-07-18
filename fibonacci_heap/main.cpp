#include <iostream>
#include <fstream>
#include <cstdio>
#include <chrono>
#include <iomanip>

#include "graph.h"

using namespace std;

int main()
{
    // input file 1, output redirect
    // freopen("input1.txt", "r", stdin);
    ifstream inputstream ("input1.txt", std::ios::in);
    // freopen("output.txt", "w", stdout);
    ofstream outputstream("output.txt", std::ios::out);
    
    // input file 1, set graph
    int n, m;//n vertices, m edges;
    cin >> n >> m;
    graph G(n, m);
    int u, v, w;
    for (int i = 0; i < m; i++)
    {
        cin >> u >> v >> w;
        G.add_edge(u, v, w);
    }

    // input file 2 redirect
    // freopen("input2.txt", "w", stdin);
    inputstream.close();
    ifstream input2stream ("input2.txt", std::ios::in);

    // for each line in file 2, use dijkstra with two heaps for cost, path length and time, print in output file
    int t;
    cin >> t;
    while(t--)
    {
        double total = 0.0;
        
        int src, dest;
        cin >> src >> dest;

        //time start
        auto start = chrono::high_resolution_clock::now();
        
        // do dijkstra with binary heap, put cost, path length
        G.dijkstra(src, BINARY);
        cout << G.print_path_length(src, dest, DIJKSTRA, BINARY) << " " << G.cost(src, dest, DIJKSTRA, BINARY) << " " ;
        
        // find end time
        total += double(chrono::duration_cast<chrono::nanoseconds>(
                            chrono::high_resolution_clock::now() - start)
                            .count() /
                        1000000.0);
        // write end time in millisecond
        cout << total << " ";

        // same for fibonacci heap
        total = 0.0;

        auto start1 = chrono::high_resolution_clock::now();
        
        // do dijkstra with fibonacci heap
        G.dijkstra(src, FIBONACCI);
        cout << G.print_path_length(src, dest, DIJKSTRA, FIBONACCI) << " " << G.cost(src, dest, DIJKSTRA, FIBONACCI) << " " ;
        
        // find end time
        total += double(chrono::duration_cast<chrono::nanoseconds>(
                            chrono::high_resolution_clock::now() - start1)
                            .count() /
                        1000000.0);
        // write end time in millisecond
        cout << total << "\n";

    }


    input2stream.close();
    outputstream.close();
    return 0;
}