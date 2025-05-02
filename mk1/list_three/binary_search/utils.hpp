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