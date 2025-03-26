#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <n>\n";
        return 1;
    }

    int n = std::stoi(argv[1]);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 2 * n - 1);

    std::vector<int> nums(n);
    for (int i = 0; i < n; i++) {
        nums[i] = dist(gen);
    }

    // Sort the numbers in descending order
    std::sort(nums.begin(), nums.end(), std::greater<int>());

    std::cout << n << std::endl;
    for (int i = 0; i < n; i++) {
        std::cout << nums[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}
