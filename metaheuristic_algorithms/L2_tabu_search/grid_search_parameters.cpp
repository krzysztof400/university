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

// ---------------------------------------------------------------------------
// Parameters Structs
// ---------------------------------------------------------------------------

struct SA_Params {
    double initial_temp = 1000.0; 
    double alpha = 0.99;          
    int epochs_per_temp = 100;    
    double min_temp = 0.001;      
};

struct TS_Params {
    int tabu_list_size = 50;      
    int max_iterations = 1000;    
    int max_no_improve = 200;     
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
    
    // Zmniejszona responsywność paska postępu, aby nie zaśmiecać konsoli podczas Grid Search
    auto thread_worker = [&](int start, int end) {
        long long ld = 0, ls = 0;
        int lbest = std::numeric_limits<int>::max();
        std::vector<int> lpath;
        for (int i = start; i < end; ++i) {
            auto [d, p, s] = worker_fn(i);
            ld += d; ls += s;
            if (d < lbest) { lbest = d; lpath = std::move(p); }
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

    return { (double)total_dist / iterations,
             (double)total_steps / iterations,
             best_dist, best_path, total_timer.elapsed_ms() };
}

// ---------------------------------------------------------------------------
// Main - Grid Search
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

    // GRID SEARCH: SIMULATED ANNEALING
    // std::cout << "Starting Grid Search for Simulated Annealing...\n";
    
    // std::vector<double> alphas = {0.80, 0.90, 0.95, 0.99, 0.999};
    // std::vector<int> epochs = {50, 100, 200, 500};
    // std::vector<double> temps = {500.0, 1000.0, 5000.0, 10000.0, 50000.0};

    // std::ofstream sa_file(GRAPH_NAME + "sa_grid_results.csv");
    // sa_file << "graph,iterations,initial_temp,alpha,epochs_per_temp,min_temp,best_dist,avg_dist,avg_steps,time_ms\n";

    // int sa_total_configs = alphas.size() * epochs.size() * temps.size();
    // int sa_current_config = 0;

    // for (double alpha : alphas) {
    //     for (int epoch : epochs) {
    //         for (double temp : temps) {
    //             sa_current_config++;
    //             std::cout << "\r[SA] Config " << sa_current_config << "/" << sa_total_configs 
    //                       << " (alpha=" << alpha << ", epochs=" << epoch << ")" << std::flush;
                
    //             SA_Params params;
    //             params.alpha = alpha;
    //             params.epochs_per_temp = epoch;
    //             params.initial_temp = temp;
    //             params.min_temp = 0.001;

    //             TaskResult res = run_parallel(iterations, [&](int) {
    //                 return simulated_annealing(random_perm(), params);
    //             });

    //             sa_file << GRAPH_NAME << "," << iterations << "," 
    //                     << params.initial_temp << "," << params.alpha << "," 
    //                     << params.epochs_per_temp << "," << params.min_temp << ","
    //                     << res.best_dist << "," << res.avg_dist << "," 
    //                     << res.avg_steps << "," << res.total_time_ms << "\n";
    //         }
    //     }
    // }
    // sa_file.close();
    // std::cout << "\nSA Grid Search Complete. Saved to sa_grid_results.csv\n\n";


    // GRID SEARCH: TABU SEARCH
    std::cout << "Starting Grid Search for Tabu Search...\n";

    std::vector<int> tabu_sizes = {1, 10, 20, 30, 40, 50, 100, 200, 500, 1000};
    std::vector<int> max_iters = {500, 1000, 2000, 5000, 10000};
    std::vector<int> no_improves = {100, 200, 500, 1000};

    std::ofstream ts_file(GRAPH_NAME + "ts_grid_results.csv");
    ts_file << "graph,iterations,tabu_list_size,max_iterations,max_no_improve,best_dist,avg_dist,avg_steps,time_ms\n";

    int ts_total_configs = tabu_sizes.size() * max_iters.size() * no_improves.size();
    int ts_current_config = 0;

    for (int t_size : tabu_sizes) {
        for (int m_iter : max_iters) {
            for (int no_imp : no_improves) {
                // Skip illogical configurations (no_improve shouldn't be larger than max_iterations)
                if (no_imp > m_iter) continue;

                ts_current_config++;
                std::cout << "\r[TS] Config " << ts_current_config << "/" << ts_total_configs 
                          << " (size=" << t_size << ", max_it=" << m_iter << ")" << std::flush;
                
                TS_Params params;
                params.tabu_list_size = t_size;
                params.max_iterations = m_iter;
                params.max_no_improve = no_imp;

                TaskResult res = run_parallel(iterations, [&](int) {
                    return tabu_search(random_perm(), params);
                });

                ts_file << GRAPH_NAME << "," << iterations << "," 
                        << params.tabu_list_size << "," << params.max_iterations << "," 
                        << params.max_no_improve << ","
                        << res.best_dist << "," << res.avg_dist << "," 
                        << res.avg_steps << "," << res.total_time_ms << "\n";
            }
        }
    }
    ts_file.close();
    std::cout << "\nTS Grid Search Complete. Saved to ts_grid_results.csv\n";

    return 0;
}