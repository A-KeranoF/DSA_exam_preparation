#include <utility>
#include <vector>

using namespace std;

typedef AdjacencyList AdjacencyList;

namespace graph_traversals {
vector<int> bfs_get_path(const AdjacencyList& graph, int start, int target)
{
    vector<bool> visited(graph.size(), false); // save spot for all graph nodes and mark them NOT visited
    queue<int> neighbors;

    vector<int> parentPath(graph.size(), -1); // custom logic

    visited[start] = true;
    neighbors.push(start);

    while (!neighbors.empty()) {
        int parent = neighbors.front();
        neighbors.pop();

        for (int neighbor : graph[parent]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                neighbors.push(neighbor);

                // insert logic for custom task [here]

                // custom logic
                parentPath[neighbor] = parent;
                if (neighbor == target)
                    break;
            }
        }
    }

    // more custom logic, maybe could be brought out to a new function

    vector<int> path;

    if (!visited[target])
        return path; // no path

    for (int i = target; // now get parent of the target and push back into path.
        i != -1; // until we get the start node :
        i = parentPath[i]) // push back the parent of said parent
    {
        path.push_back(i);
    }

    // reverse the path because we traversed the parents in backwards manner, we need to fix it
    reverse(path.begin(), path.end()); // std::reverse

    return path;
}

void dfs(const AdjacencyList& graph, int start)
{
    vector<int> visited(graph.size(), false);
    stack<int> neighbors;

    neighbors.push(start);

    while (!toVisit.empty()) {
        int element = neighbors.top();

        // insert custom logic here 1

        neighbors.push(element); // simulate pushing the current to system stack for recursion. it is the same as the pushing to stack below but we need to preemptively put the current element to stack because a recursion will have already done it while making the callbacks (also we dont want to drop the first element, we need it for callbacks too)

        if (visited[element])
            continue;

        visited[element] = true;

        // insert custom logic here 2

        for (size_t i = graph.size() - 1;
            i >= 0;
            --i) // need to traverse from right to left so the right ones will come LATER while backtracking (because without recursion we cannot guarantee otherwise to call the right nodes); because the leftmost node is priority in general
        {
            // get the rightmost neighbor of current ELEMENT
            int rightmostNeighbor = graph[element][i];
            if (!visited[rightmostNeighbor])
                neighbors.push(rightmostNeighbor); // prepare elements for backtracking because we try to simulate recursion thst should call them in later moment if it was recursive
        }
    }
}

void dfsRecursiveStep(const AdjacencyList& graph, vector<int>& visited, int current)
{
    visited[current] = true;

    // insert custom logic here

    for (int neighbor : graph[current])
        if (!visited[neighbor])
            dfsRecursiveStep(graph, visited, neighbor)); // this is what the iterative version tries to emulate, but here the recursion simplifies the moments when we get to act with custom logic
}

void dfsRecursive(const AdjacencyList& graph, int startNode)
{
    vector<int> visited(graph.size(), false);
    dfsRecursiveStep(graph, visited, startNode);
}

struct weightedEdge {
    int toVertex = -1;
    int distance = 0;
    bool operator<(const edge& rhs) const // or just make custom comparator and pass it as template argument to the heap
    {
        return distance > rhs.distance;
        // it is opposite because by default the priority queue is max heap, // but we need min priority queue
    }
};

int dijkstra(const vector<vector<pair<int, int>>>& weightedGraph, const int start, const int target, vector<int>& path)
{
    vector<int> distances(weightedGraph.size(), INT_MAX);
    vector<int> prevs(weightedGraph.size(), -1); // aka parents
    vector<int> path;
    priority_queue<weightedEdge> priorityQueue;

    distances[start] = 0; // basically checking for being visited[start]
    priorityQueue.push(make_pair(start, 0)); // push edge

    while (!priorityQueue.empty()) {
        weightedEdge current = priorityQueue.top();
        priorityQueue.pop();

        // when target is reached, build the path, get the total distance cost and terminate
        if (current.toVertex == target) {
            int backtrack = target;
            // build the path from target backwards to origin
            while (backtrack != start) {
                path.push_back(backtrack);
                backtrack = prevs[backtrack];
            }
            path.push_back(start); // the start is missed in the if clause, we need to put it in the path

            reverse(path.begin(), path.end());

            return distances[current.toVertex];
        }

        // traverse neighbors, put them into heap, which will traverse by the lowest cost
        // (or find the costs if they are not known)
        for (pair<int, int> neighbor : weightedGraph[current.toVertex]) {
            int vertex = neighbor.first;
            int cost = neighbor.second + current.distance;

            if (cost < distances[vertex]) {
                distances[vertex] = cost; // "graph relaxation" to find indetermined frontal costs
                priorityQueue.push({ vertex, cost });
                prevs[vertex] = current.vertex;
            }
        }
    }

    return INT_MAX; // element could not be reached, no path
}

void dfsTopologicalSortRecursiveStep(const AdjacencyList& graph, int current, vector<int>& visited, vector<int>& result)
{
    visited[current] = true;

    for (int neighbor : graph[current])
        if (!visited[neighbor])
            dfsTopologicalSortRecursiveStep(graph, neighbor, visited, result));

    result.push_back(current);
}

vector<int> topologicalSort(const AdjacencyList& graph) const
{
    vector<int> result(graph.size());
    vector<bool> visited(graph.size(), false);

    for (size_t i = 0; i < graph.size(); ++i)
        if (!visited[i])
            dfsTopologicalSortRecursiveStep(graph, i, visited, result);

    reverse(result.begin(), result.end());
}

};

namespace graph_edge_opeartions {

void add_edge(AdjacencyList& graph, unsigned startVertex, unsigned endVertex)
{
    if (startVertex >= graph.size() || endVertex >= graph.size())
        throw std::runtime_error("Invalid vertex passed");

    graph[startVertex].push_back(endVertex);
    // if it was weighted, just insert a pair of the end vertex and the cost :)
}

void add_edge(vector<vector<pair<unsigned, double>>>& graph, unsigned startVertex, unsigned endVertex, double cost)
{
    if (startVertex >= graph.size() || endVertex >= graph.size())
        throw std::runtime_error("Invalid vertex passed");

    graph[startVertex].push_back(make_pair(endVertex, cost));
}

void remove_edge_oriented(AdjacencyList& graph, unsigned startVertex, unsigned endVertex)
{
    if (startVertex >= graph.size())
        throw std::runtime_error("Invalid first vertex passed");

    auto startIter = graph[startVertex].begin();

    while (startIter != graph[startVertex].end()) {
        if (*startIter == endVertex)
            break;
        startIter++;
    }

    if (startIter == graph[startVertex].end())
        throw std::runtime_error("Invalid second vertex passed");

    graph[startVertex].erase(startIter);
}

void remove_edge_NOT_oriented(AdjacencyList& graph, unsigned startVertex, unsigned endVertex)
{
    if (startVertex >= graph.size())
        throw std::runtime_error("Invalid first vertex passed");

    auto startIter = graph[startVertex].begin();
    while (startIter != graph[startVertex].end()) {
        if (*startIter == endVertex)
            break;
        startIter++;
    }

    if (startIter == graph[startVertex].end())
        throw std::runtime_error("Invalid second vertex passed");

    graph[startVertex].erase(startIter);

    // now take care of the symetrical edge

    auto endIter = graph[endVertex].begin();
    while (endIter != graph[endVertex].end()) {
        if (*endIter == startVertex)
            break;
        endIter++;
    }

    graph[endVertex].erase(endIter);
}
};

unsigned countConnectedComponents(const AdjacencyList& graph)
{
    unsigned count = 0;
    vector<int> visited(graph.size(), false);

    for (int i = 0; i < graph.size(); ++i) {
        if (!visited[i]) {
            graph_traversals::dfsRecursiveStep(graph, visited, startNode);
            count++;
        }
    }

    return count;
}

struct MST {
    std::vector<std::tuple<size_t, size_t, int>> edges;
    size_t sumOfWeights;
};

MST Prim(AdjacencyList& graph) const
{
    struct primEdge {
        size_t start;
        size_t end;
        int weight;
        bool operator>(const primEdge& other) const
        {
            return weight > other.weight;
        }
    };

    MST result;
    result.sumOfWeights = 0;

    std::priority_queue<primEdge, std::vector<primEdge>, std::greater<primEdge>> pq;
    std::vector<bool> visited(graph.size(), false);

    pq.push({ 0, 0, 0 });
    size_t addedEdges = 0;
    bool isFirst = true;

    while (addedEdges < graph.size() - 1 && !pq.empty()) {
        auto current = pq.top();
        pq.pop();

        if (visited[current.end]) {
            continue;
        }

        visited[current.end] = true;
        for (int i = 0; i < adj[current.end].size(); i++) {
            pq.push({ current.end,
                (size_t)adj[current.end][i].first,
                adj[current.end][i].second });
        }

        if (isFirst) {
            isFirst = false;
            continue;
        }

        // result.edges.push_back({ current.start, current.end, current.weight });
        result.edges.push_back({ current });
        result.sumOfWeights += current.weight;
        addedEdges++;
    }

    return result;
}

int main()
{
    // adjecency list (each index in the big vector corresponds to the node,
    // the elements inside are the neighbors (in this case - an undirected graph))
    AdjacencyList graph = {
        { 1, 2, 3 },
        { 0, 4 },
        { 0, 4, 5 },
        { 0, 6 },
        { 1, 2, 8 },
        { 2, 7 },
        { 3, 7 },
        { 5, 6, 8 },
        { 4, 7 }
    };

    return 0;
}
