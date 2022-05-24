#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <iterator>
#include <climits>

#define MAX_VER 1001
#define INF INT_MAX
#define minINF INT_MIN

// adjacency list
// 1-INDEXED GRAPH, 1-1000 VERTICES AVAILABLE
// THIS IS DIRECTED GRAPH, NEED TO USE TWO OPPOSSING EDGE FOR A UNDIRECTED GRAPH
std::vector<int> gr[MAX_VER];

// reverse direction graph
std::vector<int> revGr[MAX_VER];

// bfs parent array;
int parent[MAX_VER];

// bfs shortest distance from src vertex array, only useful by calling bfs directly with src
int distFromSrc[MAX_VER];

// dfs discover time array
int discTime[MAX_VER];

// dfs end visit time array
int endTime[MAX_VER];

// dfs global time var
int time;

// cycle present or not, bipartite or not
bool isCyclic = false;
bool bipartite = true;

// topological sort stack
std::stack<int> tstack;

enum color
{
    WHITE,
    RED,
    BLUE,
    GRAY,
    BLACK
};

enum traversal
{
    BFS,
    DFS
};

// coloring array for bipartite check
color biColor[MAX_VER];

// is visited color array: visited/white, visit/grey, finished visit/black
color isVisited[MAX_VER];

// connectivity check visit Array
bool visited1[MAX_VER];
bool visited2[MAX_VER];

void init()
{
    for (int i = 1; i <= MAX_VER; i++)
    {
        parent[i] = discTime[i] = endTime[i] = 0;
        biColor[i] = isVisited[i] = WHITE;
        distFromSrc[i] = INF;
    }
}

void input(int edge, int ver)
{
    for (int i = 1; i <= edge; i++)
    {
        int u, v;
        std::cin >> u >> v;
        gr[u].push_back(v);
        revGr[v].push_back(u);
    }
}

void printAdjacent(int edge, int ver)
{
    for (int i = 1; i <= ver; i++)
    {
        std::cout << i << ": ";

        for (std::vector<int>::iterator it = gr[i].begin(); it != gr[i].end(); it++)
        {
            std::cout << *it << " ";
        }
        std::cout << "\n";
    }
}

void preVisit(int v)
{
    std::cout << v << " ";
}

void postVisit(int v)
{
    // std::cout << v << " ";
}

void bfs(int src)
{
    // initialize vertices
    // for (int i = 1; i <= MAX_VER; i++)
    // {
    //     isVisited[i] = 0;
    //     distFromSrc[i] = INF;
    //     parent[i] = 0;
    // }
    // src grey/1
    isVisited[src] = GRAY;
    distFromSrc[src] = 0;
    parent[src] = 0;

    // queue starts
    std::queue<int> Q;
    Q.push(src);

    while (!Q.empty())
    {
        int u = Q.front();
        Q.pop();
        preVisit(u);

        // all neighbours of u traversed

        for (std::vector<int>::iterator it = gr[u].begin(); it != gr[u].end();
             it++)
        {
            // traversing and pushing this vertex
            if (isVisited[*it] == WHITE)
            {
                isVisited[*it] = GRAY;
                distFromSrc[*it] = distFromSrc[u] + 1;
                parent[*it] = u;
                Q.push(*it);
            }
        }
        isVisited[u] = BLACK;
    }
}

void dfs(int src)
{
    time++;
    discTime[src] = time;
    isVisited[src] = GRAY;
    preVisit(src);

    // visit deep into adjacent list
    for (std::vector<int>::iterator it = gr[src].begin(); it != gr[src].end(); it++)
    {
        if (isVisited[*it] == WHITE)
        {
            parent[*it] = src;
            // visit this node's neighbours now
            dfs(*it);
        }
        // if (isVisited[*it] == 1 && *it != src ) // a node has back edge to a grey node and not parent node so cycle
        // isCyclic = true;
    }
    isVisited[src] = BLACK;

    time++;
    endTime[src] = time;
    postVisit(src);
}

void graphTraverse(traversal t, int total_ver)
{
    init();

    for (int i = 1; i <= total_ver; i++)
        if (isVisited[i] == WHITE)
        {
            if (t == BFS)
                bfs(i);
            else
                dfs(i);
        }
}

bool cycleHelp(int v)
{
    isVisited[v] = GRAY;

    for (std::vector<int>::iterator it = gr[v].begin(); it != gr[v].end(); it++)
    {
        if (isVisited[*it] == GRAY && *it != v) // grey node in adjacent of grey node, cycle
        {
            std::cout << " src: " << v << " gray: " << *it << "\n";
            return true;
        }

        if (isVisited[*it] == WHITE && cycleHelp(*it))
        {
            std::cout << " src: " << v << " white: " << *it << "\n";
            return true;
        }
    }

    isVisited[v] = BLACK;
    return false;
}

bool hasCycle(int total_vert)
{
    init();

    for (int i = 1; i <= total_vert; i++)
    {
        if (isVisited[i] == WHITE && cycleHelp(i))
        {
            std::cout << "cycle :" << i << " ";
            return true;
        }
    }
    return false;
}
void topohelp(int v)
{
    isVisited[v] = BLACK;
    for (std::vector<int>::iterator it = gr[v].begin(); it != gr[v].end(); it++)
    {
        if (isVisited[*it] != BLACK)
            topohelp(*it);
    }
    // std::cout << v << " ";
    tstack.push(v);
}

void topoSort(int total_ver)
{
    init();

    for (int i = 1; i <= total_ver; i++)
        if (isVisited[i] != BLACK)
            topohelp(i);

    while (!tstack.empty())
    {
        std::cout << tstack.top() << " ";
        tstack.pop();
    }
    
}

bool isBipartite(int total_vert)
{
    init();
    std::queue<int> q;

    // loop to ensure checking all vertices
    for (int i = 1; i <= total_vert && biColor[i] == WHITE; i++)
    {
        // push and color
        q.push(i);
        biColor[i] = RED;

        // check adjacents
        while (!q.empty())
        {
            int curr = q.front();
            q.pop();

            for (std::vector<int>::iterator it = gr[curr].begin(); it != gr[curr].end();
                 it++)
            {
                // already colored same as parent color, not bipartite
                if (biColor[*it] == biColor[curr])
                    return false;

                // not colored yet
                if (biColor[*it] == WHITE)
                {
                    biColor[*it] = (biColor[curr] == BLUE) ? RED : BLUE;
                    q.push(*it);
                }
            }
        }
    }
    return true;
}

void dfs1(int i)
{
    visited1[i] = true;

    for (std::vector<int>::iterator it = gr[i].begin(); it != gr[i].end(); it++)
        if (!visited1[*it])
            dfs1(*it);
}

void dfs2(int i)
{
    visited2[i] = true;

    for (std::vector<int>::iterator it = revGr[i].begin(); it != revGr[i].end(); it++)
        if (!visited2[*it])
            dfs2(*it);
}

bool isConnectedUndirected(int total_ve)
{
    for (int i = 1; i <= total_ve; i++)
        visited1[i] = visited2[i] = false;

    // run dfs at vertex 1 from original graph and reverse direction graph
    dfs1(1);
    dfs2(1);

    // if any vertex is not visited in one of the dfs, graph is not connected
    for (int i = 1; i <= total_ve; i++)
        if (!visited1[i] && !visited2[i])
            return false;

    return true;
}

void printConnectedComponentUndirected(int total_ver)
{
    init();

    for (int i = 1; i <= total_ver; i++)
    {
        if (isVisited[i] == WHITE)
        {
            dfs(i);
            std::cout << "\n";
        }
    }
}