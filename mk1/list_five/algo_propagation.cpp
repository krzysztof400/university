#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <sstream>
#include <string> // Added for std::getline

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

void optimalPropagation(int n, const std::vector<std::vector<int>>& tree, int root) {
    if (root < 0 || root >= n) {
        std::cerr << "Error: Root index is out of bounds." << std::endl;
        return;
    }
    std::vector<int> depth(n, -1);
    std::queue<int> q;

    assign(depth[root], 0);
    q.push(root);

    // BFS to calculate depth of each node
    while (!q.empty()) {
        int node = q.front();
        q.pop();

        for (int child : tree[node]) {
            if (depth[child] == -1) { // Fix: Correct depth check
                assign(depth[child], depth[node] + 1);
                q.push(child);
            }
        }
    }

    // Sort nodes by depth in descending order
    std::vector<int> nodes(n);
    for (int i = 0; i < n; ++i) assign(nodes[i], i);
    std::sort(nodes.begin(), nodes.end(), [&](int a, int b) {
        return depth[a] > depth[b]; // Fix: Simplified comparison
    });

    // Calculate the number of rounds
    int rounds = 0;
    std::vector<bool> informed(n, false);
    informed[root] = true;
    assignments++;

    for (int node : nodes) {
        if (!informed[node]) continue; // Only propagate from informed nodes
        bool propagated_in_this_round = false;
        for (int child : tree[node]) {
            if (!informed[child]) {
                informed[child] = true;
                assignments++;
                propagated_in_this_round = true;
            }
        }
        if (propagated_in_this_round) { // Increment rounds only if new nodes were informed
            rounds++;
        }
    }


    std::cout << "Optimal Propagation Rounds: " << rounds << std::endl;
    std::cout << "Assignments: " << assignments << ", Comparisons: " << comparisons << std::endl;

    // Output the tree structure for reuse
    std::cout << "Tree:" << std::endl;
    for (int i = 0; i < n; ++i) {
        for (int child : tree[i]) {
            // Ensure each edge is printed only once (e.g., 0-1, not also 1-0)
            if (i < child) {
                std::cout << i << " " << child << std::endl;
            }
        }
    }
}

void processInput(std::stringstream& input) {
    std::string line;
    int n = 0, root = -1; // Initialize n and root

    // Skip "MST (Prim):" line
    std::getline(input, line);
    if (!input.good() || line.find("MST (Prim):") == std::string::npos) {
        std::cerr << "Error: Expected 'MST (Prim):' line not found." << std::endl;
        return;
    }

    // Read root and n
    input >> root >> n;
    std::getline(input, line); // Consume the rest of the line after reading root and n

    // Debugging: Print the input values
    std::cerr << "Debug: Number of nodes (n) = " << n << ", Root index = " << root << std::endl;

    // Validate the input
    if (n <= 0) {
        std::cerr << "Error: Number of nodes must be greater than 0." << std::endl;
        return;
    }
    if (root < 0 || root >= n) {
        std::cerr << "Error: Root index is out of bounds. Valid range is [0, " << n - 1 << "]." << std::endl;
        return;
    }

    std::vector<std::vector<int>> tree(n);
    int u, v;
    char dash, colon; // To consume '-' and ':'
    double weight; // To consume weight

    while (std::getline(input, line)) {
        std::stringstream lineStream(line);
        if (lineStream >> u >> dash >> v >> colon >> weight) {
            if (u < 0 || u >= n || v < 0 || v >= n) {
                std::cerr << "Error: Edge contains invalid node index. Valid range is [0, " << n - 1 << "]." << std::endl;
                return;
            }
            tree[u].push_back(v);
            tree[v].push_back(u);
        } else if (line.find("Total Weight:") != std::string::npos ||
                   line.find("Assignments:") != std::string::npos ||
                   line.find("Comparisons:") != std::string::npos) {
            // Skip these lines
            continue;
        } else {
            // If it's not an edge or a recognized skip line, it might be the end of useful input
            // or an unexpected format. Break or handle as error.
            break;
        }
    }

    optimalPropagation(n, tree, root);
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        // Default mode: read input from stdin
        std::stringstream input;
        std::string line;
        while (std::getline(std::cin, line)) {
            input << line << "\n";
        }
        processInput(input);
    } else if (argc == 2 && std::string(argv[1]) == "--process") {
        std::string line;
        std::getline(std::cin, line); 
        std::getline(std::cin, line); 
        std::getline(std::cin, line); 

        std::stringstream treeInput;
        std::stringstream fullInputForProcess;
        while (std::getline(std::cin, line)) {
            fullInputForProcess << line << "\n";
        }
        processInput(fullInputForProcess);


    } else {
        std::cerr << "Usage: " << argv[0] << " [--process]" << std::endl;
        return 1;
    }

    return 0;
}