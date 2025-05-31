#include <stdio.h>
#include <algorithm> 

int assignments = 0;
int comparisons = 0;

void assign(int &a, int &b){
    a = b;
    assignments++;
}

bool compare(int &a, int &b){
    comparisons++;
    return a > b;
}

class RB_Tree {
private:
    struct Node {
        int data;
        bool red;
        Node* left;
        Node* right;
        Node* parent;

        Node(int value) : data(value), left(nullptr), right(nullptr), parent(nullptr) {}
    };

    Node* root = nullptr;

    char* left_trace;
    char* right_trace;
    const static int MAX_DEPTH = 1000000;

    void printHelper(Node* root, int depth, char prefix) {
        if (root == nullptr) return;
        if (root->left != nullptr) {
            printHelper(root->left, depth + 1, '/');
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
        printf("[%d]\n", root->data);
        left_trace[depth] = ' ';
        if (root->right != nullptr) {
            right_trace[depth] = '|';
            printHelper(root->right, depth + 1, '\\');
        }
    }

    void insertHelper(Node*& node, int value) {
        if (node == nullptr) {
            node = new Node(value);
        } else if (value < node->data) {
            if (!compare(value, node->data) && value != node->data) {
                insertHelper(node->left, value);
            }
        } else if (compare(value, node->data)) {
            insertHelper(node->right, value);
        }
        // If value == node->data, we don't insert (no duplicates)
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
    RB_Tree() {
        left_trace = new char[MAX_DEPTH];
        right_trace = new char[MAX_DEPTH];
        for (int i = 0; i < MAX_DEPTH; i++) {
            left_trace[i] = ' ';
            right_trace[i] = ' ';
        }
    }

    ~RB_Tree() {
        delete[] left_trace;
        delete[] right_trace;
        deleteTree(root);
    }

    void deleteNode(int value) {
        Node* parent = nullptr;
        Node* current = root;
    
        while (current != nullptr && current->data != value) {
            parent = current;
            if (!compare(value, current->data) && value != current->data) {
                current = current->left;
            } else if (compare(value, current->data)) {
                current = current->right;
            } else {
                break; // Found the node
            }
        }
    
        if (current == nullptr) {
            printf("Value %d not found in the tree.\n", value);
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
        } else { // Node with two children
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
        printHelper(root, 0, '-');
    }

    void printStats() {
        printf("Total assignments: %d\n", assignments);
        printf("Total comparisons: %d\n", comparisons);
    }

    void resetStats() {
        assignments = 0;
        comparisons = 0;
    }
};

int main(int argc, char* argv[]) {
    RB_Tree myRB_Tree;

    bool printsHelp = false;
    if (argc > 1 && argv[1][0] == 'h') {
        printsHelp = true;
    }

    while (true) {
        int value;
        if(printsHelp) {
            printf("Choose operation:\n");
            printf("i - Insert a node\n");
            printf("d - Delete a node\n");
            printf("p - Print the tree\n");
            printf("h - Get the height of the tree\n");
            printf("s - Show statistics (assignments/comparisons)\n");
            printf("r - Reset statistics\n");
            printf("q - Quit\n");
            printf("example: i5\n");
            printf("Enter command: ");
        }
        char command;
        scanf(" %c", &command);
        if (command == 'q') {
            break;
        } else if (command == 'i') {
            scanf("%d", &value);
            myRB_Tree.insertNode(value);
        } else if (command == 'd') {
            scanf("%d", &value);
            myRB_Tree.deleteNode(value);
        } else if (command == 'p') {
            myRB_Tree.printTree();
        } else if (command == 'h') {
            printf("Height of the tree: %d\n", myRB_Tree.getHeight());
        } else if (command == 's') {
            myRB_Tree.printStats();
        } else if (command == 'r') {
            myRB_Tree.resetStats();
            printf("Statistics reset.\n");
        } else {
            printf("Unknown command. Please try again.\n");
        }
    }

    return 0;
}