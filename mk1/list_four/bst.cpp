#include <stdio.h>
#include <algorithm>

long long assignments = 0;
long long comparisons = 0;

void assign(int &a, int b){
    a = b;
    assignments++;
}

bool compare(int a, int b){
    comparisons++;
    return a > b;
}

class BST {
private:
    struct Node {
        int data;
        Node* left;
        Node* right;

        Node(int value) : data(value), left(nullptr), right(nullptr) {}
    };

    Node* root = nullptr;

    char* left_trace;
    char* right_trace;
    const static int MAX_DEPTH = 1000;

    void printHelper(Node* node, int depth, char prefix) {
        if (!node || depth >= MAX_DEPTH-1) return;
        
        if (node->left) {
            printHelper(node->left, depth + 1, '/');
        }
        
        if (prefix == '/') left_trace[depth - 1] = '|';
        if (prefix == '\\') right_trace[depth - 1] = ' ';
        
        if (depth == 0) printf("-");
        if (depth > 0) printf(" ");
        
        for (int i = 0; i < depth - 1; i++) {
            if (left_trace[i] == '|' || right_trace[i] == '|') {
                printf("| ");
            } else {
                printf("  ");
            }
        }
        
        if (depth > 0) printf("%c-", prefix);
        printf("[%d]\n", node->data);
        
        left_trace[depth] = ' ';
        if (node->right) {
            right_trace[depth] = '|';
            printHelper(node->right, depth + 1, '\\');
        }
    }

    void insertHelper(Node*& node, int value) {
        if (node == nullptr) {
            node = new Node(value);
            assign(node->data, value);  // Track assignment
        } else if (!compare(node->data, value) && !compare(value, node->data)) {
            // Equal values - do nothing (no duplicates)
            return;
        } else if (compare(node->data, value)) {
            // node->data > value, go left
            insertHelper(node->left, value);
        } else {
            // value > node->data, go right
            insertHelper(node->right, value);
        }
    }

    int getHeightHelper(Node* node) {
        if (node == nullptr) return 0;
        return 1 + std::max(getHeightHelper(node->left), getHeightHelper(node->right));
    }

    void deleteTree(Node* node) {
        if (node == nullptr) return;
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }

public:
    BST() {
        left_trace = new char[MAX_DEPTH];
        right_trace = new char[MAX_DEPTH];
        std::fill(left_trace, left_trace + MAX_DEPTH, ' ');
        std::fill(right_trace, right_trace + MAX_DEPTH, ' ');
    }

    ~BST() {
        delete[] left_trace;
        delete[] right_trace;
        deleteTree(root);
    }

    void deleteNode(int value) {
        Node* parent = nullptr;
        Node* current = root;
    
        // Find the node to delete
        while (current != nullptr) {
            if (!compare(current->data, value) && !compare(value, current->data)) {
                // Found the node
                break;
            }
            parent = current;
            if (compare(current->data, value)) {
                // current->data > value, go left
                current = current->left;
            } else {
                // value > current->data, go right
                current = current->right;
            }
        }
    
        if (current == nullptr) {
            printf("Value %d not found.\n", value);
            return;
        }
    
        // Node with one child or no child
        if (current->left == nullptr || current->right == nullptr) {
            Node* newCurrent = (current->left != nullptr) ? current->left : current->right;
    
            if (parent == nullptr) {
                root = newCurrent;
            } else if (parent->left == current) {
                parent->left = newCurrent;
            } else {
                parent->right = newCurrent;
            }
    
            delete current; 
        } else { 
            // Node with two children
            Node* successorParent = current;
            Node* successor = current->right;
    
            while (successor->left != nullptr) {
                successorParent = successor;
                successor = successor->left;
            }
            
            assign(current->data, successor->data);
            
            if (successorParent->left == successor) {
                successorParent->left = successor->right;
            } else {
                successorParent->right = successor->right;
            }
    
            delete successor; 
        }
    }    

    void insertNode(int value) {
        insertHelper(root, value);
    }

    int getHeight() {
        return getHeightHelper(root);
    }

    void printTree() {
        if (!root) {
            printf("Tree is empty.\n");
            return;
        }
        printHelper(root, 0, '-');
    }

    void printStats() {
        printf("Total assignments: %lld\n", assignments);
        printf("Total comparisons: %lld\n", comparisons);
    }

    void resetStats() {
        assignments = 0;
        comparisons = 0;
    }
};

int main(int argc, char* argv[]) {
    BST myTree;
    bool printsHelp = (argc>1 && argv[1][0]=='h');

    if (printsHelp) {
        printf("Commands:\n"
               " i<n>  Insert n\n"
               " d<n>  Delete n\n"
               " p     Print tree\n"
               " h     Height\n"
               " s     Stats\n"
               " r     Reset stats\n"
               " q     Quit\n");
    }

    while (true) {
        if (printsHelp) printf("Cmd> ");
        char cmd;
        if (scanf(" %c", &cmd)!=1) break;
        if (cmd=='q') break;

        int v;
        switch (cmd) {
            case 'i': 
                if (scanf("%d",&v)==1) {
                    myTree.insertNode(v); 
                    if (printsHelp) printf("Inserted %d\n", v);
                }
                break;
            case 'd': 
                if (scanf("%d",&v)==1) {
                    myTree.deleteNode(v);
                }
                break;
            case 'p': myTree.printTree(); break;
            case 'h': printf("Height: %d\n", myTree.getHeight()); break;
            case 's': myTree.printStats(); break;
            case 'r': myTree.resetStats(); printf("Stats reset.\n"); break;
            default: printf("Unknown cmd '%c'\n", cmd);
        }
    }
    return 0;
}