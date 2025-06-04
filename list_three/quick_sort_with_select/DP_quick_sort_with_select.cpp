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
std::pair<T, int> find_kth(T* array, int low, int high, int k) {
    int size = high - low + 1;
    if (size <= 5) {
        T* temp = new T[size];
        int* indices = new int[size];
        for (int i = 0; i < size; ++i) {
            temp[i] = array[low + i];
            indices[i] = low + i;
        }
        for (int i = 1; i < size; ++i) {
            T key = temp[i];
            int idx = indices[i];
            int j = i - 1;
            while (j >= 0 && compare(temp[j], key)) {
                temp[j + 1] = temp[j];
                indices[j + 1] = indices[j];
                --j;
            }
            temp[j + 1] = key;
            indices[j + 1] = idx;
        }
        T value = temp[k - 1];
        int index = indices[k - 1];
        delete[] temp;
        delete[] indices;
        return {value, index};
    }

    int group_count = (size + 4) / 5;
    T* medians = new T[group_count];
    int* median_indices = new int[group_count];
    for (int i = 0; i < group_count; ++i) {
        int start = low + i * 5;
        int end = start + 5;
        if (end > high + 1) end = high + 1;
        int len = end - start;
        T* temp = new T[len];
        int* indices = new int[len];
        for (int j = 0; j < len; ++j) {
            temp[j] = array[start + j];
            indices[j] = start + j;
        }
        for (int j = 1; j < len; ++j) {
            T key = temp[j];
            int idx = indices[j];
            int m = j - 1;
            while (m >= 0 && compare(temp[m], key)) {
                temp[m + 1] = temp[m];
                indices[m + 1] = indices[m];
                --m;
            }
            temp[m + 1] = key;
            indices[m + 1] = idx;
        }
        medians[i] = temp[len / 2];
        median_indices[i] = indices[len / 2];
        delete[] temp;
        delete[] indices;
    }

    auto [pivot, pivot_index] = find_kth(medians, 0, group_count - 1, (group_count + 1) / 2);
    delete[] medians;
    delete[] median_indices;

    T* left = new T[size];
    T* right = new T[size];
    T* equal = new T[size];
    int l = 0, r = 0, e = 0;
    for (int i = low; i <= high; ++i) {
        if (compare(pivot, array[i])) {
            assign(left[l++], array[i]);
        } else if (compare(array[i], pivot)) {
            assign(right[r++], array[i]);
        } else {
            assign(equal[e++], array[i]);
        }
    }

    std::pair<T, int> result;
    if (k <= l) {
        result = find_kth(left, 0, l - 1, k);
    } else if (k <= l + e) {
        result = {pivot, pivot_index};
    } else {
        result = find_kth(right, 0, r - 1, k - l - e);
    }

    delete[] left;
    delete[] right;
    delete[] equal;
    return result;
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
        } else {
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

    int size = right - left + 1;
    print_if_small(array, total_size, "Sorting");

    int k1 = (size + 2) / 3;
    int k2 = (2 * size + 2) / 3;
    if (size <= 5) {
        T* temp = new T[size];
        for (int i = 0; i < size; ++i) {
            temp[i] = array[left + i];
        }
        insertion_sort(temp, size);
        for (int i = 0; i < size; ++i) {
            array[left + i] = temp[i];
        }
        delete[] temp;
        print_if_small(array, total_size, "After recursion");
        return;
    }

    auto [pivot1, idx1] = find_kth(array, left, right, k1);
    auto [pivot2, idx2] = find_kth(array, left, right, k2);

    if (compare(pivot2, pivot1)) {
        std::swap(pivot1, pivot2);
        std::swap(idx1, idx2);
    }

    swap(array[left], array[idx1]);
    swap(array[right], array[idx2]);

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
    delete[] original_array; // Poprawka: zwolnij pamięć
    return 0;
}