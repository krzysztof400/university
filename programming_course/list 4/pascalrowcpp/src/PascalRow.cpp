#include <iostream>
#include <vector>

std::vector<int> generateRow(int rowNum) {
    std::vector<int> row(rowNum + 1, 1);
    for (int j = 1; j < rowNum; j++) {
        row[j] = row[j - 1] * (rowNum - j + 1) / j;
    }
    return row;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <numRows>" << std::endl;
        return 1;
    }

    int numRows = std::stoi(argv[1]);
    if (numRows < 0) {
        std::cerr << "Number of rows must be non-negative." << std::endl;
        return 1;
    }

    for (int i = 0; i < numRows; i++) {
        std::vector<int> row = generateRow(i);
        for (int num : row) {
            std::cout << num << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}