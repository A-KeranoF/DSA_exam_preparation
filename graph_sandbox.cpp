#include <vector>

using namespace std;

vector<int> bfs_get_path(const vector<vector<int>>& graph, int start, int target)
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

void dfs(const vector<vector<int>>& graph, int start)
{
    vector<int> visited(graph.size(), false);
    stack<int> neighbors;

    visited[start] = true;
    neighbors.push(start);

    while (!toVisit.empty()) {
        int element = neighbors.top();

        if (visited[element])
            continue;

        visited[element] = true;

        // insert custom logic here 1

        neighbors.push(element); // simulate pushing the current to system stack for recursion. it is the same as the pushing to stack below but we need to preemptively put the current element to stack because a recursion will have already done it while making the callbacks (also we dont want to drop the first element, we need it for callbacks too)

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

void dfsRecursiveStep(const vector<vector<int>>& graph, vector<int>& visited, int current)
{
    visited[current] = true;

    // insert custom logic here

    for (int neighbor : graph[current])
        if (!visited[neighbor])
            dfsRecursiveStep(graph, visited, neighbor)); // this is what the iterative version tries to emulate, but here the recursion simplifies the moments when we get to act with custom logic
}

void dfsRecursive(const vector<vector<int>>& graph, int startNode)
{
    vector<int> visited(graph.size(), false);
    dfsRecursiveStep(graph, visited, startNode);
}

int main()
{
    // adjecency list (each index in the big vector corresponds to the node,
    // the elements inside are the neighbors (in this case - an undirected graph))
    vector<vector<int>> graph = {
        { 3, 4, 5 },
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
