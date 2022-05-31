#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>

#define INF INT_MAX
#define mp(a, b) std::make_pair(a, b)
#define edge_dec std::pair<int, int>

enum algo
{
    PRIM,
    KRUSKAL
};

class graph
{
    // adjacency list, array of vector of <next vertex, weight of this edge>
    std::vector<std::pair<int, double>> *Gr;

    // graph edge list, <weight, edge: a pair of vertices>
    std::vector<std::pair<double, edge_dec>> gr;

    // mst edge list, <weight, edge: a pair of vertices>
    std::vector<std::pair<double, edge_dec>> kruskal_tr;

    // using 0 indexed vertex numbering, V vertex: 0 to V-1 number
    // parent array, defines which set currently the vertice belongs to, initialized as own set only
    int *par;

    // vistitation arr for Prim's
    bool *isVisited;

    // key arr for Prim's
    double *key;

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

    void init()
    {
        for (int i = 0; i < V; i++)
        {
            isVisited[i] = false;
            par[i] = i;
            key[i] = INF;
        }
        kruskal_tr.clear();
    }

public:
    graph(int V)
    {
        this->V = V;

        Gr = new std::vector<std::pair<int, double>>[V];
        key = new double[V];
        par = new int[V];
        isVisited = new bool[V];

        // MAKE_SET step, O(V), also  isVisited
        init();

        gr.clear();
    }

    ~graph()
    {
        delete[] key;
        delete[] par;
        delete[] isVisited;
        delete[] Gr;
    }

    void addEdge(int u, int v, int weight)
    {
        gr.push_back(mp(weight, mp(u, v)));
        Gr[u].push_back(mp(v, weight));
        Gr[v].push_back(mp(u, weight));
    }

    void mst_kruskal()
    {
        init();
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

    void mst_prim()
    {

        init();

        // min priority queue, sorted over first value in pair,
        std::priority_queue<std::pair<double, int>,
                            std::vector<std::pair<double, int>>,
                            std::greater<std::pair<double, int>>>
            pq;

        // random src, 0 th ver, pushed with 0 key value, will stay at top now
        pq.push(mp(0, 0));
        key[0] = 0;

        //this is executed till all vertices have been pushed then popped or V times
        //with O(lg V) extraction, O(V lg V)
        // sum total = O(V lg V) + O(E lg V)
        while (!pq.empty())
        {
            // extract top/min keyed vertex u, O(lg V)
            int u = pq.top().second;
            pq.pop();

            // while iterating over adjacency list, same vertex may be added multiple times, so ignore visited vertex
            if (!isVisited[u])
            {
                isVisited[u] = true;

                // iterate through all adjacent of u
                // total adjacency list entry is 2 * |E| times, or total time for loop ran
                // on average then, O(E/V)
                // and total run time is in O(E * lg V)
                for (std::vector<std::pair<int, double>>::iterator it = Gr[u].begin();
                     it != Gr[u].end(); it++)
                {
                    int v = (*it).first;
                    double weight = (*it).second;

                    // if v is not visited AND it's key is greater than weight of edge connecting it to tree,
                    // we add it to pq, and update it's key and set u as parent
                    if (!isVisited[v] && key[v] > weight)
                    {
                        key[v] = weight;
                        // insertion in min priority queue O(lg V)
                        pq.push(mp(key[v], v));
                        par[v] = u;
                    }
                }
            }
        }
    }

    double cost_mst(algo a)
    {
        double res = 0;
        if (a == KRUSKAL)
        {
            mst_kruskal();

            for (int i = 0; i < kruskal_tr.size(); i++)
                res += kruskal_tr[i].first;
        }
        else
        {
            mst_prim();
            for (int i = 0; i < V; i++)
            {
                res += key[i];
            }
        }
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

        else
        {
            mst_prim();
            std::cout << "List of edges selected by Prim's: {";

            for (int i = 1; i < V; i++)
            {
                std::cout << "(" << par[i] << "," << i << ")";
                if (i < V - 1)
                    std::cout << ",";
            }
        }
    }
};