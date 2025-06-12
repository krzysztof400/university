#include <iostream>
#include <vector>
#include <algorithm> // Still needed for std::stoi
#include <iomanip>
#include <string>

// Global counters for assignments and comparisons
long long assignments = 0;
long long comparisons = 0;

// Custom assignment function to track assignments
template <typename T>
void assign(T &a, T b) {
    a = b;
    assignments++;
}

// Custom swap function using assign to track assignments
template <typename T>
void swap(T &a, T &b) {
    T temp;
    assign(temp, a);
    assign(a, b);
    assign(b, temp);
}

// Structure for an edge in Kruskal's algorithm
struct KruskalEdge {
    int u, v;
    double weight;

    // Overload the less than operator for sorting
    // This is used by mergeSort internally
    bool operator<(const KruskalEdge& other) const {
        comparisons++; // Increment comparison counter for each comparison
        return weight < other.weight;
    }
};

// Disjoint Set Union (DSU) structure for union-find operations
struct DSU {
    std::vector<int> parent; // Stores the parent of each element
    std::vector<int> rank;   // Stores the rank of each set (for union by rank)

    // Constructor: initializes each element as its own parent and rank to 0
    DSU(int n) {
        parent.resize(n + 1);
        rank.resize(n + 1);
        for (int i = 1; i <= n; ++i) {
            assign(parent[i], i); // Each element is initially its own parent
            assign(rank[i], 0);   // Initial rank is 0
            comparisons++; // For loop condition check
        }
        comparisons++; // Final loop condition check
    }

    // Find operation with path compression
    int find(int i) {
        comparisons++; // For if condition check (parent[i] == i)
        if (parent[i] == i) {
            return i; // If i is the root, return i
        }
        // Path compression: set parent[i] to the root of its set
        int root;
        assign(root, find(parent[i]));
        assign(parent[i], root);
        return parent[i];
    }

    // Unite operation by rank
    bool unite(int i, int j) {
        int root_i, root_j;
        assign(root_i, find(i)); // Find root of set containing i
        assign(root_j, find(j)); // Find root of set containing j

        comparisons++; // For if condition check (root_i != root_j)
        if (root_i != root_j) { // If they are in different sets
            comparisons++; // For if condition check (rank[root_i] < rank[root_j])
            if (rank[root_i] < rank[root_j]) {
                swap(root_i, root_j); // Ensure root_i has greater or equal rank
            }
            assign(parent[root_j], root_i); // Make root_i the parent of root_j
            comparisons++; // For if condition check (rank[root_i] == rank[root_j])
            if (rank[root_i] == rank[root_j]) {
                assign(rank[root_i], rank[root_i] + 1); // Increment rank if ranks were equal
            }
            return true; // Union successful
        }
        return false; // Already in the same set
    }
};

// --- START: Merge Sort Implementation ---
// These functions are designed to be included directly in the same file
// as main or linked as a separate compilation unit.

template <typename T>
void merge(std::vector<T>& arr, int left, int mid, int right) {
    int n1;
    assign(n1, mid - left + 1);
    int n2;
    assign(n2, right - mid);

    std::vector<T> L(n1);
    std::vector<T> R(n2);

    for (int i = 0; i < n1; ++i) {
        assign(L[i], arr[left + i]);
        comparisons++; // For loop condition check
    }
    comparisons++; // Final loop condition check

    for (int j = 0; j < n2; ++j) {
        assign(R[j], arr[mid + 1 + j]);
        comparisons++; // For loop condition check
    }
    comparisons++; // Final loop condition check

    int i = 0;
    int j = 0;
    int k;
    assign(k, left);

    while (i < n1 && j < n2) {
        comparisons += 2; // For while loop condition check
        comparisons++; // For if condition check (L[i] < R[j])
        if (L[i] < R[j]) {
            assign(arr[k], L[i]);
            i++;
        } else {
            assign(arr[k], R[j]);
            j++;
        }
        k++;
    }
    comparisons += 2; // Final while loop condition check

    while (i < n1) {
        comparisons++; // For while loop condition check
        assign(arr[k], L[i]);
        i++;
        k++;
    }
    comparisons++; // Final while loop condition check

    while (j < n2) {
        comparisons++; // For while loop condition check
        assign(arr[k], R[j]);
        j++;
        k++;
    }
    comparisons++; // Final while loop condition check
}

template <typename T>
void mergeSort(std::vector<T>& arr, int left, int right) {
    comparisons++; // For if condition check (left < right)
    if (left < right) {
        int mid;
        assign(mid, left + (right - left) / 2);

        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);

        merge(arr, left, mid, right);
    }
}

// Wrapper function to call mergeSort for a vector
template <typename T>
void sortWithMergeSort(std::vector<T>& arr) {
    if (!arr.empty()) {
        mergeSort(arr, 0, arr.size() - 1);
    }
    comparisons++; // For if condition check
}

// --- END: Merge Sort Implementation ---

int main(int argc, char* argv[]) {
    // Check for correct number of command line arguments
    comparisons++; // For if condition check
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <number_of_vertices>" << std::endl;
        return 1;
    }

    int n;
    // Attempt to convert the argument to an integer
    try {
        n = std::stoi(argv[1]);
    } catch (const std::invalid_argument& e) {
        std::cerr << "Invalid number of vertices: " << e.what() << std::endl;
        return 1;
    } catch (const std::out_of_range& e) {
        std::cerr << "Number of vertices out of range: " << e.what() << std::endl;
        return 1;
    }

    // Validate the number of vertices
    comparisons++; // For if condition check
    if (n <= 0) {
        std::cerr << "Number of vertices must be positive." << std::endl;
        return 1;
    }

    std::vector<KruskalEdge> edges; // Vector to store graph edges
    int u_input, v_input;
    double weight_input;

    // Read edges from standard input until EOF
    while (std::cin >> u_input >> v_input >> weight_input) {
        comparisons++; // For while loop condition check
        // Validate vertex indices
        comparisons += 4; // For all conditions in the if statement
        if (u_input < 1 || u_input > n || v_input < 1 || v_input > n) {
            std::cerr << "Invalid vertex indices. Vertices must be between 1 and " << n << std::endl;
            return 1;
        }
        edges.push_back({u_input, v_input, weight_input});
    }
    comparisons++; // Final while loop condition check

    // Sort the edges by weight using our custom Merge Sort
    sortWithMergeSort(edges);

    DSU dsu(n); // Initialize Disjoint Set Union structure

    std::vector<KruskalEdge> mst_edges; // Vector to store edges of the MST
    double total_mst_weight = 0.0; // Total weight of the MST

    // Iterate through sorted edges to build the MST
    for (const auto& edge : edges) {
        comparisons++; // For loop condition check
        comparisons++; // For if condition check (dsu.unite(edge.u, edge.v))
        if (dsu.unite(edge.u, edge.v)) { // If uniting the vertices forms no cycle
            mst_edges.push_back(edge); // Add edge to MST
            total_mst_weight += edge.weight; // Add weight to total MST weight

            comparisons++; // For if condition check (mst_edges.size() == n - 1)
            // If we have n-1 edges, we have found the MST
            if (mst_edges.size() == static_cast<std::vector<KruskalEdge>::size_type>(n - 1)) {
                break;
            }
        }
    }
    comparisons++; // Final loop condition check

    // Set precision for printing double values
    std::cout << std::fixed << std::setprecision(6);

    // Print the edges in the MST
    for (const auto& edge : mst_edges) {
        comparisons++; // For loop condition check
        std::cout << edge.u << " -- " << edge.v << " (Weight: " << edge.weight << ")" << std::endl;
    }
    comparisons++; // Final loop condition check

    // Print total MST weight and counters
    std::cout << "Total MST Weight: " << total_mst_weight << std::endl;
    std::cout << "Assignments: " << assignments << std::endl;
    std::cout << "Comparisons: " << comparisons << std::endl;

    return 0;
}
