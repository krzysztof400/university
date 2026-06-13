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
    std::vector<int> best_len_in_epoch;
    double total_time_ms; 
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
// Algorithm
// ---------------------------------------------------------------------------
static TaskResult genetic_algorithm(bool withIslands = true, std::function<std::vector<int>>(std::vector<int>) crossover_method) {
    // static evaluate(){}
    // static crossover(){}
    // static update_population(){}
    // static memetic(){}
    int popSize = N * 10;
    vector<vector<int>(N)>(popSize) population;
    vector<int>(N) fitness;
    for(int i = 0; i<N; i++) {
        population[i] = random_perm();
    }
    while() {
        for(int i=0; i<N; ++i) {
            fitness[i] = path_dist(population[i]);
        }



    }
    // select
    // crossover
    // have paraleism based on islands
    // update population
    // memetic development

    return //best in population
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

    int iterations = 10;

    std::cout << "--- Parametry A Priori ---\n";
    std::cout << "[SA] T_start = " << sa_params.initial_temp << ", alpha = " << sa_params.alpha << "\n";
    std::cout << "[TS] Tabu List = " << ts_params.tabu_list_size << ", Max Iter = " << ts_params.max_iterations << "\n\n";

    std::cout << "Genetic algorithm(10 trials)...\n";
    TaskResult sa_res = run_parallel(iterations, [&](int) {
        return genetic_algorithm();
    });
    std::cout << "  Best dist:    " << sa_res.best_dist << "\n";
    std::cout << "  Avg dist:     " << sa_res.avg_dist << "\n";
    std::cout << "  Total time:   " << sa_res.total_time_ms << " ms\n\n";

    return 0;
}