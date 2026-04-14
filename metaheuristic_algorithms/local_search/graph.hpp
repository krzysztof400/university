#pragma once

#include <vector>
#include <string>
#include <utility>
#include <tuple>

struct TaskResult {
    double avg_dist;
    double avg_steps;
    int best_dist;
    std::vector<int> best_path;
    int mst_weight = 0;
};

class Graph {
public:
    Graph(const std::string& path);

    std::string getName() const;
    int getSize() const;

    TaskResult run_task_1();
    TaskResult run_task_2();
    TaskResult run_task_3();

private:
    std::string name;
    int n;
    std::vector<std::pair<double, double>> coords;
    std::vector<int> city_ids;
    
    std::vector<int> distances;
    
    // MST edges
    std::vector<std::pair<int, int>> mst;

    // Initialization
    void load(const std::string& path);
    void compute_distances();

    // Helpers
    int path_distance(const std::vector<int>& path) const;
    std::vector<int> generate_random_permutation() const;
    
    // MST methods
    void compute_mst();
    std::vector<int> compute_tsp_from_mst_starting_at(int start_idx);

    // Core Local Search algorithms
    std::tuple<int, std::vector<int>, int> local_search_invert(std::vector<int> initial_path) const;
    std::tuple<int, std::vector<int>, int> local_search_random_invert(std::vector<int> initial_path) const;
};
