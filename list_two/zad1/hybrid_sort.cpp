#include <iostream>
#include "utils.hpp"

bool small_array = false;

template <typename T>
void swap_elements(T& a, T& b) {
    T temp;
    assign(temp, a);
    assign(a, b);
    assign(b, temp);
}

template <typename T>
void print_if_small(T* array, int size, const std::string& msg) {
    if (small_array) {
        std::cout << msg << ": [ ";
        print_array(array, size);
        std::cout << "]" << std::endl;
    }
}
template <typename T>
int partition(T* array, int low, int high) {
    T pivot;
    assign(pivot, array[high]);
    int i = low - 1;
    print_if_small(array, high - low + 1, "Before partition");
    for (int j = low; j < high; ++j) {
        if (compare(pivot, array[j])) {
            i++;
            swap_elements(array[i], array[j]);
        }
    }

    swap_elements(array[i + 1], array[high]);
    print_if_small(array, high - low + 1, "After partition");
    return i + 1;
}

template <typename T>
void quick_sort(T* array, int low, int high) {
    if (low < high) {
        int pivot_index = partition(array, low, high);
        quick_sort(array, low, pivot_index - 1);
        quick_sort(array, pivot_index + 1, high);
    }
}

template <typename T>
T* insertion_sort(T* array, int size) {
    if (size <= 1) {
        T* result = new T[size];
        if (size == 1) {
            assign(result[0], array[0]);
        }
        return result;
    }

    T* result = new T[size];
    for (int i = 0; i < size; i++) {
        assign(result[i], array[i]);
    }
    
    for (int i = 1; i < size; i++) {
        T key;
        assign(key, result[i]);
        int j = i - 1;
        while (j >= 0 && compare(result[j], key)) {
            assign(result[j + 1], result[j]);
            j--;
        }
        
        assign(result[j + 1], key);
        print_if_small(result, size, "After inserting " + std::to_string(key));
    }
    
    return result;
}

int main() {
    int n;
    std::cin >> n;

    auto* array = new int[n];
    for (int i = 0; i < n; i++) {
        std::cin >> array[i];
    }
    auto* original_array = new int[n];
    for (int i = 0; i < n; i++) {
        original_array[i] = array[i];
    }

    small_array = (n <= 40);
    print_if_small(original_array, n, "Original array");

    if (n<5){
        array = insertion_sort(array, n);
    }
    else{
        quick_sort(array, 0, n - 1);
    }

    print_if_small(original_array, n, "Original array");
    print_if_small(array, n, "Sorted array");
    std::cout << "Is sorted: " << (is_sorted(array, n) ? "Yes" : "No") << std::endl;

    std::cout << "Number of comparisons: " << comparisons << std::endl;
    std::cout << "Number of swaps: " << assigns << std::endl;

    delete[] array;
    return 0;
}
