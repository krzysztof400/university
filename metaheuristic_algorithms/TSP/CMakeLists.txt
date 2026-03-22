cmake_minimum_required(VERSION 3.28)
project(Traveling_Salesman_Problem CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(TSP main.cpp)

target_sources(TSP
    PRIVATE
        FILE_SET CXX_MODULES FILES
        graph.cppm
)