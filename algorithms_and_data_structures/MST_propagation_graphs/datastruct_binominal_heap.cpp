#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>
#include <chrono>
#include <fstream>

int assignments = 0;
int comparisons = 0;

void assign(int &a, int b) {
    a = b;
    assignments++;
}

bool compare(int a, int b) {
    comparisons++;
    return a > b;
}

struct BinomialNode {
    int key;
    int degree;
    BinomialNode* parent;
    BinomialNode* child;
    BinomialNode* sibling;

    BinomialNode(int k) : key(k), degree(0), parent(nullptr), child(nullptr), sibling(nullptr) {}
};

class BinomialHeap {
private:
    BinomialNode* head;

    void linkTrees(BinomialNode* y, BinomialNode* z) {
        y->parent = z;
        y->sibling = z->child;
        z->child = y;
        assign(z->degree, z->degree + 1);
    }

    BinomialNode* merge(BinomialNode* h1, BinomialNode* h2) {
        BinomialNode* merged = nullptr;
        BinomialNode** pos = &merged;

        while (h1 && h2) {
            if (compare(h1->degree, h2->degree)) {
                *pos = h2;
                h2 = h2->sibling;
            } else {
                *pos = h1;
                h1 = h1->sibling;
            }
            pos = &((*pos)->sibling);
        }

        *pos = h1 ? h1 : h2;
        return merged;
    }

    BinomialNode* unionHeaps(BinomialNode* h1, BinomialNode* h2) {
        BinomialNode* merged = merge(h1, h2);
        if (!merged) return nullptr;

        BinomialNode* prev = nullptr;
        BinomialNode* curr = merged;
        BinomialNode* next = curr->sibling;

        while (next) {
            if (curr->degree != next->degree || (next->sibling && next->sibling->degree == curr->degree)) {
                prev = curr;
                curr = next;
            } else {
                if (compare(curr->key, next->key)) {
                    curr->sibling = next->sibling;
                    linkTrees(next, curr);
                } else {
                    if (prev) prev->sibling = next;
                    else merged = next;
                    linkTrees(curr, next);
                    curr = next;
                }
            }
            next = curr->sibling;
        }

        return merged;
    }

    BinomialNode* extractMinTree(BinomialNode*& h) {
        BinomialNode* prevMin = nullptr;
        BinomialNode* minNode = h;
        BinomialNode* prev = nullptr;
        BinomialNode* curr = h;

        while (curr) {
            if (compare(minNode->key, curr->key)) {
                minNode = curr;
                prevMin = prev;
            }
            prev = curr;
            curr = curr->sibling;
        }

        if (prevMin) prevMin->sibling = minNode->sibling;
        else h = minNode->sibling;

        return minNode;
    }

    void clearHeap(BinomialNode* node) {
        if (!node) return;
        clearHeap(node->child);
        clearHeap(node->sibling);
        delete node;
    }

public:
    BinomialHeap() : head(nullptr) {}
    
    ~BinomialHeap() {
        clearHeap(head);
    }

    void insert(int key) {
        BinomialNode* newNode = new BinomialNode(key);
        head = unionHeaps(head, newNode);
    }

    int getMin() {
        if (!head) throw std::runtime_error("Heap is empty");

        BinomialNode* minNode = head;
        BinomialNode* curr = head->sibling;

        while (curr) {
            if (compare(minNode->key, curr->key)) {
                minNode = curr;
            }
            curr = curr->sibling;
        }

        return minNode->key;
    }

    int extractMin() {
        if (!head) throw std::runtime_error("Heap is empty");

        BinomialNode* minTree = extractMinTree(head);
        int minKey = minTree->key;

        BinomialNode* child = minTree->child;
        BinomialNode* reversedChild = nullptr;

        while (child) {
            BinomialNode* next = child->sibling;
            child->sibling = reversedChild;
            child->parent = nullptr;
            reversedChild = child;
            child = next;
        }

        head = unionHeaps(head, reversedChild);
        delete minTree;

        return minKey;
    }

    bool isEmpty() {
        return head == nullptr;
    }

    void unionWith(BinomialHeap& other) {
        head = unionHeaps(head, other.head);
        other.head = nullptr; // Transfer ownership
    }

    void printHeap() {
        BinomialNode* curr = head;
        while (curr) {
            std::cout << "Tree with root " << curr->key << " and degree " << curr->degree << std::endl;
            curr = curr->sibling;
        }
    }
};

// Function to reset counters
void resetCounters() {
    assignments = 0;
    comparisons = 0;
}

// Function to get current comparison count
int getComparisons() {
    return comparisons;
}

// Automated test function
void runExperiment(int n, int experiment_num, std::string output_file) {
    std::ofstream file(output_file);
    file << "operation,comparisons\n";
    
    resetCounters();
    
    // Create random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 10000);
    
    // Create two heaps
    BinomialHeap H1, H2;
    
    int operation_count = 0;
    
    // Insert n random elements into H1
    for (int i = 0; i < n; i++) {
        int prev_comparisons = getComparisons();
        H1.insert(dis(gen));
        operation_count++;
        file << operation_count << "," << (getComparisons() - prev_comparisons) << "\n";
    }
    
    // Insert n random elements into H2
    for (int i = 0; i < n; i++) {
        int prev_comparisons = getComparisons();
        H2.insert(dis(gen));
        operation_count++;
        file << operation_count << "," << (getComparisons() - prev_comparisons) << "\n";
    }
    
    // Union H1 and H2
    int prev_comparisons = getComparisons();
    H1.unionWith(H2);
    operation_count++;
    file << operation_count << "," << (getComparisons() - prev_comparisons) << "\n";
    
    // Extract 2n elements
    std::vector<int> extracted;
    for (int i = 0; i < 2 * n; i++) {
        prev_comparisons = getComparisons();
        int min_val = H1.extractMin();
        extracted.push_back(min_val);
        operation_count++;
        file << operation_count << "," << (getComparisons() - prev_comparisons) << "\n";
    }
    
    // Verify that extracted sequence is sorted
    bool is_sorted = std::is_sorted(extracted.begin(), extracted.end());
    
    // Verify that heap is empty
    bool is_empty = H1.isEmpty();
    
    file.close();
    
    // Output summary
    std::cout << "Experiment " << experiment_num << " (n=" << n << "):\n";
    std::cout << "Total comparisons: " << getComparisons() << "\n";
    std::cout << "Average comparisons per operation: " << (double)getComparisons() / operation_count << "\n";
    std::cout << "Extracted sequence is sorted: " << (is_sorted ? "YES" : "NO") << "\n";
    std::cout << "Heap is empty after extraction: " << (is_empty ? "YES" : "NO") << "\n";
    std::cout << "Results saved to: " << output_file << "\n\n";
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "Usage: " << argv[0] << " <n> <experiment_number> [output_file]\n";
        std::cout << "Or use interactive mode: " << argv[0] << " interactive\n";
        return 1;
    }
    
    if (std::string(argv[1]) == "interactive") {
        // Original interactive mode
        BinomialHeap heap;
        int command, key;
        while (true) {
            std::cout << "Enter command (1 key: insert, 2: getMin, 3: extractMin, 4: printHeap, 5: stats, 0: exit): ";
            std::cin >> command;

            switch (command) {
                case 1: {
                    std::cin >> key;
                    heap.insert(key);
                    break;
                }
                case 2: {
                    try {
                        std::cout << "Minimum: " << heap.getMin() << std::endl;
                    } catch (const std::runtime_error& e) {
                        std::cout << e.what() << std::endl;
                    }
                    break;
                }
                case 3: {
                    try {
                        std::cout << "Extracted Minimum: " << heap.extractMin() << std::endl;
                    } catch (const std::runtime_error& e) {
                        std::cout << e.what() << std::endl;
                    }
                    break;
                }
                case 4: {
                    heap.printHeap();
                    break;
                }
                case 5: {
                    std::cout << "Assignments: " << assignments << ", Comparisons: " << comparisons << std::endl;
                    break;
                }
                case 0: {
                    return 0;
                }
                default: {
                    std::cout << "Invalid command. Try again." << std::endl;
                    break;
                }
            }
        }
    } else {
        // Automated experiment mode
        int n = std::atoi(argv[1]);
        int exp_num = std::atoi(argv[2]);
        std::string output_file = (argc > 3) ? argv[3] : ("experiment_n" + std::to_string(n) + "_exp" + std::to_string(exp_num) + ".csv");
        
        runExperiment(n, exp_num, output_file);
    }
    
    return 0;
}