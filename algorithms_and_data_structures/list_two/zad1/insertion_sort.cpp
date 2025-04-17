#include <vector>
#include <iostream>

int comparisons = 0;
int swaps = 0;

bool compare(int a, int b) {
    comparisons++;
    return a < b;
}

bool compare_equal(int a, int b) {
    comparisons++;
    return a <= b;
}

void assign(int &a, int b) {
    swaps++;
    a = b;
}

std::vector<int> insertion_sort(int n, std::vector<int> array){
    for(int i = 0; i<n; i++){
        int key;
        assign(key, array[i]);
        int j;
        assign(j, i-1);
        while (compare(key, array[j])){
            assign(array[j+1], array[j]);
            j--;
        }
        assign(array[j+1], key);
    }
    return array;
}

bool is_sorted(std::vector<int> array) {
    for (int i = 1; i < array.size(); i++) {
        if (array[i] < array[i-1]) {
            return false;
        }
    }
    return true;
}

void print_array(std::vector<int> array) {
    for (int num : array) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
}

int main() {
    int n;
    std::cin >> n;
    std::vector<int> array(n);
    for (int i = 0; i < n; i++) {
        std::cin >> array[i];
    }

    std::vector<int> sorted_array = insertion_sort(n, array);

    if (n < 40) {
        std::cout << "Original array: " << std::endl;
        print_array(array);
        std::cout << "Sorted array: " << std::endl;
        print_array(sorted_array);
    }

    std::cout << "Number of comparisons: " << comparisons << std::endl;
    std::cout << "Number of swaps: " << swaps << std::endl;

    if(is_sorted(sorted_array)) {
        std::cout << "Array is sorted" << std::endl;
    } else {
        std::cout << "Array is not sorted" << std::endl;
    }

    return 0;
}