Lab 3 - Dijkstra Implementations
Author: Krzysztof Zając

# Description:

This project implements three variants of Dijkstra's algorithm for the Shortest Path problem:

1. Standard Dijkstra (using std::priority_queue)
2. Dial's Algorithm (using buckets)
3. Radix Heap Dijkstra

# Compilation:

To compile the project, simply run:
make

This will generate three executables:

- dijkstra
- dial
- radixheap

# Usage:

1. Single Source Shortest Path (SSSP):
   ./[algorithm] -d [graph_file] -ss [sources_file] -oss [output_file]

   Example:
   ./dijkstra -d inputs/USA-road-d.NY.gr -ss inputs/sources.ss -oss res.ss.res

2. Point-to-Point (P2P):
   ./[algorithm] -d [graph_file] -p2p [pairs_file] -op2p [output_file]

   Example:
   ./dial -d inputs/USA-road-d.NY.gr -p2p inputs/pairs.p2p -op2p res.p2p.res

# Requirements:

- Linux environment (Ubuntu tested)
- g++ compiler supporting C++17
- Make
