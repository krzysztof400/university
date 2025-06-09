#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>

int assignments = 0;
int comparisons = 0;

void assign(int &a, int b) {
    a = b;
    assignments++;
}

bool compare(int a, int b) {
    comparisons++;
    return a > b;
}

using Edge = std::tuple<int, int, double>;

struct DisjointSet {
    std::vector<int> parent, rank;

    DisjointSet(int n) : parent(n), rank(n, 0) {
        for (int i = 0; i < n; ++i) assign(parent[i], i);
    }

    int find(int u) {
        if (parent[u] != u) { // Fix: Correct condition for path compression
            assign(parent[u], find(parent[u]));
        }
        return parent[u];
    }

    void unite(int u, int v) {
        u = find(u);
        v = find(v);
        if (u != v) {
            if (rank[u] < rank[v]) std::swap(u, v); // Fix: Simplified rank comparison
            assign(parent[v], u);
            if (rank[u] == rank[v]) assign(rank[u], rank[u] + 1);
        }
    }
};

void kruskalMST(int n, const std::vector<Edge>& edges) {
    assignments = 0; // Reset counters
    comparisons = 0;

    DisjointSet ds(n);
    std::vector<Edge> mst;
    double totalWeight = 0;

    for (const auto& [u, v, w] : edges) {
        if (ds.find(u) != ds.find(v)) {
            ds.unite(u, v);
            mst.push_back({u, v, w});
            totalWeight += w;
        }
    }

    int size  = mst.size(); 
    if (size != n - 1) { // Check if MST is valid
        std::cout << "The graph is disconnected. MST cannot be formed." << std::endl;
        return;
    }

    std::cout << "MST (Kruskal):" << std::endl;
    for (const auto& [u, v, w] : mst) {
        std::cout << u << " - " << v << " : " << w << std::endl;
    }
    std::cout << "Total Weight: " << totalWeight << std::endl;
    std::cout << "Assignments: " << assignments << ", Comparisons: " << comparisons << std::endl;
}

int main() {
    int n;
    std::cin >> n;
    std::vector<Edge> edges;

    int u, v;
    double w;
    while (std::cin >> u >> v >> w) {
        edges.emplace_back(u, v, w);
    }

    std::sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) {
        return std::get<2>(a) < std::get<2>(b);
    });

    kruskalMST(n, edges);

    return 0;
}