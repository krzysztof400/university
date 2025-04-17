#include <vector>
#include <iostream>

int comparisons = 0;
int swaps = 0;
bool small_array;

bool compare(int a, int b) {
    comparisons++;
    return a < b;
}

bool compare_equal(int a, int b) {
    comparisons++;
    return a <= b;
}

void assign(int &a, int &b) {
    swaps++;
    a = b;
}

void print_array(std::vector<int> array) {
    for (int num : array) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
}

std::vector<int> quick_sort(int n, std::vector<int> array) {
    if (compare_equal(n, 1)) {
        return array;
    }
    int pivot;
    assign(pivot, array[n/2]);

    std::vector<int> right_array, left_array, equal_array;

    for (int num : array) {
        if (compare(num, pivot)) {
            left_array.push_back(num);
        } else if (compare(pivot, num)) {
            right_array.push_back(num);
        } else {
            equal_array.push_back(num);
        }
    }

    left_array = quick_sort(left_array.size(), left_array);
    right_array = quick_sort(right_array.size(), right_array);

    if(small_array){
        std::cout << "Array:" << std::endl;
        print_array(array);

        std::cout << "Left side of array:" << std::endl;
        print_array(left_array);

        std::cout << "Pivot:" << std::endl;
        print_array(equal_array);

        std::cout << "Right side array:" << std::endl;
        print_array(right_array);
        
        std::cout << std::endl;
        std::cout << std::endl;    
    }

    left_array.insert(left_array.end(), equal_array.begin(), equal_array.end());
    left_array.insert(left_array.end(), right_array.begin(), right_array.end());
    swaps += 2;

    return left_array;
}

bool is_sorted(std::vector<int> array) {
    for (int i = 1; i < array.size(); i++) {
        if (array[i] < array[i-1]) {
            return false;
        }
    }
    return true;
}

int main() {
    int n;
    std::cin >> n;
    std::vector<int> array(n);
    for (int i = 0; i < n; i++) {
        std::cin >> array[i];
    }

    if (n < 40){
        small_array = true;
    }
    else {
        small_array = false;
    }

    std::vector<int> sorted_array = quick_sort(n, array);

    if (small_array) {
        std::cout << "Original array: " << std::endl;
        print_array(array);
        std::cout << "Sorted array: " << std::endl;
        print_array(sorted_array);
    }

    std::cout << "Number of comparisons: " << comparisons << std::endl;
    std::cout << "Number of swaps: " << swaps << std::endl;

    if(is_sorted(sorted_array)) {
        std::cout << "Array is sorted" << std::endl;
    } else {
        std::cout << "Array is not sorted" << std::endl;
    }

    return 0;
}