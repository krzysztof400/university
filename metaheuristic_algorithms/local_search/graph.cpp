#include <algorithm>
#include <atomic>
#include <cassert>
#include <cmath>
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <mutex>
#include <queue>
#include <random>
#include <sstream>
#include <stack>
#include <string>
#include <thread>
#include <tuple>
#include <vector>

// ---------------------------------------------------------------------------
// Globals / types
// ---------------------------------------------------------------------------

static int N = 0;                           // number of cities
static std::string GRAPH_NAME;
static std::vector<int> CITY_IDS;           // 1-based city ids
static std::vector<int> DIST;               // flat n×n distance matrix

thread_local std::mt19937 tl_rng(std::random_device{}());

inline int dist(int i, int j) { return DIST[i * N + j]; }

// ---------------------------------------------------------------------------
// Load TSPLIB file
// ---------------------------------------------------------------------------

static void load_and_build(const std::string& path) {
    std::vector<double> xs, ys;
    std::ifstream file(path);
    std::string line;
    bool reading = false;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        if (line.find("NAME") == 0) {
            auto p = line.find(':');
            if (p != std::string::npos) {
                GRAPH_NAME = line.substr(p + 1);
                GRAPH_NAME.erase(0, GRAPH_NAME.find_first_not_of(" \t\r\n"));
            }
        } else if (line.find("NODE_COORD_SECTION") != std::string::npos) {
            reading = true;
        } else if (line.find("EOF") != std::string::npos) {
            break;
        } else if (reading) {
            std::istringstream iss(line);
            int id; double x, y;
            if (iss >> id >> x >> y) {
                CITY_IDS.push_back(id);
                xs.push_back(x);
                ys.push_back(y);
            }
        }
    }
    N = (int)CITY_IDS.size();

    // Distance matrix
    DIST.resize(N * N, 0);
    for (int i = 0; i < N; ++i)
        for (int j = i + 1; j < N; ++j) {
            double dx = xs[i] - xs[j], dy = ys[i] - ys[j];
            int d = (int)std::round(std::sqrt(dx*dx + dy*dy));
            DIST[i*N+j] = DIST[j*N+i] = d;
        }
}

// ---------------------------------------------------------------------------
// Path utilities
// ---------------------------------------------------------------------------
static int path_dist(const std::vector<int>& path) {
    int total = 0;
    for (int i = 0; i < N - 1; ++i)
        total += DIST[(path[i]-1)*N + (path[i+1]-1)];
    total += DIST[(path.back()-1)*N + (path[0]-1)];
    return total;
}

static std::vector<int> random_perm() {
    std::vector<int> p = CITY_IDS;
    std::shuffle(p.begin(), p.end(), tl_rng);
    return p;
}

// ---------------------------------------------------------------------------
// classical 2-opt with full neightbourhood list 
// Returns {distance, improved_path, steps}
// ---------------------------------------------------------------------------
static std::tuple<int, std::vector<int>, int>
local_search_2opt(std::vector<int> path) {
    // conversion to 0-based indexes
    std::vector<int> tour(N);
    for (int i = 0; i < N; ++i) {
        tour[i] = path[i] - 1;
    }

    int cur_dist = path_dist(path);
    int steps = 0;
    bool improved = true;

    while (improved) {
        improved = false;
        
        int best_delta = 0;
        int best_i = -1;
        int best_j = -1;

        // we look through entire invert neighbourhood - N^2
        for (int i = 0; i < N - 1; ++i) {
            for (int j = i + 1; j < N; ++j) {
                // skipping reversing of path
                if (i == 0 && j == N - 1) continue;

                int t1 = tour[i];
                int t1_next = tour[i + 1];
                int t2 = tour[j];
                int t2_next = tour[(j + 1) % N];

                // calculate len of path
                int delta = DIST[t1 * N + t2] + DIST[t1_next * N + t2_next]
                          - DIST[t1 * N + t1_next] - DIST[t2 * N + t2_next];

                if (delta < best_delta) {
                    best_delta = delta;
                    best_i = i;
                    best_j = j;
                }
            }
        }

        if (best_delta < 0) {
            std::reverse(tour.begin() + best_i + 1, tour.begin() + best_j + 1);
            cur_dist += best_delta;
            ++steps;
            improved = true;
        }
    }

    std::vector<int> result(N);
    for (int i = 0; i < N; ++i) result[i] = CITY_IDS[tour[i]];
    
    return {cur_dist, result, steps};
}

// ---------------------------------------------------------------------------
// 2-opt with RANDOM candidate sampling (task 2)
// Sample n random pairs; keep best improving move
// ---------------------------------------------------------------------------
static std::tuple<int,std::vector<int>,int>
local_search_2opt_random(std::vector<int> path) {
    std::vector<int> tour(N);
    std::vector<int> pos(N);
    for (int i = 0; i < N; ++i) {
        tour[i] = path[i] - 1;
        pos[path[i] - 1] = i;
    }

    int cur_dist = path_dist(path);
    int steps = 0;
    std::uniform_int_distribution<int> rnd(0, N - 1);

    while (true) {
        int best_delta = 0, best_lo = -1, best_hi = -1;

        for (int k = 0; k < N; ++k) {
            int pi = rnd(tl_rng), pj = rnd(tl_rng);
            if (pi == pj) continue;
            if (pi > pj) std::swap(pi, pj);
            if (pi == 0 && pj == N - 1) continue;

            int A = tour[pi], B = tour[(pi+1)%N];
            int C = tour[pj], D = tour[(pj+1)%N];

            int delta = DIST[A*N+C] + DIST[B*N+D] - DIST[A*N+B] - DIST[C*N+D];
            if (delta < best_delta) {
                best_delta = delta;
                best_lo = pi; best_hi = pj;
            }
        }

        if (best_delta < 0) {
            std::reverse(tour.begin() + best_lo + 1, tour.begin() + best_hi + 1);
            for (int x = best_lo + 1; x <= best_hi; ++x)
                pos[tour[x]] = x;
            cur_dist += best_delta;
            ++steps;
        } else break;
    }

    std::vector<int> result(N);
    for (int i = 0; i < N; ++i) result[i] = CITY_IDS[tour[i]];
    return {cur_dist, result, steps};
}

// ---------------------------------------------------------------------------
// MST (Prim, O(n² ) dense — fine since we have dense distance matrix)
// Returns list of edges as (u,v) 0-based indices
// ---------------------------------------------------------------------------
static std::vector<std::pair<int,int>> build_mst() {
    using PII = std::pair<int,int>;
    std::priority_queue<PII, std::vector<PII>, std::greater<PII>> pq;
    std::vector<int>  key(N, std::numeric_limits<int>::max());
    std::vector<int>  parent(N, -1);
    std::vector<bool> in_tree(N, false);

    key[0] = 0;
    pq.push({0, 0});

    std::vector<std::pair<int,int>> mst;
    mst.reserve(N - 1);

    while (!pq.empty()) {
        auto [w, u] = pq.top(); pq.pop();
        if (in_tree[u]) continue;
        in_tree[u] = true;
        if (parent[u] != -1)
            mst.push_back({parent[u], u});
        for (int v = 0; v < N; ++v) {
            if (!in_tree[v] && DIST[u*N+v] < key[v]) {
                key[v] = DIST[u*N+v];
                parent[v] = u;
                pq.push({key[v], v});
            }
        }
    }
    return mst;
}

// DFS traversal of MST to build initial TSP tour
static std::vector<int> mst_tour(const std::vector<std::pair<int,int>>& mst, int start) {
    std::vector<std::vector<int>> adj(N);
    for (auto& [u, v] : mst) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    std::vector<bool> visited(N, false);
    std::vector<int> path;
    path.reserve(N);
    std::stack<int> stack;
    stack.push(start);
    while (!stack.empty()) {
        int u = stack.top(); stack.pop();
        if (visited[u]) continue;
        visited[u] = true;
        path.push_back(CITY_IDS[u]);
        std::vector<int>& nb = adj[u];
        std::sort(nb.rbegin(), nb.rend());
        for (int v : nb)
            if (!visited[v]) stack.push(v);
    }
    return path;
}

struct TaskResult {
    double avg_dist, avg_steps;
    int best_dist;
    std::vector<int> best_path;
    int extra; // mst_weight for task 3
};

static TaskResult run_parallel(
    int iterations,
    std::function<std::tuple<int,std::vector<int>,int>(int)> worker_fn)
{
    int num_threads = (int)std::thread::hardware_concurrency();
    std::vector<std::thread> threads;
    std::mutex mtx;

    long long total_dist = 0, total_steps = 0;
    int best_dist = std::numeric_limits<int>::max();
    std::vector<int> best_path;
    std::atomic<int> completed{0};
    int ten_pct = std::max(1, iterations / 10);

    auto thread_worker = [&](int start, int end) {
        long long ld = 0, ls = 0;
        int lbest = std::numeric_limits<int>::max();
        std::vector<int> lpath;
        for (int i = start; i < end; ++i) {
            auto [d, p, s] = worker_fn(i);
            ld += d; ls += s;
            if (d < lbest) { lbest = d; lpath = std::move(p); }
            int c = ++completed;
            if (c % ten_pct == 0 || c == iterations) {
                std::lock_guard<std::mutex> lk(mtx);
                std::cout << "      " << (c*100/iterations) << "% ("
                          << c << "/" << iterations << ")\r" << std::flush;
            }
        }
        std::lock_guard<std::mutex> lk(mtx);
        total_dist += ld; total_steps += ls;
        if (lbest < best_dist) { best_dist = lbest; best_path = std::move(lpath); }
    };

    int chunk = std::max(1, iterations / num_threads);
    for (int t = 0; t < num_threads; ++t) {
        int s = t * chunk;
        int e = (t == num_threads - 1) ? iterations : s + chunk;
        if (s >= iterations) break;
        threads.emplace_back(thread_worker, s, e);
    }
    for (auto& t : threads) t.join();
    std::cout << "\n";

    return { (double)total_dist / iterations,
             (double)total_steps / iterations,
             best_dist, best_path, 0 };
}

// ---------------------------------------------------------------------------
// Task runners
// ---------------------------------------------------------------------------
static TaskResult task1() {
    int iters = std::min(N, 10);
    return run_parallel(iters, [](int) {
        return local_search_2opt(random_perm());
    });
}

static TaskResult task2() {
    int iters = std::min(N, 10);
    return run_parallel(iters, [](int) {
        return local_search_2opt_random(random_perm());
    });
}

static TaskResult task3() {
    auto mst = build_mst();

    int mst_weight = 0;
    for (auto& [u, v] : mst) mst_weight += DIST[u*N+v];

    int iters = (int)std::ceil(std::sqrt((double)N));

    std::uniform_int_distribution<int> rnd_start(0, N - 1);

    auto result = run_parallel(iters, [&](int) {
        int s = rnd_start(tl_rng);
        auto init = mst_tour(mst, s);
        return local_search_2opt(init);
    });
    result.extra = mst_weight;
    return result;
}

// ---------------------------------------------------------------------------
// Main
// ---------------------------------------------------------------------------
static void print_result(const char* task_name, const TaskResult& r, bool show_mst) {
    std::cout << "  [" << task_name << "]\n";
    if (show_mst)
        std::cout << "    MST weight:   " << r.extra << "\n";
    std::cout << "    Avg distance: " << (long long)r.avg_dist << "\n";
    std::cout << "    Avg steps:    " << (long long)r.avg_steps << "\n";
    std::cout << "    Best dist:    " << r.best_dist << "\n";
    std::cout << "    Best path:";
    for (int c : r.best_path) std::cout << " " << c;
    std::cout << "\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <tsplib_file> [tasks: 1 2 3]\n";
        return 1;
    }

    std::cout << "Loading " << argv[1] << " ...\n";
    load_and_build(argv[1]);
    std::cout << "Graph: " << GRAPH_NAME << " (n=" << N << ")\n";

    // Determine which tasks to run (default: all)
    bool run_t1 = false, run_t2 = false, run_t3 = false;
    if (argc == 2) { run_t1 = run_t2 = run_t3 = true; }
    for (int i = 2; i < argc; ++i) {
        std::string a = argv[i];
        if (a == "1") run_t1 = true;
        if (a == "2") run_t2 = true;
        if (a == "3") run_t3 = true;
    }

    if (run_t1) { std::cout << "  Task 1 (full 2-opt, n starts)...\n"; print_result("Task 1", task1(), false); }
    if (run_t2) { std::cout << "  Task 2 (random-sample 2-opt, n starts)...\n"; print_result("Task 2", task2(), false); }
    if (run_t3) { std::cout << "  Task 3 (MST-seeded 2-opt, sqrt(n) starts)...\n"; print_result("Task 3", task3(), true); }

    return 0;
}