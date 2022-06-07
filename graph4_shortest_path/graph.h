#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>

#define INF INT_MAX
#define MAX_VER 1000
#define MAX_EDGE 10000
#define adj_vertex_dec std::pair<int, int>
#define edge_dec std::pair<int, std::pair<int, int>>
#define mp(end_vertex, weight) std::make_pair(end_vertex, weight)

enum algo
{
    DIJKSTRA,
    BELLMAN_FORD
};

/* @brief   graph class with adjacency list, only shortest path methods for now
 * @todo    add other graph functions: delete_edge(param...) most important
 */
class graph
{
private:
    int ver, ed;

    /*  array of vector of <next vertex, edge weight>
        access: iterator it in adj_list[u], (*it).first = v, (*it).second = w */
    std::vector<adj_vertex_dec> adj_list[MAX_VER];

    /*  edge list, vector of <weight, <origin, end>>
        access: edge_list[i].second.first = origin, *.second  = end, edge_list[i].first = weight*/
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

public:
    graph(int v = MAX_VER, int e = MAX_EDGE)
    {
        ver = v;
        ed = e;
    }

    /* @brief   adds to adjacency list of origin vertex an <end, weight> pair, to edge list <weight, <origin,end>>
     * @param   origin,end,weight      of this edge
     */
    void add_edge(int origin, int end, int weight)
    {
        adj_list[origin].push_back(mp(end, weight));
        edge_list.push_back(mp(weight, mp(origin, end)));
    }

    void dijkstra(int src);

    bool bellman_ford(int src);

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
            int u = edge_list[j].second.first, v = edge_list[j].second.second, w = edge_list[j].first;
            Relax(u, v, w);
        }
    }

    for (int j = 0; j < ed; j++)
    {
        int u = edge_list[j].second.first, v = edge_list[j].second.second, w = edge_list[j].first;

        if (dist[u] != INF && dist[v] > dist[u] + w)
            return false;
    }
    return true;
}