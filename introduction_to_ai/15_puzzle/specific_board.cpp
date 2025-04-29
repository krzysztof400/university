#include <iostream>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <chrono>
#include <random>
#include <iomanip>
#include <numeric>
#include <cmath>
#include <string>

// For a 4x4 puzzle, we need 16 positions, each can be represented using 4 bits (0-15)
// So we need 64 bits total to represent the state
class PuzzleState {
public:
    uint64_t board;
    int size;

    // Constructor from encoded board
    PuzzleState(uint64_t b, int s) : board(b), size(s) {}
    
    // Default constructor
    PuzzleState() : board(0), size(0) {}

    bool operator==(const PuzzleState& other) const {
        return board == other.board;
    }

    // Get value at position (row, col)
    int getValue(int row, int col) const {
        int pos = row * size + col;
        return static_cast<int>((board >> (pos * 4)) & 0xF);
    }

    // Set value at position (row, col)
    void setValue(int row, int col, int value) {
        int pos = row * size + col;
        // Clear the target bits
        board &= ~(static_cast<uint64_t>(0xF) << (pos * 4));
        // Set the new value
        board |= static_cast<uint64_t>(value) << (pos * 4);
    }

    // Find the position of a specific value
    std::pair<int, int> findValue(int value) const {
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                if (getValue(i, j) == value) {
                    return {i, j};
                }
            }
        }
        return {-1, -1}; // Not found
    }

    // Swap values at two positions
    void swapValues(int r1, int c1, int r2, int c2) {
        int val1 = getValue(r1, c1);
        int val2 = getValue(r2, c2);
        setValue(r1, c1, val2);
        setValue(r2, c2, val1);
    }

    void print() const {
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                int value = getValue(i, j);
                if (value == 0) {
                    std::cout << std::setw(2) << " " << " ";
                } else {
                    std::cout << std::setw(2) << value << " ";
                }
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    // Helper method to convert from vector representation to int64
    static PuzzleState fromVector(const std::vector<int>& vec, int size) {
        PuzzleState state(0, size);
        for (int i = 0; i < size * size; ++i) {
            state.board |= static_cast<uint64_t>(vec[i]) << (i * 4);
        }
        return state;
    }
};

namespace std {
    template <>
    struct hash<PuzzleState> {
        size_t operator()(const PuzzleState& state) const {
            // We can use the board value directly as a hash since it's already a unique identifier
            return state.board;
        }
    };
}

struct Node {
    PuzzleState state;
    int g_score;
    int h_score;
    
    Node(const PuzzleState& s, int g, int h)
        : state(s), g_score(g), h_score(h) {}
    
    // Compute f_score on the fly
    int getFScore() const {
        return g_score + h_score;
    }
    
    // Comparison for priority queue
    bool operator>(const Node& other) const {
        if (getFScore() == other.getFScore()) {
            return h_score > other.h_score;
        }
        return getFScore() > other.getFScore();
    }
};

class FifteenPuzzle {
private:
    int size;
    PuzzleState goal_state;
    std::vector<std::pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; // Up, Down, Left, Right

    // Returns position of blank tile (row, column)
    std::pair<int, int> getBlankPosition(const PuzzleState& state) const {
        return state.findValue(0);
    }

    // Counts inversions in the board
    int countInversions(const PuzzleState& state) const {
        int inversions = 0;
        
        for (int i = 0; i < size * size; ++i) {
            int row_i = i / size;
            int col_i = i % size;
            int val_i = state.getValue(row_i, col_i);
            
            if (val_i == 0) continue;  // Skip the blank tile
            
            for (int j = i + 1; j < size * size; ++j) {
                int row_j = j / size;
                int col_j = j % size;
                int val_j = state.getValue(row_j, col_j);
                
                if (val_j != 0 && val_i > val_j) {
                    inversions++;
                }
            }
        }
        return inversions;
    }

public:
    FifteenPuzzle(int s = 4) : size(s) {
        // Initialize goal state
        uint64_t board = 0;
        for (int i = 0; i < size * size - 1; ++i) {
            board |= static_cast<uint64_t>(i + 1) << (i * 4);
        }
        // Last position is 0 (blank)
        goal_state = PuzzleState(board, size);
    }

    // Check if the given state is solvable
    bool isSolvable(const PuzzleState& state) const {
        int inversions = countInversions(state);
        return inversions % 2 == 0;
    }

    PuzzleState generate_from_vector(std::vector<int> my_board, int size = 16) const {
        uint64_t board = 0;
        for (int i = 0; i < size; ++i) {
            board |= static_cast<uint64_t>(my_board[i]) << (i * 4);
        }
        return PuzzleState(board, 4);
    }
    

    // Get all possible neighbor states
    std::vector<std::pair<PuzzleState, int>> getNeighbors(const PuzzleState& state) const {
        std::vector<std::pair<PuzzleState, int>> neighbors;
        auto [blank_row, blank_col] = getBlankPosition(state);

        for (const auto& [dr, dc] : directions) {
            int new_row = blank_row + dr;
            int new_col = blank_col + dc;

            // Check if new position is on the board
            if (new_row >= 0 && new_row < size && new_col >= 0 && new_col < size) {
                // Create new state by swapping blank with the tile
                PuzzleState new_state = state;
                int moved_tile = state.getValue(new_row, new_col);
                
                new_state.swapValues(blank_row, blank_col, new_row, new_col);
                
                neighbors.push_back({new_state, moved_tile});
            }
        }

        return neighbors;
    }

    // Heuristic 1: Number of misplaced tiles
    int misplacedTiles(const PuzzleState& state) const {
        int count = 0;
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                int value = state.getValue(i, j);
                if (value != 0 && value != goal_state.getValue(i, j)) {
                    count++;
                }
            }
        }
        return count;
    }

    // Heuristic 2: Manhattan distance
    int manhattanDistance(const PuzzleState& state) const {
        int distance = 0;
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                int value = state.getValue(i, j);
                if (value != 0) {  // Skip blank tile
                    // Current position
                    int curr_row = i;
                    int curr_col = j;
                    
                    // Goal position (for value)
                    int goal_idx = value - 1;  // Position in goal state
                    int goal_row = goal_idx / size;
                    int goal_col = goal_idx % size;
                    
                    // Add Manhattan distance
                    distance += std::abs(curr_row - goal_row) + std::abs(curr_col - goal_col);
                }
            }
        }
        return distance;
    }

    // Solve using "misplaced tiles" heuristic
    std::pair<std::vector<int>, int> solveMisplaced(const PuzzleState& initial_state) {
        return solveWithHeuristic(initial_state, 0); // 0 means misplacedTiles
    }
    
    // Solve using "manhattan distance" heuristic
    std::pair<std::vector<int>, int> solveManhattan(const PuzzleState& initial_state) {
        return solveWithHeuristic(initial_state, 1); // 1 means manhattanDistance
    }
    
private:
    // Internal A* algorithm implementation
    std::pair<std::vector<int>, int> solveWithHeuristic(const PuzzleState& initial_state, int heuristic_type) {
        int visited_states = 0;
        
        // Priority queue for A*
        std::priority_queue<Node, std::vector<Node>, std::greater<Node>> open_set;
        
        // Closed set of visited states
        std::unordered_set<PuzzleState> closed_set;
        
        // Map of previous states to reconstruct path
        std::unordered_map<PuzzleState, std::pair<PuzzleState, int>> came_from;
        
        // Map of G scores for each state
        std::unordered_map<PuzzleState, int> g_score;
        
        // Calculate heuristic value for initial state
        int h_value;
        if (heuristic_type == 0) {
            h_value = misplacedTiles(initial_state);
        } else {
            h_value = manhattanDistance(initial_state);
        }
        
        // Add initial state
        open_set.push(Node(initial_state, 0, h_value));
        g_score[initial_state] = 0;
        
        while (!open_set.empty()) {
            // Get state with lowest F score
            Node current_node = open_set.top();
            open_set.pop();
            visited_states++;
            
            // If we found the solution
            if (current_node.state == goal_state) {
                // Reconstruct path
                std::vector<int> moves;
                PuzzleState current = current_node.state;
                
                while (came_from.find(current) != came_from.end()) {
                    auto [prev_state, move] = came_from[current];
                    if (move != -1) {  // -1 means initial state
                        moves.push_back(move);
                    }
                    current = prev_state;
                }
                
                // Reverse path
                std::reverse(moves.begin(), moves.end());
                
                return {moves, visited_states};
            }
            
            // Add current state to closed set
            closed_set.insert(current_node.state);
            
            // Check all neighbor states
            for (const auto& [neighbor_state, moved_tile] : getNeighbors(current_node.state)) {
                // Skip already checked states
                if (closed_set.find(neighbor_state) != closed_set.end()) {
                    continue;
                }
                
                // Calculate new G score
                int tentative_g_score = g_score[current_node.state] + 1;
                
                // Check if neighbor is already in open set
                bool in_open_set = g_score.find(neighbor_state) != g_score.end();
                
                // If neighbor wasn't visited yet or we found a better path
                if (!in_open_set || tentative_g_score < g_score[neighbor_state]) {
                    // Update path
                    came_from[neighbor_state] = {current_node.state, moved_tile};
                    g_score[neighbor_state] = tentative_g_score;
                    
                    // Calculate new H score
                    int h_value;
                    if (heuristic_type == 0) {
                        h_value = misplacedTiles(neighbor_state);
                    } else {
                        h_value = manhattanDistance(neighbor_state);
                    }
                    
                    // Add to priority queue
                    open_set.push(Node(neighbor_state, tentative_g_score, h_value));
                }
            }
        }
        
        // If no solution found
        return {{}, visited_states};
    }
};

int main(int argc, char* argv[]) {
    bool only_manhattan = false;
    std::vector<int> given_board;
    if (argc > 1) {
        try {
            only_manhattan = (std::stoi(argv[1]) == 1);
        } catch (const std::exception& e) {
            std::cerr << "Invalid arguments. Using default values.\n";
        }
    }

    for (int i = 0; i < 16; ++i) {
        int value;
        std::cout << "Enter value for position " << i + 1 << " (0 for blank): ";
        std::cin >> value;
        given_board.push_back(value);
    }
    
    FifteenPuzzle puzzle;
    
    // Generate random initial state
    PuzzleState initial_state = puzzle.generate_from_vector(given_board, 16);
    
    std::cout << "Initial state:" << std::endl;
    initial_state.print();
    
    if(!only_manhattan){
        // Solve using "misplaced tiles" heuristic
        std::cout << "\nSolving using 'misplaced tiles' heuristic..." << std::endl;
        auto start_time = std::chrono::high_resolution_clock::now();
        auto [misplaced_moves, misplaced_visited] = puzzle.solveMisplaced(initial_state);
        auto end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> misplaced_duration = end_time - start_time;
        
        if (!misplaced_moves.empty()) {
            std::cout << "Solution found in " << misplaced_moves.size() << " steps!" << std::endl;
            std::cout << "Visited states: " << misplaced_visited << std::endl;
            std::cout << "Solution time: " << misplaced_duration.count() << " s" << std::endl;
            std::cout << "Move sequence (tiles to move): ";
            for (int move : misplaced_moves) {
                std::cout << move << " ";
            }
            std::cout << std::endl;
        } else {
            std::cout << "No solution found!" << std::endl;
        }
    }
    
    // Solve using "manhattan distance" heuristic
    std::cout << "\nSolving using 'manhattan distance' heuristic..." << std::endl;
    auto start_time = std::chrono::high_resolution_clock::now();
    auto [manhattan_moves, manhattan_visited] = puzzle.solveManhattan(initial_state);
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> manhattan_duration = end_time - start_time;
    
    if (!manhattan_moves.empty()) {
        std::cout << "Solution found in " << manhattan_moves.size() << " steps!" << std::endl;
        std::cout << "Visited states: " << manhattan_visited << std::endl;
        std::cout << "Solution time: " << manhattan_duration.count() << " s" << std::endl;
        std::cout << "Move sequence (tiles to move): ";
        for (int move : manhattan_moves) {
            std::cout << move << " ";
        }
        std::cout << std::endl;
    } else {
        std::cout << "No solution found!" << std::endl;
    }
    return 0;
}