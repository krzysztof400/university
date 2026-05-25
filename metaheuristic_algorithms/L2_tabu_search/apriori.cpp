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
#include <chrono>

// ---------------------------------------------------------------------------
// Globals / Types / Utility
// ---------------------------------------------------------------------------

static int N = 0;                           
static std::string GRAPH_NAME;
static std::vector<int> CITY_IDS;           
static std::vector<int> DIST;               

thread_local std::mt19937 tl_rng(std::random_device{}());

inline int dist(int i, int j) { return DIST[i * N + j]; }

struct Timer {
    std::chrono::time_point<std::chrono::steady_clock> start;
    Timer() : start(std::chrono::steady_clock::now()) {}
    double elapsed_ms() {
        auto end = std::chrono::steady_clock::now();
        return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    }
};

struct TaskResult {
    double avg_dist, avg_steps;
    int best_dist;
    std::vector<int> best_path;
    double total_time_ms; 
};

struct SA_Params {
    double initial_temp; 
    double alpha;          
    int epochs_per_temp;    
    double min_temp = 0.001;      
};

struct TS_Params {
    int tabu_list_size;      
    int max_iterations;    
    int max_no_improve;     
};

// ---------------------------------------------------------------------------
// Loading and Basic Heuristics
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
                GRAPH_NAME.erase(GRAPH_NAME.find_last_not_of(" \t\r\n") + 1);
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

    DIST.resize(N * N, 0);
    for (int i = 0; i < N; ++i)
        for (int j = i + 1; j < N; ++j) {
            double dx = xs[i] - xs[j], dy = ys[i] - ys[j];
            int d = (int)std::round(std::sqrt(dx*dx + dy*dy));
            DIST[i*N+j] = DIST[j*N+i] = d;
        }
}

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

static void apply_2opt_inversion(std::vector<int>& tour, int i, int j) {
    if (i > j) std::swap(i, j);
    std::reverse(tour.begin() + i + 1, tour.begin() + j + 1);
}

// ---------------------------------------------------------------------------
// A Priori Parameter Estimation
// ---------------------------------------------------------------------------

static double estimate_initial_temperature(double target_prob = 0.85, int samples = 1000) {
    std::vector<int> current_path = random_perm();
    std::uniform_int_distribution<int> rnd_idx(0, N - 1);
    
    double sum_positive_delta = 0.0;
    int positive_count = 0;

    for (int k = 0; k < samples; ++k) {
        int i = rnd_idx(tl_rng);
        int j = rnd_idx(tl_rng);
        while (i == j || std::abs(i - j) == 1 || (i == 0 && j == N - 1) || (j == 0 && i == N - 1)) {
            i = rnd_idx(tl_rng); j = rnd_idx(tl_rng);
        }
        if (i > j) std::swap(i, j);

        int t1 = current_path[i] - 1;
        int t1_next = current_path[i + 1] - 1;
        int t2 = current_path[j] - 1;
        int t2_next = current_path[(j + 1) % N] - 1;

        int delta = DIST[t1 * N + t2] + DIST[t1_next * N + t2_next]
                  - DIST[t1 * N + t1_next] - DIST[t2 * N + t2_next];

        if (delta > 0) {
            sum_positive_delta += delta;
            positive_count++;
        }
    }

    if (positive_count == 0) return 1000.0;

    double avg_delta = sum_positive_delta / positive_count;
    
    // T = - (avg_delta) / ln(P)
    return -avg_delta / std::log(target_prob);
}

// ---------------------------------------------------------------------------
// Algorithms
// ---------------------------------------------------------------------------

static std::tuple<int, std::vector<int>, int> 
simulated_annealing(std::vector<int> initial_path, SA_Params params) {
    int steps = 0;
    std::vector<int> current_path = initial_path;
    int current_cost = path_dist(current_path);
    std::vector<int> best_path = current_path;
    int best_cost = current_cost;
    double temp = params.initial_temp;
    
    std::uniform_int_distribution<int> rnd_idx(0, N - 1);
    std::uniform_real_distribution<double> rnd_prob(0.0, 1.0);

    while (temp > params.min_temp) {
        for (int epoch = 0; epoch < params.epochs_per_temp; ++epoch) {
            int i = rnd_idx(tl_rng);
            int j = rnd_idx(tl_rng);
            while (i == j || std::abs(i - j) == 1 || (i == 0 && j == N - 1) || (j == 0 && i == N - 1)) {
                i = rnd_idx(tl_rng); j = rnd_idx(tl_rng);
            }
            if (i > j) std::swap(i, j);

            int t1 = current_path[i] - 1;
            int t1_next = current_path[i + 1] - 1;
            int t2 = current_path[j] - 1;
            int t2_next = current_path[(j + 1) % N] - 1;

            int delta = DIST[t1 * N + t2] + DIST[t1_next * N + t2_next]
                      - DIST[t1 * N + t1_next] - DIST[t2 * N + t2_next];

            if (delta < 0 || std::exp(-delta / temp) > rnd_prob(tl_rng)) {
                apply_2opt_inversion(current_path, i, j);
                current_cost += delta;
                if (current_cost < best_cost) {
                    best_cost = current_cost;
                    best_path = current_path;
                }
            }
            steps++;
        }
        temp *= params.alpha; 
    }
    return {best_cost, best_path, steps};
}

static std::tuple<int, std::vector<int>, int> 
tabu_search(std::vector<int> initial_path, TS_Params params) {
    int steps = 0;
    std::vector<int> current_path = initial_path;
    int current_cost = path_dist(current_path);
    std::vector<int> best_path = current_path;
    int best_cost = current_cost;
    std::vector<std::vector<int>> tabu_matrix(N + 1, std::vector<int>(N + 1, 0));

    int no_improve = 0;
    std::uniform_int_distribution<int> rnd_idx(0, N - 1);
    int sample_size = std::min(500, N); 

    for (int iter = 1; iter <= params.max_iterations; ++iter) {
        if (no_improve >= params.max_no_improve) break;

        int iter_best_delta = std::numeric_limits<int>::max();
        int best_i = -1, best_j = -1;

        for (int k = 0; k < sample_size; ++k) {
            int i = rnd_idx(tl_rng);
            int j = rnd_idx(tl_rng);
            while (i == j || std::abs(i - j) == 1 || (i == 0 && j == N - 1) || (j == 0 && i == N - 1)) {
                i = rnd_idx(tl_rng); j = rnd_idx(tl_rng);
            }
            if (i > j) std::swap(i, j);

            int t1 = current_path[i] - 1;
            int t1_next = current_path[i + 1] - 1;
            int t2 = current_path[j] - 1;
            int t2_next = current_path[(j + 1) % N] - 1;

            int delta = DIST[t1 * N + t2] + DIST[t1_next * N + t2_next]
                      - DIST[t1 * N + t1_next] - DIST[t2 * N + t2_next];

            int city_u = current_path[i];
            int city_v = current_path[j];
            bool is_tabu = tabu_matrix[city_u][city_v] >= iter;
            bool aspiration = (current_cost + delta < best_cost);

            if (!is_tabu || aspiration) {
                if (delta < iter_best_delta) {
                    iter_best_delta = delta;
                    best_i = i;
                    best_j = j;
                }
            }
        }

        if (best_i != -1) {
            apply_2opt_inversion(current_path, best_i, best_j);
            current_cost += iter_best_delta;

            int city_u = current_path[best_i]; 
            int city_v = current_path[best_j];
            tabu_matrix[city_u][city_v] = iter + params.tabu_list_size;
            tabu_matrix[city_v][city_u] = iter + params.tabu_list_size;

            if (current_cost < best_cost) {
                best_cost = current_cost;
                best_path = current_path;
                no_improve = 0;
            } else {
                no_improve++;
            }
        } else {
            no_improve++;
        }
        steps++;
    }
    return {best_cost, best_path, steps};
}

// ---------------------------------------------------------------------------
// Parallel Runner 
// ---------------------------------------------------------------------------
static TaskResult run_parallel(
    int iterations,
    std::function<std::tuple<int,std::vector<int>,int>(int)> worker_fn)
{
    Timer total_timer;
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
             best_dist, best_path, total_timer.elapsed_ms() };
}

// ---------------------------------------------------------------------------
// Main
// ---------------------------------------------------------------------------
int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <tsplib_file>\n";
        return 1;
    }

    std::cout << "Loading " << argv[1] << " ...\n";
    load_and_build(argv[1]);
    std::cout << "Graph: " << GRAPH_NAME << " (n=" << N << ")\n\n";

    int iterations = 100;

    // ==========================================
    // A Priori Parametrization
    // ==========================================
    
    // SA
    SA_Params sa_params;
    sa_params.initial_temp = estimate_initial_temperature(0.85); // 85% chance for start
    sa_params.alpha = 0.995; 
    sa_params.epochs_per_temp = std::max(100, N); // scaling epochs with graph

    // TS
    TS_Params ts_params;
    ts_params.tabu_list_size = std::max(1, (int)std::sqrt(N)); // TL = sqrt(N)
    ts_params.max_iterations = std::max(1000, 10 * N); 
    ts_params.max_no_improve = ts_params.max_iterations / 5;   // 20% times without = stop


    std::cout << "--- Parametry A Priori ---\n";
    std::cout << "[SA] T_start = " << sa_params.initial_temp << ", alpha = " << sa_params.alpha << "\n";
    std::cout << "[TS] Tabu List = " << ts_params.tabu_list_size << ", Max Iter = " << ts_params.max_iterations << "\n\n";

    std::cout << "Starting Simulated Annealing (100 trials)...\n";
    TaskResult sa_res = run_parallel(iterations, [&](int) {
        return simulated_annealing(random_perm(), sa_params);
    });
    std::cout << "  Best dist:    " << sa_res.best_dist << "\n";
    std::cout << "  Avg dist:     " << sa_res.avg_dist << "\n";
    std::cout << "  Total time:   " << sa_res.total_time_ms << " ms\n\n";

    std::cout << "Starting Tabu Search (100 trials)...\n";
    TaskResult ts_res = run_parallel(iterations, [&](int) {
        return tabu_search(random_perm(), ts_params);
    });
    std::cout << "  Best dist:    " << ts_res.best_dist << "\n";
    std::cout << "  Avg dist:     " << ts_res.avg_dist << "\n";
    std::cout << "  Total time:   " << ts_res.total_time_ms << " ms\n";

    return 0;
}