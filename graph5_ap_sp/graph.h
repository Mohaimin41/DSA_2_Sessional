#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <algorithm>
#include <climits>

#define INF INT_MAX
#define MAX_VER 1000
#define MAX_EDGE 10000
#define adj_vertex_dec std::pair<int, int>
#define edge_dec std::tuple<int, int, int>
#define mp(end_vertex, weight) std::make_pair(end_vertex, weight)
#define mt3(a, b, c) std::make_tuple(a, b, c)

enum algo
{
    DIJKSTRA,
    BELLMAN_FORD,
    FLOYD_WARSHAL,
    MATRIX_MUL
};

/* @brief   graph class with adjacency list, only shortest path methods for now
 * @todo    add other graph functions: delete_edge(param...) most important
 */
class graph
{
private:
    int ver, ed;

    /*  2d adjacency matrix, 0 for j == i, inf for no edge
        access: adj_mat[i][j] = weight(edge i to j)
     */
    int **adj_mat;

    int **D_old;
    int **D_new;

    /*  array of vector of <next vertex, edge weight>
        access: iterator it in adj_list[u], (*it).first = v, (*it).second = w */
    std::vector<adj_vertex_dec> adj_list[MAX_VER];

    /*  edge list, vector of <weight, origin, end>
        access: std::get<0>(edge_list[i]) = weight, <1>... = origin, <2>... = end*/
    std::vector<edge_dec> edge_list;

    int predecessor[MAX_VER];
    int dist[MAX_VER];

    /* @brief   dist and predecessor set to infinity and -1, dist[src] to 0
     * @param   src     source vertex for shortest path
     */
    void Initilization(int src)
    {
        for (int i = 0; i < ver; i++)
        {
            dist[i] = INF;
            predecessor[i] = -1;
        }
        dist[src] = 0;
    }

    /* @brief   updates dist[v] with lesser dist from src vertex and predecessor[v] with u
     * @param   u   origin of this edge
     * @param   v   end of this edge
     * @param   w   weight of this edge
     */
    void Relax(int u, int v, int w)
    {
        if (dist[u] != INF && dist[v] > dist[u] + w)
        {
            dist[v] = dist[u] + w;
            predecessor[v] = u;
        }
    }

    /*
     * @brief   makes a path by backtracking predecessor array from dest to src
     * @param   src,dest    of the path
     * @warning original values for predecessor array is -1 so may go over bound
     *          if the shortest path functions dont work properly
     *          May get stuck in loop again if shortest path functions don't work properly
     */
    std::vector<int> make_path(int src, int dest)
    {
        int curr = dest;

        std::vector<int> path;
        while (curr != src)
        {
            path.push_back(curr);
            curr = predecessor[curr];
        }

        path.push_back(src);

        std::reverse(path.begin(), path.end());
        return path;
    }

    /*
     *@brief matrix multiplication for shortest path, modifies D_old matrix, taking D_old in lieu of parameters
     */
    void extend_sp()
    {
        // store D_old = L(m) in D_new
        for (int i = 0; i < ver; i++)
            for (int j = 0; j < ver; j++)
                D_new[i][j] = D_old[i][j];

        // turn D_old = L(m) into L(2m)
        for (int k = 0; k < ver; k++)
        {
            for (int i = 0; i < ver; i++)
                for (int j = 0; j < ver; j++)
                    // D_new[i][j] = std::min(D_old[i][j], D_old[i][k] + D_old[k][j]);
                    if (D_old[i][j] > D_old[i][k] + D_old[k][j] && (D_old[i][k] != INF && D_old[k][j] != INF))
                        D_old[i][j] = D_old[i][k] + D_old[k][j];
        }
    }

public:
    graph(int v = MAX_VER, int e = MAX_EDGE)
    {
        ver = v;
        ed = e;

        adj_mat = new int *[ver], D_old = new int *[ver], D_new = new int *[ver];

        for (int i = 0; i < ver; i++)
            adj_mat[i] = new int[ver], D_old[i] = new int[ver], D_new[i] = new int[ver];

        for (int i = 0; i < ver; i++)
            for (int j = 0; j < ver; j++)
            {
                if (j == i)
                    adj_mat[i][j] = 0;
                else
                    adj_mat[i][j] = INF;
            }
    }

    ~graph()
    {
        for (int i = 0; i < ver; i++)
            delete[] adj_mat[i], delete[] D_old[i], delete[] D_new[i];

        delete[] adj_mat, delete[] D_old, delete[] D_new;
    }

    /* @brief   adds to adjacency list of origin vertex an <end, weight> pair, to edge list <weight, <origin,end>>
     * @param   origin,end,weight      of this edge
     */
    void add_edge(int origin, int end, int weight)
    {
        adj_list[origin].push_back(mp(end, weight));
        edge_list.push_back(mt3(weight, origin, end));
        adj_mat[origin][end] = weight;
    }

    void dijkstra(int src);

    bool bellman_ford(int src);

    void floyd_warshal();

    void matrix_mult();

    int cost(int src, int destination, algo a)
    {
        if (a == DIJKSTRA)
        {
            dijkstra(src);
        }
        else
        {
            bellman_ford(src);
        }
        return dist[destination];
    }

    void print_path(int src, int dest, algo a)
    {
        if (a == DIJKSTRA)
            dijkstra(src);
        else
            bellman_ford(src);

        std::vector<int> path = make_path(src, dest);

        for (int i = 0; i < path.size(); i++)
        {
            std::cout << path[i];
            if (i < path.size() - 1)
                std::cout << " -> ";
        }
        std::cout << "\n";
    }

    void print_vector_adjList()
    {
        std::cout << "vertex \t adjacents (vertex, weight)\n";
        std::cout << "=============================================\n";
        for (int i = 0; i < ver; i++)
        {
            std::cout << i << "\t";
            for (auto v : adj_list[i])
            {
                std::cout << "(" << v.first << ", " << v.second << ") ";
            }
            std::cout << "\n";
        }
    }

    void print_adj_mat()
    {
        for (int i = 0; i < ver; i++)
        {
            for (int j = 0; j < ver; j++)
                std::cout << adj_mat[i][j] << " \t";
            std::cout << "\n";
        }
    }

    void print_shortest_path_data(int src, int dest, algo a)
    {
        if (a == DIJKSTRA)
            dijkstra(src);
        else
            bellman_ford(src);

        std::cout << "src: " << src << ", dest: " << dest << "\n";
        std::cout << "vertex\tparent\tcost\n";
        std::cout << "============================================\n";
        for (int i = 0; i < ver; i++)
        {
            std::cout << i << "\t" << predecessor[i] << "\t" << dist[i] << "\n";
        }
    }

    void print_allPair_SP(algo a)
    {
        if (a == FLOYD_WARSHAL)
        {
            floyd_warshal();
            for (int i = 0; i < ver; i++)
            {
                for (int j = 0; j < ver; j++)
                    std::cout << D_old[i][j] << " \t";
                std::cout << "\n";
            }
        }
        std::cout << "\n";
        if (a == MATRIX_MUL)
        {
            matrix_mult();
            for (int i = 0; i < ver; i++)
            {
                for (int j = 0; j < ver; j++)
                    std::cout << D_new[i][j] << " \t";
                std::cout << "\n";
            }
        }
    }
};

/* @brief   updates dist and predecessor array with distance from src and parent vertex respectively
 * @param   src     source vertex to start from
 */
void graph::dijkstra(int src)
{
    Initilization(src);

    // min priority queue of  <distance, vertex>
    // access: pq.top().second = vertex, .first = distance to that vertex from src;
    // always the least distanced vertex from source will be at top
    std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> pq;

    // solution set/stack/array/vector/queue s
    // std::vector<int> solution;

    // src in queue
    pq.push(mp(dist[src], src));

    // total V times the while loop iterates
    while (!pq.empty())
    {
        // O(lg V)
        int u = pq.top().second;
        pq.pop();

        // insert u in solution
        // solution.push_back(u);

        // iterate over all adjacents, relax the edges going to adjacents from u
        // directed graph, in total E times for loop iterates
        // each edge is relaxed and pushed to queue if allowed
        for (auto vertex : adj_list[u])
        {
            // relaxation step, and we update pq too
            int v = vertex.first, w = vertex.second;
            if (dist[u] != INF && dist[v] > dist[u] + w)
            {
                // this edge can be relaxed and the vertex v can be put in queue
                dist[v] = dist[u] + w;
                predecessor[v] = u;
                pq.push(mp(dist[v], v));
            }
        }
    }
}

/*
 * @brief   returns true if no negative cycle present, updates dist and predecessor arrays
 * @param   src   source vertex for shortest path
 * @return  true if no negative cycle present, false otherwise
 */
bool graph::bellman_ford(int src)
{
    Initilization(src);

    for (int i = 1; i < ver; i++)
    {
        for (int j = 0; j < ed; j++)
        {
            int u = std::get<1>(edge_list[j]), v = std::get<2>(edge_list[j]);
            int w = std::get<0>(edge_list[j]);
            // int u = edge_list[j].second.first, v = edge_list[j].second.second, w = edge_list[j].first;
            Relax(u, v, w);
        }
    }

    for (int j = 0; j < ed; j++)
    {
        int u = std::get<1>(edge_list[j]), v = std::get<2>(edge_list[j]);
        int w = std::get<0>(edge_list[j]);
        // int u = edge_list[j].second.first, v = edge_list[j].second.second, w = edge_list[j].first;

        if (dist[u] != INF && dist[v] > dist[u] + w)
            return false;
    }
    return true;
}

/*
 *@brief floyd warshal algo implementation, runtime O(V^3)
 */
void graph::floyd_warshal()
{
    // D_old = adj_mat
    for (int i = 0; i < ver; i++)
        for (int j = 0; j < ver; j++)
            D_old[i][j] = adj_mat[i][j];

    for (int k = 0; k < ver; k++)
    {
        for (int i = 0; i < ver; i++)
            for (int j = 0; j < ver; j++)
                // D_new[i][j] = std::min(D_old[i][j], D_old[i][k] + D_old[k][j]);
                if (D_old[i][j] > D_old[i][k] + D_old[k][j] && (D_old[i][k] != INF && D_old[k][j] != INF))
                    D_old[i][j] = D_old[i][k] + D_old[k][j];

        // set D_old to D_new
        // for (int i = 0; i < ver; i++)
        //     for (int j = 0; j < ver; j++)
        //         D_old[i][j] = D_new[i][j];
    }
}

/*
 *@brief matrix multiplication implementation, runtime O(V^3 lg V), ans in D_new
 */
void graph::matrix_mult()
{
    // D_old = L(1)= adj_mat
    for (int i = 0; i < ver; i++)
        for (int j = 0; j < ver; j++)
            D_old[i][j] = adj_mat[i][j];

    int m = 1;

    while (m < ver - 1)
    {
        // get a L(2m) from D_old = L(m)
        extend_sp();
        m = 2 * m;
    }
}