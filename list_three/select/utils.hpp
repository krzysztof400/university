#include <iostream>

template <typename T>
T* copy(T* begin, T* end, T*destination){
    T* original = destination;
    while(begin != end){
        *destination = *begin;
        begin++;
        destination++;
    }
    return original;
}

extern int comparisons;
extern int assigns;


template <typename T>
bool compare(T value1, T value2){
    comparisons++;
    return value1 > value2;
}

template <typename T>
bool is_equal(T value1, T value2){
    comparisons++;
    return value1 == value2;
}

template <typename T>
void assign(T &value1, T &value2){
    assigns++;
    value1 = value2;
}

template <typename T>
void swap_elements(T &value1, T &value2){
    T temp;
    assign(temp, value1);
    assign(value1, value2);
    assign(value2, temp);
}

template <typename T>
void print_array(T* array, int size){
    for (int i=0; i<size; i++){
        std::cout <<  array[i] << " ";      
    }
}

template <typename T>
bool is_sorted(T* array, int size){
    for (int i =0; i<size-1; i++){
        if (array[i] > array[i+1]){
            return false;
        }
    }
    return true;
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
    }
    
    return result;
}

template <typename T>
T* merge(T* sorted_array1, int size1, T* sorted_array2, int size2) {
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