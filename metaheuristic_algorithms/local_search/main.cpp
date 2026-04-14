#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <iomanip>
#include "graph.hpp"

int main() {
    // Make sure these match the paths in your environment
    std::vector<std::string> datasets = {
        "data/di38.tsp",
        "data/mu1979.tsp",
        "data/ca4663.tsp"
        "data/tz6177.tsp",
        "data/eg7146.tsp",
        "data/ei8246.tsp",
    };

    for (const auto& filename : datasets) {
        if (!std::filesystem::exists(filename)) {
            std::cout << "Warning: " << filename << " not found. Skipping...\n";
            continue;
        }

        std::cout << "==================================================\n";
        std::cout << "Processing: " << filename << "\n";
        std::cout << "==================================================\n";

        Graph g(filename);
        std::cout << "Graph loaded: " << g.getName() << " (" << g.getSize() << " nodes)\n";

        // ------------------------------------------------
        // Task 1: Full Local Search
        // ------------------------------------------------
        std::cout << "\n--- Task 1: Local Search (Full Neighborhood) ---\n";
        TaskResult res1 = g.run_task_1();
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "  Average solution value: " << res1.avg_dist << "\n";
        std::cout << "  Average steps to opt:   " << res1.avg_steps << "\n";
        std::cout << "  Best solution value:    " << res1.best_dist << "\n";

        // ------------------------------------------------
        // Task 2: Fast Local Search
        // ------------------------------------------------
        std::cout << "\n--- Task 2: Local Search (n Random Neighbors) ---\n";
        TaskResult res2 = g.run_task_2();
        std::cout << "  Average solution value: " << res2.avg_dist << "\n";
        std::cout << "  Average steps to opt:   " << res2.avg_steps << "\n";
        std::cout << "  Best solution value:    " << res2.best_dist << "\n";

        // ------------------------------------------------
        // Task 3: MST Cycle Start (3rd Year Variant)
        // ------------------------------------------------
        std::cout << "\n--- Task 3: MST Initialization + Local Search ---\n";
        TaskResult res3 = g.run_task_3();
        std::cout << "  MST minimum weight:     " << res3.mst_weight << "\n";
        std::cout << "  Average solution value: " << res3.avg_dist << "\n";
        std::cout << "  Average steps to opt:   " << res3.avg_steps << "\n";
        std::cout << "  Best solution value:    " << res3.best_dist << "\n\n";
    }

    return 0;
}
