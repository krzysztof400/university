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

// Assume compare(a, b) returns (a > b)
// Assume assign(a, b) does: a = b
void insertion_sort(int* array, int size) {
    for (int i = 1; i < size; ++i) {
        int key;
        assign(key, array[i]);
        int j = i - 1;
        while (j >= 0 && compare(array[j], key)) {
            assign(array[j + 1], array[j]);
            --j;
        }
        assign(array[j + 1], key);
    }
}

int get_median(int* array, int size) {
    merge_sort(array, size);
    return array[size / 2];
}

int select(int size, int value, int array[]) {
    if (!compare(size, 5)) {  
        // std::cout << "Using insertion sort" << std::endl;
        insertion_sort(array, size);
        return array[value - 1];
    }

    int group_count = (size + 4) / 5;
    int* medians = new int[group_count];

    for (int i = 0; i < group_count; ++i) {
        int start = i * 5;
        int end = start + 5;
        if (compare(end, size)) {
            assign(end, size);
        }
        int temp[5];
        for (int j = start; j < end; ++j) {
            assign(temp[j - start], array[j]);
        }
        int temp2 = get_median(temp, end - start);
        assign(medians[i], temp2);
    }

    int pivot = select(group_count, (group_count + 1) / 2, medians);
    delete[] medians;

    int* left = new int[size];
    int* right = new int[size];
    int* equal = new int[size];
    int l = 0, r = 0, e = 0;

    for (int i = 0; i < size; ++i) {
        if (compare(pivot, array[i])) {
            assign(left[l], array[i]);
            ++l;
        } else if (compare(array[i], pivot)) {
            assign(right[r], array[i]);
            ++r;
        } else {
            assign(equal[e], array[i]);
            ++e;
        }
    }

    int result;
    if (!compare(value, l)) {  // value <= l
        result = select(l, value, left);
    } else if (!compare(value, l + e)) {  // value <= l + e
        assign(result, pivot);
    } else {
        result = select(r, value - l - e, right);
    }

    delete[] left;
    delete[] right;
    delete[] equal;

    return result;
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
    int position = select(size, value, array);
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

    return 1;
}