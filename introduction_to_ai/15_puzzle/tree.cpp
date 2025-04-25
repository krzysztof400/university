#include <iostream>
#include <vector>
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
# include <climits>

class PuzzleState {
public:
    std::vector<int> board;
    int size;

    PuzzleState(const std::vector<int>& b, int s) : board(b), size(s) {}
    
    PuzzleState() : size(0) {}

    bool operator==(const PuzzleState& other) const {
        return board == other.board;
    }

    void print() const {
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                int value = board[i * size + j];
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
};

namespace std {
    template <>
    struct hash<PuzzleState> {
        size_t operator()(const PuzzleState& state) const {
            size_t hash_value = 0;
            for (int value : state.board) {
                hash_value = hash_value * 31 + static_cast<size_t>(value);
            }
            return hash_value;
        }
    };
}

struct Node {
    PuzzleState state;
    int g_score;  // Cost from start to current node
    int h_score;  // Value of heuristic
    int f_score;  // g_score + h_score
    int move;     // Move made to reach this state
    
    Node(const PuzzleState& s, int g, int h, int m)
        : state(s), g_score(g), h_score(h), f_score(g + h), move(m) {}
    
    // Porównanie dla kolejki priorytetowej
    bool operator>(const Node& other) const {
        if (f_score == other.f_score) {
            return h_score > other.h_score;
        }
        return f_score > other.f_score;
    }
};

class FifteenPuzzle {
private:
    int size;
    PuzzleState goal_state;
    std::vector<std::pair<int, int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; // Góra, dół, lewo, prawo

    // Zwraca pozycję pustego pola (wiersz, kolumna)
    std::pair<int, int> getBlankPosition(const PuzzleState& state) const {
        auto it = std::find(state.board.begin(), state.board.end(), 0);
        int index = static_cast<int>(std::distance(state.board.begin(), it));
        return {index / size, index % size};
    }

    // Oblicza liczbę inwersji w permutacji
    int countInversions(const std::vector<int>& board) const {
        int inversions = 0;
        for (size_t i = 0; i < board.size(); ++i) {
            if (board[i] == 0) continue;  // Pomijamy puste pole
            for (size_t j = i + 1; j < board.size(); ++j) {
                if (board[j] != 0 && board[i] > board[j]) {
                    inversions++;
                }
            }
        }
        return inversions;
    }

public:
    FifteenPuzzle(int s = 4) : size(s) {
        // Inicjalizacja stanu docelowego
        std::vector<int> goal(size * size);
        std::iota(goal.begin(), goal.end() - 1, 1);  // 1, 2, 3, ..., 15
        goal.back() = 0;  // Puste pole w prawym dolnym rogu
        goal_state = PuzzleState(goal, size);
    }

    // Sprawdza, czy dana permutacja jest rozwiązywalna
    bool isSolvable(const std::vector<int>& board) const {
        int inversions = countInversions(board);
        
        // Dla planszy 4x4 z pustym polem w prawym dolnym rogu
        // liczba inwersji musi być parzysta
        return inversions % 2 == 0;
    }

    PuzzleState generateRandomSolvableState(int maxDistance = 0) const {
        std::random_device rd;
        std::mt19937 g(rd());
    
        std::vector<int> board(size * size);
        std::iota(board.begin(), board.end() - 1, 1);
        board.back() = 0;  // Solved state with blank at end
    
        if (maxDistance == 0) {
            do {
                std::shuffle(board.begin(), board.end(), g);
                auto it = std::find(board.begin(), board.end(), 0);
                std::swap(*it, board.back());  // Ensure blank is in bottom-right
            } while (!isSolvable(board));
            return PuzzleState(board, size);
        } else {
            PuzzleState current(board, size);
            PuzzleState previous = current;
    
            for (int i = 0; i < maxDistance; ++i) {
                auto neighborPairs = getNeighbors(current);
                std::vector<PuzzleState> neighbors;
    
                // Filter out the previous state to avoid undoing the last move
                for (const auto& [neighbor, movedTile] : neighborPairs) {
                    if (neighbor.board != previous.board)
                        neighbors.push_back(neighbor);
                }
    
                if (neighbors.empty())
                    break; // no move possible without undoing
    
                std::uniform_int_distribution<> dist(0, neighbors.size() - 1);
                previous = current;
                current = neighbors[dist(g)];
            }
    
            return current;
        }
    }

    // Zwraca wszystkie możliwe sąsiednie stany
    std::vector<std::pair<PuzzleState, int>> getNeighbors(const PuzzleState& state) const {
        std::vector<std::pair<PuzzleState, int>> neighbors;
        auto [blank_row, blank_col] = getBlankPosition(state);

        for (const auto& [dr, dc] : directions) {
            int new_row = blank_row + dr;
            int new_col = blank_col + dc;

            // Sprawdzamy, czy nowa pozycja jest na planszy
            if (new_row >= 0 && new_row < size && new_col >= 0 && new_col < size) {
                // Tworzymy nowy stan
                PuzzleState new_state = state;
                int blank_idx = blank_row * size + blank_col;
                int new_idx = new_row * size + new_col;
                
                // Zapamiętujemy wartość przesuwanego kafelka
                int moved_tile = state.board[new_idx];
                
                // Zamieniamy puste pole z kafelkiem
                std::swap(new_state.board[blank_idx], new_state.board[new_idx]);
                
                neighbors.push_back({new_state, moved_tile});
            }
        }

        return neighbors;
    }

    // Heurystyka 1: Liczba kafelków nie na swoim miejscu
    int misplacedTiles(const PuzzleState& state) const {
        int count = 0;
        for (size_t i = 0; i < state.board.size(); ++i) {
            if (state.board[i] != 0 && state.board[i] != goal_state.board[i]) {
                count++;
            }
        }
        return count;
    }

    // Heurystyka 2: Suma odległości Manhattan
    int manhattanDistance(const PuzzleState& state) const {
        int distance = 0;
        for (size_t i = 0; i < state.board.size(); ++i) {
            int value = state.board[i];
            if (value != 0) {  // Pomijamy puste pole
                // Pozycja aktualna
                int curr_row = static_cast<int>(i) / size;
                int curr_col = static_cast<int>(i) % size;
                
                // Pozycja docelowa (dla wartości value)
                int goal_idx = value - 1;  // Pozycja w stanie docelowym
                int goal_row = goal_idx / size;
                int goal_col = goal_idx % size;
                
                // Dodajemy odległość Manhattan
                distance += std::abs(curr_row - goal_row) + std::abs(curr_col - goal_col);
            }
        }
        return distance;
    }

    // Rozwiązuje układankę używając algorytmu A* z heurystyką "misplaced tiles"
    std::pair<std::vector<int>, int> solveMisplaced(const PuzzleState& initial_state) {
        return solveWithHeuristic(initial_state, 0); // 0 oznacza misplacedTiles
    }
    
    // Rozwiązuje układankę używając algorytmu A* z heurystyką "manhattan distance"
    std::pair<std::vector<int>, int> solveManhattan(const PuzzleState& initial_state) {
        return solveWithHeuristic(initial_state, 1); // 1 oznacza manhattanDistance
    }
    
    std::pair<std::vector<int>, int> solveWithHeuristic(const PuzzleState& initial_state, int heuristic_type) {
        int visited_states = 0;
    
        struct TreeNode {
            PuzzleState state;
            int g_score;
            int h_score;
            int moved_tile;
            TreeNode* parent;
    
            TreeNode(const PuzzleState& s, int g, int h, int move, TreeNode* p)
                : state(s), g_score(g), h_score(h), moved_tile(move), parent(p) {}
    
            int f_score() const { return g_score + h_score; }
        };
    
        // Priority queue for frontier, sorted by f_score
        auto cmp = [](const TreeNode* a, const TreeNode* b) { return a->f_score() > b->f_score(); };
        std::priority_queue<TreeNode*, std::vector<TreeNode*>, decltype(cmp)> frontier(cmp);
        std::vector<TreeNode*> all_nodes;
    
        // Create and add root node
        int h_value = (heuristic_type == 0) ? misplacedTiles(initial_state) : manhattanDistance(initial_state);
        TreeNode* root = new TreeNode(initial_state, 0, h_value, -1, nullptr);
        all_nodes.push_back(root);
        frontier.push(root);
    
        TreeNode* solution_node = nullptr;
    
        while (!frontier.empty()) {
            // Get node with lowest f_score
            TreeNode* current = frontier.top();
            frontier.pop();
            visited_states++;
    
            // Check if goal state
            if (current->h_score == 0) {
                solution_node = current;
                break;
            }
    
            // Explore neighbors
            for (const auto& [neighbor, moved_tile] : getNeighbors(current->state)) {
                int h = (heuristic_type == 0) ? misplacedTiles(neighbor) : manhattanDistance(neighbor);
                TreeNode* child = new TreeNode(neighbor, current->g_score + 1, h, moved_tile, current);
                all_nodes.push_back(child);
                frontier.push(child);
            }
        }
    
        // Reconstruct path if solution found
        std::vector<int> moves;
        if (solution_node) {
            TreeNode* current = solution_node;
            while (current->parent != nullptr) {
                moves.push_back(current->moved_tile);
                current = current->parent;
            }
            std::reverse(moves.begin(), moves.end());
        }
    
        // Clean up memory
        for (TreeNode* node : all_nodes) {
            delete node;
        }
    
        return {moves, visited_states};
    }

public:
    // Przeprowadza eksperymenty porównujące różne heurystyki
    void runExperiments(int num_trials = 10, int max_distance = 0) {
        std::vector<int> misplaced_steps, manhattan_steps;
        std::vector<int> misplaced_visited, manhattan_visited;
        std::vector<double> misplaced_time, manhattan_time;
        
        for (int i = 0; i < num_trials; ++i) {
            std::cout << "Próba " << (i + 1) << "/" << num_trials << std::endl;
            
            // Generujemy losowy stan początkowy
            PuzzleState initial_state = generateRandomSolvableState(max_distance);
            
            // Rozwiązujemy używając heurystyki "misplaced tiles"
            auto start_time = std::chrono::high_resolution_clock::now();
            auto [misplaced_moves, misplaced_visited_count] = solveMisplaced(initial_state);
            auto end_time = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> misplaced_duration = end_time - start_time;
            
            if (!misplaced_moves.empty()) {
                misplaced_steps.push_back(static_cast<int>(misplaced_moves.size()));
                misplaced_visited.push_back(misplaced_visited_count);
                misplaced_time.push_back(misplaced_duration.count());
            }
            
            // Rozwiązujemy używając heurystyki "manhattan distance"
            start_time = std::chrono::high_resolution_clock::now();
            auto [manhattan_moves, manhattan_visited_count] = solveManhattan(initial_state);
            end_time = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> manhattan_duration = end_time - start_time;
            
            if (!manhattan_moves.empty()) {
                manhattan_steps.push_back(static_cast<int>(manhattan_moves.size()));
                manhattan_visited.push_back(manhattan_visited_count);
                manhattan_time.push_back(manhattan_duration.count());
            }
        }
        
        // Obliczamy średnie wartości
        auto mean = [](const std::vector<int>& v) -> double {
            if (v.empty()) return 0.0;
            return std::accumulate(v.begin(), v.end(), 0.0) / v.size();
        };
        
        auto mean_double = [](const std::vector<double>& v) -> double {
            if (v.empty()) return 0.0;
            return std::accumulate(v.begin(), v.end(), 0.0) / v.size();
        };
        
        // Podsumowanie wyników
        std::cout << "\n=== Raport z eksperymentów ===\n" << std::endl;
        
        std::cout << "Heurystyka: Liczba kafelków nie na swoim miejscu" << std::endl;
        std::cout << "- Średnia liczba kroków: " << mean(misplaced_steps) << std::endl;
        std::cout << "- Średnia liczba odwiedzonych stanów: " << mean(misplaced_visited) << std::endl;
        std::cout << "- Średni czas rozwiązania: " << mean_double(misplaced_time) << " s\n" << std::endl;
        
        std::cout << "Heurystyka: Odległość Manhattan" << std::endl;
        std::cout << "- Średnia liczba kroków: " << mean(manhattan_steps) << std::endl;
        std::cout << "- Średnia liczba odwiedzonych stanów: " << mean(manhattan_visited) << std::endl;
        std::cout << "- Średni czas rozwiązania: " << mean_double(manhattan_time) << " s" << std::endl;
    }
};

int main(int argc, char* argv[]) {
    int max_distance = 0;
    if (argc > 1) {
        try {
            max_distance = std::stoi(argv[1]);
        } catch (const std::exception& e) {
            std::cerr << "Invalid argument for maxDistance. Using default value of 0." << std::endl;
        }
    }
    FifteenPuzzle puzzle;
    
    // Generujemy losowy stan początkowy
    PuzzleState initial_state = puzzle.generateRandomSolvableState(max_distance);
    
    std::cout << "Stan początkowy:" << std::endl;
    initial_state.print();
    
    // Rozwiązujemy używając heurystyki "misplaced tiles"
    std::cout << "\nRozwiązywanie używając heurystyki 'misplaced tiles'..." << std::endl;
    auto start_time = std::chrono::high_resolution_clock::now();
    auto [misplaced_moves, misplaced_visited] = puzzle.solveMisplaced(initial_state);
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> misplaced_duration = end_time - start_time;
    
    if (!misplaced_moves.empty()) {
        std::cout << "Rozwiązanie znalezione w " << misplaced_moves.size() << " krokach!" << std::endl;
        std::cout << "Odwiedzone stany: " << misplaced_visited << std::endl;
        std::cout << "Czas rozwiązania: " << misplaced_duration.count() << " s" << std::endl;
        std::cout << "Sekwencja ruchów (kafelki do przesunięcia): ";
        for (int move : misplaced_moves) {
            std::cout << move << " ";
        }
        std::cout << std::endl;
    } else {
        std::cout << "Nie znaleziono rozwiązania!" << std::endl;
    }
    
    // Rozwiązujemy używając heurystyki "manhattan distance"
    std::cout << "\nRozwiązywanie używając heurystyki 'manhattan distance'..." << std::endl;
    start_time = std::chrono::high_resolution_clock::now();
    auto [manhattan_moves, manhattan_visited] = puzzle.solveManhattan(initial_state);
    end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> manhattan_duration = end_time - start_time;
    
    if (!manhattan_moves.empty()) {
        std::cout << "Rozwiązanie znalezione w " << manhattan_moves.size() << " krokach!" << std::endl;
        std::cout << "Odwiedzone stany: " << manhattan_visited << std::endl;
        std::cout << "Czas rozwiązania: " << manhattan_duration.count() << " s" << std::endl;
        std::cout << "Sekwencja ruchów (kafelki do przesunięcia): ";
        for (int move : manhattan_moves) {
            std::cout << move << " ";
        }
        std::cout << std::endl;
    } else {
        std::cout << "Nie znaleziono rozwiązania!" << std::endl;
    }
    
    // Uruchomienie eksperymentów
    std::cout << "\nCzy chcesz przeprowadzić pełne eksperymenty? [T/n]" << std::endl;
    char choice;
    std::cin >> choice;
    if (choice != 'n' && choice != 'N') {
        std::cout << "\nUruchamianie eksperymentów (to może zająć kilka minut)..." << std::endl;
        puzzle.runExperiments(100, max_distance);
    }
    
    return 0;
}