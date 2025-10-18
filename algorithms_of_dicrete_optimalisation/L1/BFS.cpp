#include <bits/stdc++.h>
using namespace std;

void BFS(const vector<vector<int>>& graph, int root) {
    int n = (int)graph.size();
    if (n == 0) return;
    if (root < 0 || root >= n) {
        cerr << "BFS root out of range.\n";
        return;
    }
    vector<bool> visited(n, false);
    queue<int> q;
    visited[root] = true;
    q.push(root);

    bool first = true;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        if (!first) cout << ' ';
        cout << (u + 1);
        first = false;
        for (int v : graph[u]) {
            if (!visited[v]) {
                visited[v] = true;
                q.push(v);
            }
        }
    }
    cout << '\n';
}

void DFS_helper(const vector<vector<int>>& graph, int u, vector<bool>& visited, bool &first) {
    visited[u] = true;
    if (!first) cout << ' ';
    cout << (u + 1);
    first = false;
    for (int v : graph[u]) {
        if (!visited[v]) DFS_helper(graph, v, visited, first);
    }
}

void DFS(const vector<vector<int>>& graph, int root) {
    int n = (int)graph.size();
    if (n == 0) return;
    if (root < 0 || root >= n) {
        cerr << "DFS root out of range.\n";
        return;
    }
    vector<bool> visited(n, false);
    bool first = true;
    DFS_helper(graph, root, visited, first);
    cout << '\n';
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    char flag;
    int n, m;

    if (!(cin >> flag)) {
        cerr << "Failed to read flag.\n";
        return 1;
    }
    if (flag != 'D' && flag != 'U') {
        cerr << "Use 'D' (directed) or 'U' (undirected) as a flag.\n";
        return 1;
    }
    if (!(cin >> n >> m)) {
        cerr << "Failed to read n and m.\n";
        return 1;
    }
    if (n <= 0) {
        cerr << "Number of vertices must be positive.\n";
        return 1;
    }

    vector<vector<int>> graph(n);

    for (int i = 0; i < m; ++i) {
        int u, v;
        if (!(cin >> u >> v)) {
            cerr << "Failed to read edge " << i << ".\n";
            return 1;
        }
        // convert to 0-based
        --u; --v;
        if (u < 0 || u >= n || v < 0 || v >= n) {
            cerr << "Edge endpoints out of range: " << u+1 << " " << v+1 << "\n";
            return 1;
        }
        graph[u].push_back(v);
        if (flag == 'U') graph[v].push_back(u);
    }

    // Sort adjacency lists for deterministic traversal
    for (int i = 0; i < n; ++i) {
        sort(graph[i].begin(), graph[i].end());
    }

    int start = 0; // start from node 1 (0-based index 0)
    cout << "BFS traversal (starting from node " << start+1 << "):\n";
    BFS(graph, start);

    cout << "DFS traversal (starting from node " << start+1 << "):\n";
    DFS(graph, start);

    return 0;
}
