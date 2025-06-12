#include <iostream>
#include "utils.hpp"

bool small_array = false;

template <typename T>
void swap(T &a, T &b) {
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
void partition(T* array, int left, int right, int &lp, int &rp, int total_size) {
    if (compare(array[left], array[right])) {
        swap(array[left], array[right]);
    }

    T pivot1, pivot2;
    assign(pivot1, array[left]);
    assign(pivot2, array[right]);

    int i = left + 1;
    int lt = left + 1;
    int gt = right - 1;

    print_if_small(array, total_size, "Before partition");

    while (i <= gt) {
        if (compare(array[lt], array[gt])) {
            if (compare(pivot1, array[i])) {
                swap(array[i], array[lt]);
                lt++;
                i++;
            } else if (compare(array[i], pivot2)) {
                swap(array[i], array[gt]);
                gt--;
            } else {
                i++;
            }
        }
        else {
            if (compare(pivot1, array[i])) {
                swap(array[i], array[lt]);
                lt++;
                i++;
            }
            else if (compare(array[i], pivot2)){
                swap(array[i], array[gt]);
                gt--;
            }
            else {
                i++;
            }
        }
    }

    lt--;
    gt++;

    swap(array[left], array[lt]);
    swap(array[right], array[gt]);

    lp = lt;
    rp = gt;

    print_if_small(array, total_size, "After partition");
}

template <typename T>
void DP_quick_sort(T* array, int left, int right, int total_size) {
    if (left >= right) return;

    print_if_small(array, total_size, "Sorting");

    int lp, rp;
    partition(array, left, right, lp, rp, total_size);

    DP_quick_sort(array, left, lp - 1, total_size);
    DP_quick_sort(array, lp + 1, rp - 1, total_size);
    DP_quick_sort(array, rp + 1, right, total_size);

    print_if_small(array, total_size, "After recursion");
}

int main() {
    int n;
    std::cin >> n;
    auto* original_array = new int[n];
    for (int i = 0; i < n; i++) {
        std::cin >> original_array[i];
    }
    auto* array = new int[n];
    for (int i = 0; i < n; i++) {
        array[i] = original_array[i];
    }

    small_array = (n <= 40);

    DP_quick_sort(array, 0, n - 1, n);

    print_if_small(original_array, n, "Original array");
    print_if_small(array, n, "Sorted array");
    std::cout << "Is sorted: " << (is_sorted(array, n) ? "Yes" : "No") << std::endl;

    std::cout << "Number of comparisons: " << comparisons << std::endl;
    std::cout << "Number of swaps: " << assigns << std::endl;

    delete[] array;
    return 0;
}
