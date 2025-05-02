#include <iostream>
#include <random>
#include <algorithm>

int main(int argc, char *argv[]){
    if(argc != 2){
        std::cout << "Incorrect amount of arguments" << std::endl;
        std::cout << "./array_generator <size_of_array>" << std::endl;
        return 0;
    }

    int size;
    size = std::stoi(argv[1]);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, 2 * size - 1);

    int array[size];

    for(int i=0; i<size; i++){
        array[i] = dis(gen);
    }

    std::cout << size << std::endl;
    for(int i=0; i<size; i++){
        std::cout << array[i] << " ";
    }
    std::cout << std::endl;
}