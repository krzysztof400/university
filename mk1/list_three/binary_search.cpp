#include <iostream>
#include <algorithm>
#include <cmath>
#include "utils.hpp"

int binary_search(int size, int value, int array[]){
    int low = 0;
    int high = size-1;
    int mid;
    while(compare(high, low)){
        mid = (low + high) / 2;
        if(compare(array[mid], value)){
            high = mid-1;
        }
        else if(compare(value, array[mid])){
            low = mid+1;
        }
        else{
            return 1;
        }
    }
    return 0;
}

int main(int argc, char *argv[]) {
    int value;
    if(argc != 2){
        std::cout << "Incorrect amount of arguments." << std::endl;
        std::cout << "./binary_search <value>" << std::endl;
        return 0;
    }
    value = std::stoi(argv[1]);

    int size;
    std::cin >> size;

    int array[size];
    for(int i=0; i<size; i++){
        std::cin >> array[i];
        // std::cout << array[i] << " ";
    }
    // std::cout << std::endl;

    int result = binary_search(size, value, array);
    if(result == 0){
        std::cout << "Value not found" << std::endl;
    }
    else{
        std::cout << "Value found" << std::endl;
    }
    std::cout << "Comparisons: " << comparisons << std::endl;
    return 1;
}