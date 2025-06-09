#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <limits>

int assignments = 0;
int comparisons = 0;

void assign(double &a, double b) {
    a = b;
    assignments++;
}

void assign(int &a, int b) {
    a = b;
    assignments++;
}

bool compare(int a, int b) {
    comparisons++;
    return a > b;
}

void primMST(int n, const std::vector<std::vector<std::pair<int, double>>>& graph) {
    std::vector<bool> inMST(n, false);
    std::vector<int> parent(n, -1);
    std::vector<double> key(n, std::numeric_limits<double>::max());
    double totalWeight = 0;

    
    auto cmp = [](const std::pair<int, double>& a, const std::pair<int, double>& b) {
        return a.second > b.second;
    };
    std::priority_queue<std::pair<int, double>, std::vector<std::pair<int, double>>, decltype(cmp)> pq(cmp);

    assign(key[0], 0.0);
    pq.push({0, 0.0});
    
    while (!pq.empty()) {
        int u = pq.top().first;
        pq.pop();
        
        if (inMST[u]) continue;
        inMST[u] = true;
        assignments++;
        
        for (const auto& [v, weight] : graph[u]) {
            if (!inMST[v] && compare(weight, key[v])) {
                assign(key[v], weight);
                assign(parent[v], u);
                pq.push({v, key[v]});
            }
        }
    }
    
    std::cout << "MST (Prim):" << std::endl;
    std::cout << 0 << " "<< n << std::endl;
    for (int i = 1; i < n; ++i) {
        if (parent[i] != -1) {
            std::cout << parent[i] << " - " << i << " : " << key[i] << std::endl;
            totalWeight += key[i];
        }
    }
    std::cout << "Total Weight: " << totalWeight << std::endl;
    std::cout << "Assignments: " << assignments << ", Comparisons: " << comparisons << std::endl;
}


int main() {
    int n;
    std::cin >> n;

    std::vector<std::vector<std::pair<int, double>>> graph(n);
    int u, v;
    double w;
    while (std::cin >> u >> v >> w) {
        graph[u].emplace_back(v, w);
        graph[v].emplace_back(u, w);
    }

    primMST(n, graph);

    return 0;
}