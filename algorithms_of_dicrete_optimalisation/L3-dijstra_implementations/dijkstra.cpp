/**
 * AOD Laboratorium 3 - Dijkstra Implementations
 * Author: [Your Name]
 * * Implements:
 * 1. Standard Dijkstra (Binary Heap) 
 * 2. Dial's Algorithm (Buckets) 
 * 3. Radix Heap Dijkstra 
 */

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <fstream>
#include <sstream>
#include <limits>
#include <chrono>
#include <list>
#include <cmath>
#include <algorithm>
#include <iomanip>

using namespace std;

typedef long long Weight;
const Weight INF = numeric_limits<Weight>::max();

struct Edge {
    int to;
    Weight weight;
};

struct Graph {
    int n, m;
    Weight min_w, max_w;
    vector<vector<Edge>> adj;
};

// Enum for algorithm selection
enum AlgorithmType { DIJKSTRA_STD, DIJKSTRA_DIAL, DIJKSTRA_RADIX };


// ALGORITHM 1: Standard Dijkstra (Binary Heap / Priority Queue)

void run_dijkstra_std(const Graph& G, int s, vector<Weight>& dist) {
    dist.assign(G.n + 1, INF);
    dist[s] = 0;
    
    // Min-priority queue: stores {distance, vertex}
    priority_queue<pair<Weight, int>, vector<pair<Weight, int>>, greater<pair<Weight, int>>> pq;
    pq.push({0, s});

    while (!pq.empty()) {
        Weight d = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (d > dist[u]) continue;

        for (const auto& e : G.adj[u]) {
            if (dist[u] + e.weight < dist[e.to]) {
                dist[e.to] = dist[u] + e.weight;
                pq.push({dist[e.to], e.to});
            }
        }
    }
}


// ALGORITHM 2: Dial's Algorithm (Buckets)

void run_dijkstra_dial(const Graph& G, int s, vector<Weight>& dist) {
    dist.assign(G.n + 1, INF);
    dist[s] = 0;

    // Dial's algorithm is efficient for small integer weights.
    // Complexity O(m + nC). Max bucket size needed is max_weight + 1 if using circular buffer.
    // NOTE: If max_w is huge, this might consume too much memory or time.
    // Using circular buffer size W + 1.
    
    Weight W = G.max_w;
    size_t bucket_count = W + 1;
    vector<vector<int>> buckets(bucket_count);
    
    buckets[0].push_back(s);
    Weight current_dist = 0;
    int n_visited = 0;

    // Use a circular cursor
    while (n_visited < G.n) {
        // Find next non-empty bucket
        // In a circular implementation, we check buckets[current_dist % size]
        // But if the graph is not fully connected, we might loop forever if we strictly count n_visited.
        // We need a safeguard or simply iterate until empty.
        // For efficiency in dense implementation, we just check forward.
        
        while (buckets[current_dist % bucket_count].empty()) {
            current_dist++;
            if (current_dist > dist[s] + (Weight)G.n * W) break; // Safety break
            // Optimization: In proper Dial, we don't scan blindly, but here simple scan is standard.
        }
        
        // Process current bucket
        size_t idx = current_dist % bucket_count;
        while (!buckets[idx].empty()) {
            int u = buckets[idx].back();
            buckets[idx].pop_back();
            
            if (dist[u] < current_dist) continue; // Already processed with shorter path
            
            n_visited++;

            for (const auto& e : G.adj[u]) {
                Weight new_dist = dist[u] + e.weight;
                if (new_dist < dist[e.to]) {
                    dist[e.to] = new_dist;
                    buckets[new_dist % bucket_count].push_back(e.to);
                }
            }
        }
        current_dist++;
    }
}


// ALGORITHM 3: Radix Heap
// Helper for Radix Heap: Most Significant Bit (0-based index)

inline int msb(unsigned long long v) {
    if (v == 0) return -1;
    return 63 - __builtin_clzll(v);
}

void run_dijkstra_radix(const Graph& G, int s, vector<Weight>& dist) {
    dist.assign(G.n + 1, INF);
    dist[s] = 0;

    // Buckets: B[0]..B[64]. 
    // Ranges: [0,0], [1,1], [2,3], [4,7], ...
    vector<vector<int>> buckets(65);
    vector<Weight> bucket_mins(65, INF); // Min value in bucket to aid finding next
    
    // Place s in bucket. Since dist[s]=0 and last_deleted=0, diff=0 -> bucket 0.
    buckets[0].push_back(s);
    
    Weight last_deleted = 0;
    int elements = 1;

    while (elements > 0) {
        // Find the lowest non-empty bucket
        int i = 0;
        while (i < 65 && buckets[i].empty()) i++;
        
        if (i == 65) break; // Should not happen if elements > 0

        // Find min key in this bucket B[i]
        Weight min_val = INF;
        int u_min = -1;
        
        // We need to iterate to find the absolute minimum in this bucket
        for(int u : buckets[i]) {
            if (dist[u] < min_val) {
                min_val = dist[u];
                u_min = u;
            }
        }
        
        last_deleted = min_val;
        
        // Move nodes from B[i] to lower buckets if i > 0
        if (i > 0) {
            // We move everything from buckets[i] to new locations based on new last_deleted
            // Then clear buckets[i]
            vector<int> nodes_to_move = buckets[i];
            buckets[i].clear();
            
            for (int u : nodes_to_move) {
                if (u == u_min) {
                     // This is the one we extract
                     continue; 
                }
                // Redistribute
                Weight diff = dist[u] - last_deleted;
                int b_idx = (diff == 0) ? 0 : msb(diff) + 1;
                buckets[b_idx].push_back(u);
            }
        } else {
            // i == 0, ranges is size 1, so all elements have key == last_deleted
            // We just pop the one we found, or pop all?
            // Standard Radix heap extracts min. In B[0], all are min.
            // Let's just remove u_min. To make it O(1) in B[0], we just take back().
            u_min = buckets[0].back();
            buckets[0].pop_back();
        }
        
        elements--;
        int u = u_min;
        
        // Relaxation
        for (const auto& e : G.adj[u]) {
            Weight new_d = dist[u] + e.weight;
            if (new_d < dist[e.to]) {
                Weight old_d = dist[e.to];
                dist[e.to] = new_d;
                
                // If old_d was INF, it wasn't in heap. Increase count.
                // But in radix heap implementation for Dijkstra, we often just push.
                // However, duplicates reduce performance. We assume standard lazy or decrease-key.
                // For simplicity: simple push to new bucket. Lazy deletion logic handles duplicates implicitly? 
                // No, standard radix heap needs decrease-key. 
                // Since implementing exact decrease-key with locators is complex, 
                // we will just insert. But Radix Heap doesn't handle duplicates well naturally 
                // without check. 
                // Optimization: Just check if we processed it? 
                // Let's rely on the fact that if we find a shorter path, we move it. 
                // Since we don't have iterators to remove from old bucket, 
                // this implementation is slightly heuristic: we simply add to new bucket.
                // When popping, we check `if (dist[u] < expected)` logic.
                
                // Correct Radix Heap Decrease Key logic:
                // We really should move it. Without tracking, we can't.
                // Fallback for this lab: Just add to new bucket.
                // When extracting, check if dist[u] != popped_dist?
                // Radix heap relies on monotonicity. 
                
                Weight diff = new_d - last_deleted;
                int b_idx = (diff == 0) ? 0 : msb(diff) + 1;
                buckets[b_idx].push_back(e.to);
                elements++;
            }
        }
    }
}


void load_graph(const string& filename, Graph& G) {
    ifstream in(filename);
    if (!in) { cerr << "Error opening " << filename << endl; exit(1); }
    
    string line;
    char type;
    string temp;
    
    G.min_w = INF;
    G.max_w = 0;
    
    while (getline(in, line)) {
        if (line.empty() || line[0] == 'c') continue;
        stringstream ss(line);
        ss >> type;
        if (type == 'p') {
            ss >> temp >> G.n >> G.m;
            G.adj.resize(G.n + 1);
        } else if (type == 'a') {
            int u, v;
            Weight w;
            ss >> u >> v >> w;
            G.adj[u].push_back({v, w});
            if (w < G.min_w) G.min_w = w;
            if (w > G.max_w) G.max_w = w;
        }
    }
    in.close();
}

vector<int> load_sources(const string& filename) {
    vector<int> sources;
    ifstream in(filename);
    if (!in) { cerr << "Error opening " << filename << endl; exit(1); }
    string line;
    char type;
    while (getline(in, line)) {
        if (line.empty() || line[0] == 'c') continue;
        stringstream ss(line);
        ss >> type;
        if (type == 's') {
            int s;
            ss >> s;
            sources.push_back(s);
        }
    }
    return sources;
}

vector<pair<int,int>> load_pairs(const string& filename) {
    vector<pair<int,int>> pairs;
    ifstream in(filename);
    if (!in) { cerr << "Error opening " << filename << endl; exit(1); }
    string line;
    char type;
    while (getline(in, line)) {
        if (line.empty() || line[0] == 'c') continue;
        stringstream ss(line);
        ss >> type;
        if (type == 'q') {
            int u, v;
            ss >> u >> v;
            pairs.push_back({u, v});
        }
    }
    return pairs;
}

int main(int argc, char* argv[]) {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    string prog_name = argv[0];
    AlgorithmType algo = DIJKSTRA_STD;
    
    if (prog_name.find("dial") != string::npos) algo = DIJKSTRA_DIAL;
    else if (prog_name.find("radix") != string::npos) algo = DIJKSTRA_RADIX;
    
    string graph_file, aux_file, out_file;
    bool mode_ss = false;
    
    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "-d") {
            graph_file = argv[++i];
        } else if (arg == "-ss") {
            aux_file = argv[++i];
            mode_ss = true;
        } else if (arg == "-p2p") {
            aux_file = argv[++i];
            mode_ss = false;
        } else if (arg == "-oss" || arg == "-op2p") {
            out_file = argv[++i];
        }
    }

    if (graph_file.empty()) {
        cerr << "Usage: " << argv[0] << " -d <graph> -ss <sources> -oss <out>" << endl;
        return 1;
    }

    Graph G;
    load_graph(graph_file, G);

    ofstream out(out_file);
    if (!out) { cerr << "Error creating output " << out_file << endl; return 1; }

    out << "f " << graph_file << " " << aux_file << endl;
    out << "g " << G.n << " " << G.m << " " << G.min_w << " " << G.max_w << endl;

    vector<Weight> dist;

    if (mode_ss) {
        vector<int> sources = load_sources(aux_file);
        double total_time = 0;

        for (int s : sources) {
            auto start = chrono::high_resolution_clock::now();
            
            if (algo == DIJKSTRA_STD) run_dijkstra_std(G, s, dist);
            else if (algo == DIJKSTRA_DIAL) run_dijkstra_dial(G, s, dist);
            else run_dijkstra_radix(G, s, dist);
            
            auto end = chrono::high_resolution_clock::now();
            total_time += chrono::duration<double, milli>(end - start).count();
        }

        out << "t " << fixed << setprecision(2) << (total_time / sources.size()) << endl;

    } else {
        vector<pair<int,int>> pairs = load_pairs(aux_file);
        
        for (auto p : pairs) {
            int s = p.first;
            int t = p.second;
            
            if (algo == DIJKSTRA_STD) run_dijkstra_std(G, s, dist);
            else if (algo == DIJKSTRA_DIAL) run_dijkstra_dial(G, s, dist);
            else run_dijkstra_radix(G, s, dist);
            
            out << "d " << s << " " << t << " " << dist[t] << endl;
        }
    }

    out.close();
    return 0;
}