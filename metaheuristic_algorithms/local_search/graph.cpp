#include "graph.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <random>
#include <thread>
#include <mutex>
#include <stack>
#include <atomic>
#include <limits>
#include <queue>

// Thread-local RNG for lock-free parallel random numbers
thread_local std::mt19937 tl_rng(std::random_device{}());

Graph::Graph(const std::string& path) {
    load(path);
    compute_distances();
}

std::string Graph::getName() const { return name; }
int Graph::getSize() const { return n; }

void Graph::load(const std::string& path) {
    std::ifstream file(path);
    std::string line;
    bool reading_nodes = false;

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        if (line.find("NAME") == 0) {
            auto colon_pos = line.find(':');
            if (colon_pos != std::string::npos) {
                name = line.substr(colon_pos + 1);
                name.erase(0, name.find_first_not_of(" \t\r\n"));
            }
        } else if (line.find("NODE_COORD_SECTION") != std::string::npos) {
            reading_nodes = true;
        } else if (line.find("EOF") != std::string::npos) {
            break;
        } else if (reading_nodes) {
            std::istringstream iss(line);
            int id;
            double x, y;
            if (iss >> id >> x >> y) {
                city_ids.push_back(id);
                coords.push_back({x, y});
            }
        }
    }
    n = city_ids.size();
}

void Graph::compute_distances() {
    distances.resize(n * n, 0);
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            double dx = coords[i].first  - coords[j].first;
            double dy = coords[i].second - coords[j].second;
            int d = static_cast<int>(std::round(std::sqrt(dx * dx + dy * dy)));
            distances[i * n + j] = d;
            distances[j * n + i] = d;
        }
    }
}

int Graph::path_distance(const std::vector<int>& path) const {
    int total = 0;
    for (size_t i = 0; i < path.size() - 1; ++i)
        total += distances[(path[i] - 1) * n + (path[i + 1] - 1)];
    total += distances[(path.back() - 1) * n + (path[0] - 1)];
    return total;
}

std::vector<int> Graph::generate_random_permutation() const {
    std::vector<int> perm = city_ids;
    std::shuffle(perm.begin(), perm.end(), tl_rng);
    return perm;
}

// ------------------------------------------------------------------
// Core Local Search Algorithms
// ------------------------------------------------------------------

std::tuple<int, std::vector<int>, int>
Graph::local_search_invert(std::vector<int> initial_path) const {
    std::vector<int> current_path = std::move(initial_path);
    int current_dist = path_distance(current_path);
    int steps = 0;

    while (true) {
        int best_delta = 0;
        int best_i = -1, best_j = -1;

        for (int i = 0; i < n - 1; ++i) {
            // FIX: use proper modular prev, consistent with cyclic tour semantics
            int A_idx = current_path[(i - 1 + n) % n] - 1;
            int B_idx = current_path[i] - 1;
            int dist_AB = distances[A_idx * n + B_idx];

            for (int j = i + 1; j < n; ++j) {
                // Skip reversing the entire cycle (no-op in cyclic tour)
                if (i == 0 && j == n - 1) continue;

                int C_idx = current_path[j] - 1;
                int D_idx = current_path[(j + 1) % n] - 1;

                int dist_CD = distances[C_idx * n + D_idx];
                int dist_AC = distances[A_idx * n + C_idx];
                int dist_BD = distances[B_idx * n + D_idx];

                int delta = (dist_AC + dist_BD) - (dist_AB + dist_CD);
                if (delta < best_delta) {
                    best_delta = delta;
                    best_i = i;
                    best_j = j;
                }
            }
        }

        if (best_delta < 0) {
            std::reverse(current_path.begin() + best_i,
                         current_path.begin() + best_j + 1);
            current_dist += best_delta;
            ++steps;
        } else {
            break;
        }
    }

    return {current_dist, current_path, steps};
}

std::tuple<int, std::vector<int>, int>
Graph::local_search_random_invert(std::vector<int> initial_path) const {
    std::vector<int> current_path = std::move(initial_path);
    int current_dist = path_distance(current_path);
    int steps = 0;
    std::uniform_int_distribution<int> dist_idx(0, n - 1);

    while (true) {
        int best_delta = 0;
        int best_i = -1, best_j = -1;

        for (int k = 0; k < n; ++k) {
            int i = dist_idx(tl_rng);
            int j = dist_idx(tl_rng);
            if (i == j) continue;
            if (i > j) std::swap(i, j);
            if (i == 0 && j == n - 1) continue;

            // FIX: use modular prev, same as full version
            int A_idx = current_path[(i - 1 + n) % n] - 1;
            int B_idx = current_path[i] - 1;
            int C_idx = current_path[j] - 1;
            int D_idx = current_path[(j + 1) % n] - 1;

            int delta = (distances[A_idx * n + C_idx] + distances[B_idx * n + D_idx])
                      - (distances[A_idx * n + B_idx] + distances[C_idx * n + D_idx]);

            if (delta < best_delta) {
                best_delta = delta;
                best_i = i;
                best_j = j;
            }
        }

        if (best_delta < 0) {
            std::reverse(current_path.begin() + best_i,
                         current_path.begin() + best_j + 1);
            current_dist += best_delta;
            ++steps;
        } else {
            break;
        }
    }

    return {current_dist, current_path, steps};
}

// ------------------------------------------------------------------
// MST Implementation (Task 3)  — FIX: O(n log n) with priority queue
// ------------------------------------------------------------------

void Graph::compute_mst() {
    if (!mst.empty()) return;

    // FIX: use a priority queue instead of O(n) linear scan per vertex.
    // This changes overall complexity from O(n²) to O(n log n), which
    // matters for Canada (n=4663), Tanzania (n=6117), Egypt (n=7146),
    // Ireland (n=8246), Oman (n=10000).
    using PII = std::pair<int, int>;  // (weight, vertex)
    std::priority_queue<PII, std::vector<PII>, std::greater<PII>> pq;

    std::vector<int>  parent(n, -1);
    std::vector<int>  key(n, std::numeric_limits<int>::max()); // FIX: INT_MAX not 1e9
    std::vector<bool> in_tree(n, false);

    key[0] = 0;
    pq.push({0, 0});

    while (!pq.empty()) {
        auto [w, u] = pq.top(); pq.pop();
        if (in_tree[u]) continue;
        in_tree[u] = true;

        if (parent[u] != -1)
            mst.push_back({parent[u], u});

        for (int v = 0; v < n; ++v) {
            if (!in_tree[v] && distances[u * n + v] < key[v]) {
                key[v]    = distances[u * n + v];
                parent[v] = u;
                pq.push({key[v], v});
            }
        }
    }
}

std::vector<int> Graph::compute_tsp_from_mst_starting_at(int start_idx) {
    // NOTE: compute_mst() must be called before spawning threads (already done
    // in run_task_3). Do NOT call it lazily here — that would be a data race.
    std::vector<std::vector<int>> adj(n);
    for (const auto& edge : mst) {
        adj[edge.first].push_back(edge.second);
        adj[edge.second].push_back(edge.first);
    }

    std::vector<bool> visited(n, false);
    std::vector<int>  path;
    path.reserve(n);
    std::stack<int> stack;

    stack.push(start_idx);
    while (!stack.empty()) {
        int u = stack.top(); stack.pop();
        if (visited[u]) continue;
        visited[u] = true;
        path.push_back(city_ids[u]);

        std::vector<int> neighbors = adj[u];
        std::sort(neighbors.rbegin(), neighbors.rend());
        for (int v : neighbors)
            if (!visited[v]) stack.push(v);
    }
    return path;
}

// ------------------------------------------------------------------
// Task Wrappers with Thread Chunking & Progress Tracking
// ------------------------------------------------------------------

TaskResult Graph::run_task_1() {
    int num_threads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;
    std::mutex mtx;

    long long total_dist = 0, total_steps = 0;
    // FIX: INT_MAX instead of 1e9 cast to int (overflows on large instances)
    int best_dist = std::numeric_limits<int>::max();
    std::vector<int> best_path;

    std::atomic<int> completed{0};
    int ten_percent = std::max(1, n / 10);

    auto worker = [&](int start, int end) {
        long long local_dist = 0, local_steps = 0;
        int local_best_dist = std::numeric_limits<int>::max(); // FIX
        std::vector<int> local_best_path;

        for (int i = start; i < end; ++i) {
            auto init_path = generate_random_permutation();
            auto [dist, path, steps] = local_search_invert(init_path);

            local_dist  += dist;
            local_steps += steps;
            if (dist < local_best_dist) {
                local_best_dist = dist;
                local_best_path = path;
            }

            int c = ++completed;
            if (c % ten_percent == 0 || c == n) {
                std::lock_guard<std::mutex> lock(mtx);
                std::cout << "      Progress: " << (c * 100 / n)
                          << "% (" << c << "/" << n << ")\r" << std::flush;
            }
        }

        std::lock_guard<std::mutex> lock(mtx);
        total_dist  += local_dist;
        total_steps += local_steps;
        if (local_best_dist < best_dist) {
            best_dist = local_best_dist;
            best_path = local_best_path;
        }
    };

    int chunk = n / num_threads;
    for (int i = 0; i < num_threads; ++i) {
        int start = i * chunk;
        int end   = (i == num_threads - 1) ? n : (i + 1) * chunk;
        threads.emplace_back(worker, start, end);
    }
    for (auto& t : threads) t.join();

    std::cout << "\n";
    return {(double)total_dist / n, (double)total_steps / n, best_dist, best_path, 0};
}

TaskResult Graph::run_task_2() {
    int num_threads = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;
    std::mutex mtx;

    long long total_dist = 0, total_steps = 0;
    int best_dist = std::numeric_limits<int>::max(); // FIX
    std::vector<int> best_path;

    std::atomic<int> completed{0};
    int ten_percent = std::max(1, n / 10);

    auto worker = [&](int start, int end) {
        long long local_dist = 0, local_steps = 0;
        int local_best_dist = std::numeric_limits<int>::max(); // FIX
        std::vector<int> local_best_path;

        for (int i = start; i < end; ++i) {
            auto init_path = generate_random_permutation();
            auto [dist, path, steps] = local_search_random_invert(init_path);

            local_dist  += dist;
            local_steps += steps;
            if (dist < local_best_dist) {
                local_best_dist = dist;
                local_best_path = path;
            }

            int c = ++completed;
            if (c % ten_percent == 0 || c == n) {
                std::lock_guard<std::mutex> lock(mtx);
                std::cout << "      Progress: " << (c * 100 / n)
                          << "% (" << c << "/" << n << ")\r" << std::flush;
            }
        }

        std::lock_guard<std::mutex> lock(mtx);
        total_dist  += local_dist;
        total_steps += local_steps;
        if (local_best_dist < best_dist) {
            best_dist = local_best_dist;
            best_path = local_best_path;
        }
    };

    int chunk = n / num_threads;
    for (int i = 0; i < num_threads; ++i) {
        int start = i * chunk;
        int end   = (i == num_threads - 1) ? n : (i + 1) * chunk;
        threads.emplace_back(worker, start, end);
    }
    for (auto& t : threads) t.join();

    std::cout << "\n";
    return {(double)total_dist / n, (double)total_steps / n, best_dist, best_path, 0};
}

TaskResult Graph::run_task_3() {
    // FIX: build MST on the main thread BEFORE spawning workers to avoid
    // the data race on the lazy `if (mst.empty())` check inside the threads.
    compute_mst();

    int mst_weight = 0;
    for (const auto& edge : mst)
        mst_weight += distances[edge.first * n + edge.second];

    int iterations = static_cast<int>(std::ceil(std::sqrt(n)));

    int num_threads = std::min((int)std::thread::hardware_concurrency(), iterations);
    std::vector<std::thread> threads;
    std::mutex mtx;

    long long total_dist = 0, total_steps = 0;
    int best_dist = std::numeric_limits<int>::max(); // FIX
    std::vector<int> best_path;

    std::atomic<int> completed{0};
    int ten_percent = std::max(1, iterations / 10);

    auto worker = [&](int start, int end) {
        long long local_dist = 0, local_steps = 0;
        int local_best_dist = std::numeric_limits<int>::max(); // FIX
        std::vector<int> local_best_path;
        std::uniform_int_distribution<int> dist_start(0, n - 1);

        for (int i = start; i < end; ++i) {
            int start_node = dist_start(tl_rng);
            // Safe: mst is fully built and read-only from here on
            auto init_path = compute_tsp_from_mst_starting_at(start_node);
            auto [dist, path, steps] = local_search_invert(init_path);

            local_dist  += dist;
            local_steps += steps;
            if (dist < local_best_dist) {
                local_best_dist = dist;
                local_best_path = path;
            }

            int c = ++completed;
            if (c % ten_percent == 0 || c == iterations) {
                std::lock_guard<std::mutex> lock(mtx);
                std::cout << "      Progress: " << (c * 100 / iterations)
                          << "% (" << c << "/" << iterations << ")\r" << std::flush;
            }
        }

        std::lock_guard<std::mutex> lock(mtx);
        total_dist  += local_dist;
        total_steps += local_steps;
        if (local_best_dist < best_dist) {
            best_dist = local_best_dist;
            best_path = local_best_path;
        }
    };

    int chunk = std::max(1, iterations / num_threads);
    for (int i = 0; i < num_threads; ++i) {
        int start = i * chunk;
        int end   = (i == num_threads - 1) ? iterations : (i + 1) * chunk;
        threads.emplace_back(worker, start, end);
    }
    for (auto& t : threads) t.join();

    std::cout << "\n";
    return {(double)total_dist / iterations, (double)total_steps / iterations,
            best_dist, best_path, mst_weight};
}