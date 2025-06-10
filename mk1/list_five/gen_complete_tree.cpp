#include <iostream>
#include <vector>
#include <random>
#include <iomanip>
#include <chrono>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <number_of_vertices>" << std::endl;
        return 1;
    }

    int n;
    try {
        n = std::stoi(argv[1]);
    } catch (const std::invalid_argument& ia) {
        std::cerr << "Invalid number: " << argv[1] << std::endl;
        return 1;
    } catch (const std::out_of_range& oor) {
        std::cerr << "Number out of range: " << argv[1] << std::endl;
        return 1;
    }

    if (n <= 0) {
        std::cerr << "Number of vertices must be positive." << std::endl;
        return 1;
    }

    // Initialize random number generator
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 generator(seed);
    std::uniform_real_distribution<double> distribution(0.0, 1.0);

    std::cout << std::fixed << std::setprecision(6); // Set precision for output

    for (int i = 1; i <= n; ++i) {
        for (int j = i + 1; j <= n; ++j) {
            double weight = distribution(generator);
            while (weight == 0.0 || weight == 1.0) {
                weight = distribution(generator);
            }
            std::cout << i << " " << j << " " << weight << std::endl;
        }
    }

    return 0;
}