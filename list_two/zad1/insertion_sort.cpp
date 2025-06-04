<<<<<<< .working
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
    auto * org_array = new int[n];
    for (int i = 0; i < n; i++) {
        org_array[i] = array[i];
    }
    if(n>40){
        small_array = false;
    }
    else{
        small_array = true;
    }
    print_if_small(org_array, n, "Original array");

    int* sorted_array = insertion_sort(array, n);


    print_if_small(org_array, n, "Original array");
    print_if_small(array, n, "Sorted array");
    if (is_sorted(sorted_array, n)) {
        std::cout << "Array is sorted" << std::endl;
    } else {
        std::cout << "Array is not sorted" << std::endl;
    }

    std::cout << "Number of comparisons: " << comparisons << std::endl;
    std::cout << "Number of swaps: " << assigns << std::endl;

    delete[] array;
    delete[] sorted_array;

    return 0;
}||||||| .old
=======
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
    auto * org_array = new int[n];
    for (int i = 0; i < n; i++) {
        org_array[i] = array[i];
    }
    if(n>40){
        small_array = false;
    }
    else{
        small_array = true;
    }
    print_if_small(org_array, n, "Original array");

    auto* sorted_array = insertion_sort(array, n);


    print_if_small(org_array, n, "Original array");
    print_if_small(array, n, "Sorted array");
    if (is_sorted(sorted_array, n)) {
        std::cout << "Array is sorted" << std::endl;
    } else {
        std::cout << "Array is not sorted" << std::endl;
    }

    std::cout << "Number of comparisons: " << comparisons << std::endl;
    std::cout << "Number of swaps: " << assigns << std::endl;

    delete[] array;
    delete[] sorted_array;

    return 0;
}>>>>>>> .new
