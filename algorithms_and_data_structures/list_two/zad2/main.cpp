#include "functions.h"
#include <vector>
#include <iostream>

int main(int argc, char *argv[]) {
    std::vector<int> array = generating_n_array(argc > 1 ? atoi(argv[1]) : 10);

    for(int num : array){
        std::cout << num << " ";
    }
    std::cout << std::endl;

    std::vector<int> sorted_array = insertion_sort(array.size(), array);
    std::vector<int> sorted_array2 = merge_sort(array.size(), array);
    std::vector<int> sorted_array3 = quick_sort(array.size(), array);

    for(int num : sorted_array){
        std::cout << num << " ";
    }
    std::cout << std::endl;
 
    for(int num : sorted_array2){
        std::cout << num << " ";
    }
    std::cout << std::endl;

    for(int num : sorted_array3){
        std::cout << num << " ";
    }
    std::cout << std::endl;

    return 0;
}