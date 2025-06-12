#include <iostream>
#include "utils.hpp"

bool small_array = false;

template <typename T>
void print_if_small(T* array, int size, const std::string& msg) {
    if (small_array) {
        std::cout << msg << ": [ ";
        print_array(array, size);
        std::cout << "]" << std::endl;
    }
}

template <typename T>
T find_pivot(T* array, int low, int high) {
    int size = high - low + 1;
    if (size <= 5) {
        T* temp = new T[size];
        for (int i = 0; i < size; ++i) {
            temp[i] = array[low + i];
        }
        insertion_sort(temp, size);
        T median = temp[size / 2];
        delete[] temp;
        return median;
    }
    int group_count = (size + 4) / 5;
    T* medians = new T[group_count];
    for (int i = 0; i < group_count; ++i) {
        int start = low + i * 5;
        int end = start + 5;
        if (end > high + 1) end = high + 1;
        int len = end - start;
        T* temp = new T[len];
        for (int j = 0; j < len; ++j) {
            temp[j] = array[start + j];
        }
        insertion_sort(temp, len);
        medians[i] = temp[len / 2];
        delete[] temp;
    }
    T pivot = find_pivot(medians, 0, group_count - 1);
    delete[] medians;
    return pivot;
}

template <typename T>
int partition(T* array, int low, int high) {
    T pivot;
    assign(pivot, array[high]);
    int i = low - 1;
    print_if_small(array, high - low + 1, "Before partition");
    for (int j = low; j < high; ++j) {
        if (compare(pivot, array[j])) { // pivot > array[j]
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
        T pivot = find_pivot(array, low, high);
        int k = low;
        while (k <= high && array[k] != pivot) {
            ++k;
        }
        swap_elements(array[k], array[high]);
        int pivot_index = partition(array, low, high);
        quick_sort(array, low, pivot_index - 1);
        quick_sort(array, pivot_index + 1, high);
    }
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

    quick_sort(array, 0, n - 1);

    print_if_small(original_array, n, "Original array");
    print_if_small(array, n, "Sorted array");
    std::cout << "Is sorted: " << (is_sorted(array, n) ? "Yes" : "No") << std::endl;

    std::cout << "Number of comparisons: " << comparisons << std::endl;
    std::cout << "Number of swaps: " << assigns << std::endl;

    delete[] array;
    delete[] original_array; // Poprawka: zwolnij pamięć
    return 0;
}