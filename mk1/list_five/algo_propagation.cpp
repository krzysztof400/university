#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <sstream>
#include <string>
#include <vector> // Included for std::pair

// Global counters for assignments and comparisons
int assignments = 0;
int comparisons = 0;

// Wrapper function to count assignment operations
void assign(int &a, int b) {
    a = b;
    assignments++;
}

// Wrapper function to count comparison operations (Note: not used in the corrected algorithm)
bool compare(int a, int b) {
    comparisons++;
    return a > b;
}

/**
 * @brief Calculates the optimal propagation rounds in a tree from a root node.
 * The number of rounds is the maximum depth found via BFS.
 * @param n The total number of nodes in the tree.
 * @param tree The adjacency list representation of the tree.
 * @param root The 0-indexed root node for the propagation.
 */
void optimalPropagation(int n, const std::vector<std::vector<int>>& tree, int root) {
    if (root < 0 || root >= n) {
        std::cerr << "Error: Root index is out of bounds." << std::endl;
        return;
    }
    std::vector<int> depth(n, -1);
    std::queue<int> q;

    assign(depth[root], 0);
    q.push(root);

    int max_depth = 0;

    // BFS to calculate the depth of each node from the root
    while (!q.empty()) {
        int node = q.front();
        q.pop();

        // The number of rounds is the depth of the furthest node
        if (depth[node] > max_depth) {
            max_depth = depth[node];
        }

        for (int child : tree[node]) {
            if (depth[child] == -1) { // If node has not been visited
                assign(depth[child], depth[node] + 1);
                q.push(child);
            }
        }
    }

    std::cout << "Optimal Propagation Rounds: " << max_depth << std::endl;
    std::cout << "Assignments: " << assignments << ", Comparisons: " << comparisons << std::endl;

    // Output the tree structure for verification, converting back to 1-based indexing for clarity
    std::cout << "Tree:" << std::endl;
    for (int i = 0; i < n; ++i) {
        for (int child : tree[i]) {
            if (i < child) { // Print each edge only once
                std::cout << i + 1 << " -- " << child + 1 << std::endl;
            }
        }
    }
}

/**
 * @brief Processes the input stream to build the tree and run the propagation algorithm.
 * @param input A stringstream containing the MST data.
 */
void processInput(std::stringstream& input) {
    std::string line;
    std::getline(input, line); // Skip the header line "Minimum Spanning Tree Edges:"

    int max_node = 0;
    std::vector<std::pair<int, int>> edges;

    // First pass: read all edges and determine the number of nodes (n)
    while (std::getline(input, line)) {
        // Stop parsing when summary lines are reached
        if (line.find("Total MST Weight:") != std::string::npos) {
            break;
        }

        std::stringstream lineStream(line);
        int u, v;
        std::string dash; // To consume the "--" separator

        // Parse the format "u -- v ..."
        if (lineStream >> u >> dash >> v) {
            edges.push_back({u, v});
            if (u > max_node) max_node = u;
            if (v > max_node) max_node = v;
        }
    }

    int n = max_node;
    if (n <= 0) {
        std::cerr << "Error: No valid edges found in the input." << std::endl;
        return;
    }

    // Assume root is node 1 if not specified. Convert to 0-based index.
    int root = 0;

    // Build the adjacency list from the collected edges
    std::vector<std::vector<int>> tree(n);
    for (const auto& edge : edges) {
        // Convert 1-based input to 0-based index for the vector
        int u = edge.first - 1;
        int v = edge.second - 1;
        tree[u].push_back(v);
        tree[v].push_back(u);
    }

    optimalPropagation(n, tree, root);
}

int main() {
    // Read the entire standard input into a stringstream for processing
    std::stringstream input_buffer;
    input_buffer << std::cin.rdbuf();
    processInput(input_buffer);

    return 0;
}