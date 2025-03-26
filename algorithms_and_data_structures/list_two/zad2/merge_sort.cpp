#include <vector>

std::vector<int> merge_sort(int n, std::vector<int> array);
std::vector<int> merge(std::vector<int> sorted_array1, std::vector<int> sorted_array2);

std::vector<int> merge_sort (int n, std::vector<int> array){
    int size = n;

    if (size <= 1){
        return array;
    }

    int mid = size/2;
    std::vector<int> left_array(array.begin(), array.begin() + mid);
    std::vector<int> right_array(array.begin() + mid, array.end());

    return merge(merge_sort(left_array.size(), left_array), merge_sort(right_array.size(), right_array));
}


std::vector<int> merge(std::vector<int> sorted_array1, std::vector<int> sorted_array2){
    std::vector<int> return_array;
    int size1 = sorted_array1.size();
    int size2 = sorted_array2.size();
    int p1 = 0;
    int p2 = 0;
    while(p1<size1 || p2<size2){
        if (p1 >= size1) {
            return_array.push_back(sorted_array2[p2]);
            p2++;
        } else if (p2 >= size2) {
            return_array.push_back(sorted_array1[p1]);
            p1++;
        } else if (sorted_array1[p1] > sorted_array2[p2]) {
            return_array.push_back(sorted_array2[p2]);
            p2++;
        } else {
            return_array.push_back(sorted_array1[p1]);
            p1++;
        }
    }
    return return_array;
}