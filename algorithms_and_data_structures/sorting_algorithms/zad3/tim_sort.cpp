#include "utils.hpp"
#include <iostream>

extern bool small_array;

template <typename T>
void print_if_small(T* array, int size, const std::string& msg) {
    if (small_array) {
        std::cout << msg << ": [ ";
        print_array(array, size);
        std::cout << "]" << std::endl;
    }
}

template <typename T>
void merge(T* array, int left, int mid, int right) {
    int n1 = mid - left;
    int n2 = right - mid;

    // Create temporary arrays
    T* L = new T[n1];
    T* R = new T[n2];
    
    // Copy data to temporary arrays
    for (int i = 0; i < n1; ++i) assign(L[i], array[left + i]);
    for (int i = 0; i < n2; ++i) assign(R[i], array[mid + i]);

    // Merge the temporary arrays back
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (compare(R[j], L[i])) {
            assign(array[k++], L[i++]);
        } else {
            assign(array[k++], R[j++]);
        }
    }

    // Copy the remaining elements of L[]
    while (i < n1) assign(array[k++], L[i++]);
    
    // Copy the remaining elements of R[]
    while (j < n2) assign(array[k++], R[j++]);
    
    // Free temporary arrays
    delete[] L;
    delete[] R;
}

template <typename T>
int find_runs(T* array, int size, int* runs, int max_runs) {
    if (size <= 0) return 0;
    
    int run_count = 0;
    runs[run_count++] = 0; // Start of first run
    
    for (int i = 1; i < size; i++) {
        if (compare(array[i-1], array[i])) {
            // If current element is smaller than previous, it's the start of a new run
            if (run_count < max_runs) {
                runs[run_count++] = i;
            }
        }
    }
    
    // Add end of array as the final position
    if (run_count < max_runs) {
        runs[run_count++] = size;
    }
    
    return run_count;
}

template <typename T>
void adaptive_run_merge_sort(T* array, int size) {
    if (size <= 1) return;
    
    // Maximum possible number of runs (worst case: each element is its own run)
    const int max_runs = size + 1;
    int* runs = new int[max_runs];
    int* new_runs = new int[max_runs];
    
    // Find natural runs in the array
    int run_count = find_runs(array, size, runs, max_runs);
    
    // Print runs if small array
    if (small_array) {
        std::cout << "Found " << run_count - 1 << " runs: ";
        for (int i = 0; i < run_count - 1; i++) {
            std::cout << "[" << runs[i] << "," << runs[i+1] << ") ";
        }
        std::cout << std::endl;
    }
    
    // Merge runs until only one run remains
    while (run_count > 2) {
        int new_run_count = 1;
        new_runs[0] = runs[0]; // Always keep the first position
        
        // Merge pairs of runs
        for (int i = 0; i + 2 < run_count; i += 2) {
            int left = runs[i];
            int mid = runs[i+1];
            int right = runs[i+2];
            
            merge(array, left, mid, right);
            
            // Only add the end of the merged run
            new_runs[new_run_count++] = right;
            
            // Print after merge if small array
            if (small_array) {
                std::cout << "After merging [" << left << "," << mid << ") with [" << mid << "," << right << "): ";
                print_if_small(array, size, "Current array");
            }
        }
        
        // Handle odd number of runs - add the last run boundary
        if (run_count % 2 == 0) {
            new_runs[new_run_count++] = runs[run_count-1];
        }
        
        // Copy new runs to runs array
        for (int i = 0; i < new_run_count; i++) {
            runs[i] = new_runs[i];
        }
        run_count = new_run_count;
    }
    
    delete[] runs;
    delete[] new_runs;
}

bool small_array = false;

int main() {
    int n;
    std::cin >> n;

    auto* array = new int[n];
    for (int i = 0; i < n; ++i) {
        std::cin >> array[i];
    }
    auto* original_array = new int[n];
    for (int i = 0; i < n; ++i) {
        original_array[i] = array[i];
    }

    small_array = (n <= 40);
    print_if_small(original_array, n, "Original array");
    
    adaptive_run_merge_sort(array, n);

    print_if_small(original_array, n, "Original array");
    print_if_small(array, n, "Sorted array");
    std::cout << "Is sorted: " << (is_sorted(array, n) ? "Yes" : "No") << std::endl;

    std::cout << "Number of comparisons: " << comparisons << std::endl;
    std::cout << "Number of swaps: " << assigns << std::endl;

    delete[] array;
    delete[] original_array; 
    return 0;
}