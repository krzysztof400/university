#include <vector>
#include "utils.hpp"
#include <iostream>
#include <cmath>

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
T* merge(T* sorted_array1, int size1, T* sorted_array2, int size2) {
    print_if_small(sorted_array1, size1, "Sorted array 1");
    print_if_small(sorted_array2, size2, "Sorted array 2");
    T* merged = new T[size1 + size2];
    int i = 0;
    int p1 = 0;
    int p2 = 0;
    while(p1 < size1 && p2 < size2) {
        if(compare(sorted_array1[p1], sorted_array2[p2])) {
            assign(merged[i], sorted_array2[p2]);
            p2++;
        } else {
            assign(merged[i], sorted_array1[p1]);
            p1++;
        }
        i++;
    }
    
    while(p1 < size1) {
        assign(merged[i], sorted_array1[p1]);
        p1++;
        i++;
    }
    
    while(p2 < size2) {
        assign(merged[i], sorted_array2[p2]);
        p2++;
        i++;
    }
        
    return merged;
}

template <typename T>
T* merge_sort(T* array, int size) {
    if (size <= 1) {
        T* result = new T[size];
        if (size == 1) {
            result[0] = array[0];
        }
        return result;
    }
    
    int mid = size / 2;
    int left_size = mid;
    int right_size = size - mid;

    T* left_array = new T[left_size];
    T* right_array = new T[right_size];

    for (int i = 0; i < left_size; i++) {
        assign(left_array[i], array[i]);
    }
    
    for (int i = 0; i < right_size; i++) {
        assign(right_array[i], array[mid + i]);
    }
    
    T* sorted_left = merge_sort(left_array, left_size);
    T* sorted_right = merge_sort(right_array, right_size);
    
    delete[] left_array;
    delete[] right_array;
    
    T* result = merge(sorted_left, left_size, sorted_right, right_size);
    
    delete[] sorted_left;
    delete[] sorted_right;
    
    return result;
}

int main() {
    int n;
    std::cin >> n;
    
    auto* array = new int[n];
    for (int i = 0; i < n; i++) {
        std::cin >> array[i];
    }
    auto* org_array = new int[n];
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

    int* sorted_array = merge_sort(array, n);

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
}