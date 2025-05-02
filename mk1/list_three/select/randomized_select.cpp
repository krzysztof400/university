#include <iostream>
#include <algorithm>
#include <cmath>
#include "utils.hpp"

bool small_array = false;

void print_if_small(int array[], int size, const char* message) {
    if (small_array) {
        std::cout << message << ": ";
        print_array(array, size);
        std::cout << std::endl;
    }
}

int partition(int array[], int low, int high) {
    int pivot = array[high]; 
    int i = low - 1;
    for (int j = low; j < high; ++j) {
        if (compare(pivot, array[j])) { 
            i++;
            swap_elements(array[i], array[j]);
        }
    }
    swap_elements(array[i + 1], array[high]);  
    return i + 1;
}

int randomized_partition(int size, int low, int high, int array[]) {
    int random_index = low + rand() % (high - low + 1);
    swap_elements(array[high], array[random_index]); 
    return partition(array, low, high);
}

int randomized_select(int size, int value, int array[]) {
    print_if_small(array, size, "Array before randomized select");
    if (size == 1) {
        return array[0];
    }
    int pivot_index = randomized_partition(size, 0, size - 1, array);
    int k = pivot_index + 1;  
    if (k == value) {
        return array[pivot_index];
    } else if (k > value) {
        return randomized_select(pivot_index, value, array);  
    } else {
        return randomized_select(size - k, value - k, array + k);  
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "Incorrect amount of arguments" << std::endl;
        std::cout << "./array_generator <selected value>" << std::endl;
        return 0;
    }
    int value = std::stoi(argv[1]);

    int size;
    std::cin >> size;
    if (size < 100) {
        small_array = true;
    }
    int array[size];
    for (int i = 0; i < size; i++) {
        std::cin >> array[i];
    }
    if (value > size) {
        std::cout << "kth element is greater than size of the array" << std::endl;
        return 0;
    }
    if (value < 1) {
        std::cout << "kth element is less than 1" << std::endl;
        return 0;
    }
    print_if_small(array, size, "Input array");
    int position = randomized_select(size, value, array);
    print_if_small(array, size, "After randomized select");

    std::sort(array, array + size);
    print_if_small(array, size, "Sorted array");

    std::cout << "The " << value << "th smallest element is: " << position << std::endl;
    if(array[value-1] != position){
        std::cout << "WARNING: The " << value << "th smallest element is not the same as the " << position << "th element in sorted array" << std::endl;
    }
    else{
        std::cout << "CORRECT: The " << value << "th smallest element is the same as the " << position << "th element in sorted array" << std::endl;
    }
    std::cout << "Comparisons: " << comparisons << std::endl;
    std::cout << "Assignments: " << assigns << std::endl;

    return 0;
}