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
#include <execution>
#include <numeric>

// ---------------------------------------------------------------------------
// Globals / Types / Utility
// ---------------------------------------------------------------------------

static int N = 0;                           
static std::string GRAPH_NAME;
static std::vector<int> CITY_IDS;           
static std::vector<int> DIST;               

inline int dist(int i, int j) { return DIST[i * N + j]; }

enum class CrossoverMethod {
    OX1,
    PMX
};

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

inline std::mt19937& get_local_rng() {
    thread_local std::mt19937 tl_rng(std::random_device{}());
    return tl_rng;
}

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
    std::shuffle(p.begin(), p.end(), get_local_rng());
    return p;
}

// ---------------------------------------------------------------------------
// Genetic Operators
// ---------------------------------------------------------------------------

static std::vector<int> crossover_ox(const std::vector<int>& p1, const std::vector<int>& p2) {
    auto& rng = get_local_rng();
    std::vector<int> child(N, -1);
    
    int start = std::uniform_int_distribution<>(0, N - 1)(rng);
    int end = std::uniform_int_distribution<>(0, N - 1)(rng);
    if (start > end) std::swap(start, end);

    std::vector<bool> in_child(N + 1, false);
    for (int i = start; i <= end; ++i) {
        child[i] = p1[i];
        in_child[p1[i]] = true;
    }

    int p2_idx = (end + 1) % N;
    int child_idx = (end + 1) % N;
    for (int i = 0; i < N; ++i) {
        int gene = p2[p2_idx];
        if (!in_child[gene]) {
            child[child_idx] = gene;
            child_idx = (child_idx + 1) % N;
        }
        p2_idx = (p2_idx + 1) % N;
    }
    return child;
}

static std::vector<int> crossover_pmx(const std::vector<int>& p1, const std::vector<int>& p2) {
    auto& rng = get_local_rng();
    std::vector<int> child(N, -1);
    
    int start = std::uniform_int_distribution<>(0, N - 1)(rng);
    int end = std::uniform_int_distribution<>(0, N - 1)(rng);
    if (start > end) std::swap(start, end);

    std::vector<bool> in_child(N + 1, false);
    for (int i = start; i <= end; ++i) {
        child[i] = p1[i];
        in_child[p1[i]] = true;
    }

    for (int i = 0; i < N; ++i) {
        if (i >= start && i <= end) continue;
        
        int val = p2[i];
        // Rozwiązywanie konfliktów mapowania PMX
        while (in_child[val]) {
            auto it = std::find(p1.begin() + start, p1.begin() + end + 1, val);
            int idx = std::distance(p1.begin(), it);
            val = p2[idx];
        }
        child[i] = val;
    }
    return child;
}

// ---------------------------------------------------------------------------
// Algorithm Core
// ---------------------------------------------------------------------------
static std::tuple<int, std::vector<int>, int> genetic_algorithm(bool withIsland, CrossoverMethod cx_method) {
    int max_epochs = 100;
    int popSize = std::min(N * 10, 200); 
    
    int num_islands = withIsland ? 4 : 1;
    popSize = (popSize / num_islands) * num_islands; 
    int island_size = popSize / num_islands;
    int migration_interval = 10;

    std::vector<std::vector<int>> population(popSize);
    std::vector<int> fitness(popSize);
    std::vector<int> indices(popSize);
    std::iota(indices.begin(), indices.end(), 0);

    for(int i = 0; i < popSize; i++) {
        population[i] = random_perm();
    }

    auto evaluate = [&](const std::vector<std::vector<int>>& pop, std::vector<int>& fit) {
        std::for_each(std::execution::par_unseq, indices.begin(), indices.end(), [&](int i) {
            fit[i] = path_dist(pop[i]);
        });
    };

    auto selection_tournament = [&](const std::vector<int>& fit, int island_id, int k = 3) {
        auto& rng = get_local_rng();
        int start_idx = island_id * island_size;
        int end_idx = start_idx + island_size - 1;
        
        int best_idx = std::uniform_int_distribution<>(start_idx, end_idx)(rng);
        for(int i = 1; i < k; ++i) {
            int idx = std::uniform_int_distribution<>(start_idx, end_idx)(rng);
            if(fit[idx] < fit[best_idx]) best_idx = idx;
        }
        return best_idx;
    };

    auto do_crossover = [&](std::vector<std::vector<int>>& next_pop) {
        std::for_each(std::execution::par_unseq, indices.begin(), indices.end(), [&](int i) {
            int island_id = i / island_size; // Określenie przynależności do wyspy
            int p1_idx = selection_tournament(fitness, island_id);
            int p2_idx = selection_tournament(fitness, island_id);
            
            if (cx_method == CrossoverMethod::OX1) {
                next_pop[i] = crossover_ox(population[p1_idx], population[p2_idx]);
            } else {
                next_pop[i] = crossover_pmx(population[p1_idx], population[p2_idx]);
            }
            
            auto& rng = get_local_rng();
            if (std::uniform_real_distribution<>(0.0, 1.0)(rng) < 0.10) {
                int u = std::uniform_int_distribution<>(0, N - 1)(rng);
                int v = std::uniform_int_distribution<>(0, N - 1)(rng);
                std::swap(next_pop[i][u], next_pop[i][v]);
            }
        });
    };

    auto memetic_local_search = [&](std::vector<std::vector<int>>& pop) {
        std::for_each(std::execution::par_unseq, indices.begin(), indices.end(), [&](int i) {
            bool improved = true;
            auto& path = pop[i];
            while (improved) {
                improved = false;
                for (int u = 0; u < N - 1; ++u) {
                    for (int v = u + 2; v < N; ++v) {
                        if (u == 0 && v == N - 1) continue;
                        int n1 = path[u]-1, n2 = path[u+1]-1;
                        int n3 = path[v]-1, n4 = path[(v+1)%N]-1;
                        
                        if (DIST[n1*N + n3] + DIST[n2*N + n4] < DIST[n1*N + n2] + DIST[n3*N + n4]) {
                            std::reverse(path.begin() + u + 1, path.begin() + v + 1);
                            improved = true;
                        }
                    }
                }
            }
        });
    };

    auto update_population = [&](std::vector<std::vector<int>>& next_pop, std::vector<int>& next_fit) {
        for (int k = 0; k < num_islands; ++k) {
            int start_idx = k * island_size;
            int end_idx = start_idx + island_size;
            
            auto min_it = std::min_element(std::execution::par_unseq, fitness.begin() + start_idx, fitness.begin() + end_idx);
            int best_old_idx = std::distance(fitness.begin(), min_it);
            
            auto worst_new_it = std::max_element(std::execution::par_unseq, next_fit.begin() + start_idx, next_fit.begin() + end_idx);
            int worst_new_idx = std::distance(next_fit.begin(), worst_new_it);
            
            next_pop[worst_new_idx] = population[best_old_idx];
            next_fit[worst_new_idx] = fitness[best_old_idx];
        }
        population = std::move(next_pop);
        fitness = std::move(next_fit);
    };

    evaluate(population, fitness);
    int overall_best = std::numeric_limits<int>::max();
    std::vector<int> overall_best_path;

    for (int epoch = 0; epoch < max_epochs; ++epoch) {
        std::vector<std::vector<int>> next_population(popSize);
        std::vector<int> next_fitness(popSize);

        do_crossover(next_population);
        memetic_local_search(next_population);
        evaluate(next_population, next_fitness);
        update_population(next_population, next_fitness);

        if (withIsland && epoch > 0 && epoch % migration_interval == 0) {
            for (int k = 0; k < num_islands; ++k) {
                int current_start = k * island_size;
                int next_island_start = ((k + 1) % num_islands) * island_size;
                
                auto best_it = std::min_element(fitness.begin() + current_start, fitness.begin() + current_start + island_size);
                int best_idx = std::distance(fitness.begin(), best_it);
                
                auto worst_it = std::max_element(fitness.begin() + next_island_start, fitness.begin() + next_island_start + island_size);
                int worst_idx = std::distance(fitness.begin(), worst_it);
                
                population[worst_idx] = population[best_idx];
                fitness[worst_idx] = fitness[best_idx];
            }
        }

        int current_best_idx = std::distance(fitness.begin(), std::min_element(fitness.begin(), fitness.end()));
        if (fitness[current_best_idx] < overall_best) {
            overall_best = fitness[current_best_idx];
            overall_best_path = population[current_best_idx];
        }
    }

    return {overall_best, overall_best_path, max_epochs};
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
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " <tsplib_file> 1/0 1/0\n";
        return 1;
    }

    std::cout << "Loading " << argv[1] << " ...\n";
    load_and_build(argv[1]);
    std::cout << "Graph: " << GRAPH_NAME << " (n=" << N << ")\n\n";

    int iterations = 10;
    bool use_island_model;
    CrossoverMethod method;
    if (*argv[2] == '1'){
        use_island_model = true;
    }
    if (*argv[3] == '1'){
        method = CrossoverMethod::PMX;
    }
    else {
        method = CrossoverMethod::OX1;
    }

    std::cout << "[GA] Metoda: Memetic Algorithm (Lamarckian)\n";
    std::cout << "[GA] Wyspy: " << (use_island_model ? "Tak (Migracja co 10 epok)" : "Nie") << "\n";
    std::cout << "[GA] Crossover: " << (method == CrossoverMethod::OX1 ? "OX1" : "PMX") << "\n";
    std::cout << "[GA] Local Search: 2-opt\n\n";

    std::cout << "Genetic algorithm (" << iterations << " trials)...\n";
    
    TaskResult ga_res = run_parallel(iterations, [&](int) {
        return genetic_algorithm(use_island_model, method);
    });
    
    std::cout << "  Best dist:    " << ga_res.best_dist << "\n";
    std::cout << "  Avg dist:     " << ga_res.avg_dist << "\n";
    std::cout << "  Total time:   " << ga_res.total_time_ms << " ms\n\n";

    return 0;
}