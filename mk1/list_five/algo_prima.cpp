#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <iomanip>
#include <string>

long long assignments = 0;
long long comparisons = 0;

template <typename T>
void assign(T &a, T b) {
    a = b;
    assignments++;
}

template <typename T>
void swap(T &a, T &b) {
    T temp;
    assign(temp, a);
    assign(a, b);
    assign(b, temp);
}

struct Edge {
    int to;
    double weight;

    bool operator>(const Edge& other) const {
        comparisons++;
        return weight > other.weight;
    }
};

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <number_of_vertices>" << std::endl;
        return 1;
    }

    int n;
    try {
        n = std::stoi(argv[1]);
    } catch (...) {
        std::cerr << "Error: Invalid number for vertices: " << argv[1] << std::endl;
        return 1;
    }

    if (n <= 0) {
        std::cerr << "Error: Number of vertices must be positive." << std::endl;
        return 1;
    }

    std::vector<std::vector<Edge>> adj(n + 1);

    int u_input, v_input;
    double weight_input;

    while (std::cin >> u_input >> v_input >> weight_input) {
        if (u_input < 1 || u_input > n || v_input < 1 || v_input > n) {
            std::cerr << "Error: Input edge (" << u_input << ", " << v_input << ") has vertex outside expected range [1, " << n << "]." << std::endl;
            return 1;
        }
        adj[u_input].push_back({v_input, weight_input});
        adj[v_input].push_back({u_input, weight_input});
    }

    std::vector<double> min_cost(n + 1);
    std::vector<int> parent(n + 1);
    std::vector<bool> in_mst(n + 1);

    for (int i = 1; i <= n; ++i) {
        assign(min_cost[i], std::numeric_limits<double>::max());
        assign(parent[i], -1);
        in_mst[i] = false;
        comparisons++;
    }
    comparisons++;

    std::priority_queue<Edge, std::vector<Edge>, std::greater<Edge>> pq;

    int start_node = 1;
    assign(min_cost[start_node], 0.0);
    pq.push({start_node, 0.0});

    double total_mst_weight = 0.0;
    int vertices_processed = 0;

    while (!pq.empty() && vertices_processed < n) {
        comparisons++;
        comparisons++;

        Edge current_edge = pq.top();
        pq.pop();

        int u_node = current_edge.to;
        double cost_to_u = current_edge.weight;

        comparisons++;
        if (cost_to_u > min_cost[u_node]) {
            continue;
        }

        comparisons++;
        if (in_mst[u_node]) {
            continue;
        }

        in_mst[u_node] = true;
        total_mst_weight += cost_to_u;
        vertices_processed++;

        for (const auto& neighbor_edge : adj[u_node]) {
            comparisons++;
            int v_node = neighbor_edge.to;
            double weight_uv = neighbor_edge.weight;

            comparisons++;
            comparisons++;
            if (!in_mst[v_node] && weight_uv < min_cost[v_node]) {
                assign(min_cost[v_node], weight_uv);
                assign(parent[v_node], u_node);
                pq.push({v_node, weight_uv});
            }
        }
    }
    comparisons++;
    comparisons++;

    std::cout << std::fixed << std::setprecision(6);

    std::cout << "Minimum Spanning Tree Edges:" << std::endl;
    for (int i = 1; i <= n; ++i) {
        comparisons++;
        if (parent[i] != -1) {
            comparisons++;
            std::cout << parent[i] << " -- " << i << " (Weight: " << min_cost[i] << ")" << std::endl;
        }
    }
    comparisons++;

    std::cout << "Total MST Weight: " << total_mst_weight << std::endl;
    std::cout << "Assignments: " << assignments << std::endl;
    std::cout << "Comparisons: " << comparisons << std::endl;

    return 0;
}
