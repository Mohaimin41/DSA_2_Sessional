#include <iostream>
#include <vector>
#include <algorithm>

#define mp(a, b) std::make_pair(a, b)
#define edge_dec std::pair<int, int>

enum algo
{
    PRIM,
    KRUSKAL
};

class graph
{
    // graph edge list, <weight, edge: a pair of vertices>
    std::vector<std::pair<double, edge_dec>> gr;

    // mst edge list, <weight, edge: a pair of vertices>
    std::vector<std::pair<double, edge_dec>> kruskal_tr;

    // using 0 indexed vertex numbering, V vertex: 0 to V-1 number
    // parent array, defines which set currently the vertice belongs to, initialized as own set only
    int *par;

    // num of nodes
    int V;

    
    // finds which tree/set the vertex is in, O(V)
    int FIND_SET(int vertex)
    {
        // if vertex is own parent, it is in it's own tree/set
        if (par[vertex] == vertex)
            return vertex;

        // else recurse back to tree root, as this root identifies the tree/set
        else
            return FIND_SET(par[vertex]);
    }

    // make the parent of u same as parent of v, or adding them in same tree/set
    void UNION_VER(int u, int v)
    {
        par[u] = par[v];
    }


public:
    graph(int V)
    {
        this->V = V;
        par = new int[V];

        // MAKE_SET step, O(V)
        for (int i = 0; i < V; i++)
            par[i] = i;

        gr.clear();
        kruskal_tr.clear();
    }

    ~graph()
    {
        delete[] par;
    }

    void addEdge(int u, int v, int weight)
    {
        gr.push_back(mp(weight, mp(u, v)));
    }

    void mst_kruskal()
    {
        // sorting the edge list by weight in ascending order, O(E log E)
        std::sort(gr.begin(), gr.end());

        // iterate through all edge, O(E)
        for (int i = 0; i < gr.size(); i++)
        {
            int u = gr[i].second.first, v = gr[i].second.second;

            // if not in same tree/set, find_set O(V)
            if (FIND_SET(u) != FIND_SET(v))
            {
                kruskal_tr.push_back(mp(gr[i].first, mp(u, v)));
                UNION_VER(u, v);
            }
        }
    }

    int cost_mst()
    {
        mst_kruskal();
        int res = 0;

        for (int i = 0; i < kruskal_tr.size(); i++)
            res += kruskal_tr[i].first;

        return res;
    }

    void print_mst(algo a)
    {

        if (a == KRUSKAL)
        {
            mst_kruskal();
            std::cout << "List of edges selected by Kruskal's: {";

            for (int i = 0; i < kruskal_tr.size(); i++)
            {
                std::cout << "(" << kruskal_tr[i].second.first << "," << kruskal_tr[i].second.second << ")";
                if (i < kruskal_tr.size() - 1)
                    std::cout << ",";
            }

            std::cout << "}";
        }
    }
};