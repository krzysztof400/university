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

class Splay_Tree {
private:
    struct Node {
        int data;
        Node* left;
        Node* right;
        Node* parent;

        Node(int value)
            : data(value), left(nullptr), right(nullptr), parent(nullptr)
        {}
    };

    Node* root = nullptr;

    // Help for ASCII-art print
    char* left_trace;
    char* right_trace;
    static const int MAX_DEPTH = 1000000;

    // ------------------------
    // ROTATIONS
    // ------------------------
    void rotateLeft(Node* pt) {
        Node* R = pt->right;
        pt->right = R->left;
        if (R->left) R->left->parent = pt;

        R->parent = pt->parent;
        if (!pt->parent)
            root = R;
        else if (pt == pt->parent->left)
            pt->parent->left = R;
        else
            pt->parent->right = R;

        R->left = pt;
        pt->parent = R;
    }

    void rotateRight(Node* pt) {
        Node* L = pt->left;
        pt->left = L->right;
        if (L->right) L->right->parent = pt;

        L->parent = pt->parent;
        if (!pt->parent)
            root = L;
        else if (pt == pt->parent->left)
            pt->parent->left = L;
        else
            pt->parent->right = L;

        L->right = pt;
        pt->parent = L;
    }

    // ------------------------
    // SPLAY OPERATION - moves node to root
    // ------------------------
    void splay(Node* pt) {
        while (pt->parent) {
            Node* parent = pt->parent;
            Node* grand = parent->parent;

            if (!grand) {
                // Zig step - parent is root
                if (pt == parent->left) {
                    rotateRight(parent);
                } else {
                    rotateLeft(parent);
                }
            } else if ((pt == parent->left) == (parent == grand->left)) {
                // Zig-Zig step - pt and parent are both left or both right children
                if (pt == parent->left) {
                    rotateRight(grand);
                    rotateRight(parent);
                } else {
                    rotateLeft(grand);
                    rotateLeft(parent);
                }
            } else {
                // Zig-Zag step - pt and parent are opposite children
                if (pt == parent->left) {
                    rotateRight(parent);
                    rotateLeft(grand);
                } else {
                    rotateLeft(parent);
                    rotateRight(grand);
                }
            }
        }
        root = pt;
    }

    // ------------------------
    // BST SEARCH (with splay)
    // ------------------------
    Node* searchHelper(int value) {
        Node* current = root;
        Node* lastNode = nullptr;

        while (current) {
            lastNode = current;
            if (value == current->data) {
                splay(current);
                return current;
            } else if (value < current->data) {
                current = current->left;
            } else {
                current = current->right;
            }
        }

        // Value not found, splay the last accessed node
        if (lastNode) {
            splay(lastNode);
        }
        return nullptr;
    }

    // ------------------------
    // BST INSERT (with splay)
    // ------------------------
    Node* insertHelper(Node*& node, Node* parent, int value) {
        if (!node) {
            Node* newNode = new Node(value);
            newNode->parent = parent;
            node = newNode;
            return newNode;
        }
        
        if (value < node->data) {
            return insertHelper(node->left, node, value);
        } else if (value > node->data) {
            return insertHelper(node->right, node, value);
        }
        // duplicate: return existing node to splay it
        return node;
    }

    // ------------------------
    // FIND MINIMUM NODE
    // ------------------------
    Node* findMin(Node* node) {
        if (!node) return nullptr;
        while (node->left) {
            node = node->left;
        }
        return node;
    }

    // ------------------------
    // FIND MAXIMUM NODE
    // ------------------------
    Node* findMax(Node* node) {
        if (!node) return nullptr;
        while (node->right) {
            node = node->right;
        }
        return node;
    }

    // ------------------------
    // JOIN TWO SUBTREES
    // ------------------------
    Node* join(Node* left, Node* right) {
        if (!left) return right;
        if (!right) return left;

        // Find and splay the maximum element in left subtree
        Node* maxLeft = findMax(left);
        
        // Temporarily set left as root to splay maxLeft
        root = left;
        splay(maxLeft);
        
        // Now maxLeft is root of left subtree and has no right child
        maxLeft->right = right;
        if (right) right->parent = maxLeft;
        
        return maxLeft;
    }

    // ------------------------
    // HEIGHT
    // ------------------------
    int getHeightHelper(Node* node) {
        if (!node) return 0;
        return 1 + std::max(getHeightHelper(node->left),
                            getHeightHelper(node->right));
    }

    // ------------------------
    // DELETE TREE (destructor)
    // ------------------------
    void deleteTree(Node* node) {
        if (!node) return;
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }

    // ------------------------
    // PRINT (ASCII-art)
    // ------------------------
    void printHelper(Node* node, int depth, char prefix) {
        if (!node) return;
        if (node->left)
            printHelper(node->left, depth+1, '/');

        if (prefix == '/') left_trace[depth-1] = '|';
        if (prefix == '\\') right_trace[depth-1] = ' ';

        if (depth == 0) printf("-");
        if (depth > 0) printf(" ");
        for (int i = 0; i < depth-1; i++) {
            if (left_trace[i]=='|' || right_trace[i]=='|')
                printf("| ");
            else
                printf("  ");
        }
        if (depth>0) printf("%c-", prefix);
        printf("[%d]\n", node->data);

        left_trace[depth] = ' ';
        if (node->right) {
            right_trace[depth] = '|';
            printHelper(node->right, depth+1, '\\');
        }
    }

public:
    Splay_Tree() {
        left_trace  = new char[MAX_DEPTH];
        right_trace = new char[MAX_DEPTH];
        std::fill(left_trace,  left_trace+MAX_DEPTH,  ' ');
        std::fill(right_trace, right_trace+MAX_DEPTH, ' ');
    }

    ~Splay_Tree() {
        delete[] left_trace;
        delete[] right_trace;
        deleteTree(root);
    }

    // ------------------------
    // PUBLIC INSERT
    // ------------------------
    void insertNode(int value) {
        Node* inserted = insertHelper(root, nullptr, value);
        if (inserted) {
            splay(inserted);
        }
    }

    // ------------------------
    // PUBLIC SEARCH
    // ------------------------
    bool searchNode(int value) {
        Node* found = searchHelper(value);
        return found != nullptr;
    }

    // ------------------------
    // PUBLIC DELETE (with splay)
    // ------------------------
    void deleteNode(int value) {
        Node* nodeToDelete = searchHelper(value);
        
        if (!nodeToDelete) {
            printf("Value %d not found.\n", value);
            return;
        }

        // nodeToDelete is now at root due to splay in searchHelper
        Node* leftSubtree = nodeToDelete->left;
        Node* rightSubtree = nodeToDelete->right;

        // Disconnect children from parent
        if (leftSubtree) leftSubtree->parent = nullptr;
        if (rightSubtree) rightSubtree->parent = nullptr;

        delete nodeToDelete;

        // Join the two subtrees
        root = join(leftSubtree, rightSubtree);
        if (root) root->parent = nullptr;
    }

    // ------------------------
    // PUBLIC ACCESS (splay without searching)
    // ------------------------
    void accessNode(int value) {
        searchHelper(value);
    }

    // ------------------------
    // GET MINIMUM (with splay)
    // ------------------------
    int getMin() {
        if (!root) {
            printf("Tree is empty.\n");
            return -1;
        }
        Node* minNode = findMin(root);
        splay(minNode);
        return minNode->data;
    }

    // ------------------------
    // GET MAXIMUM (with splay)
    // ------------------------
    int getMax() {
        if (!root) {
            printf("Tree is empty.\n");
            return -1;
        }
        Node* maxNode = findMax(root);
        splay(maxNode);
        return maxNode->data;
    }

    // ------------------------
    // OTHER UTILITIES
    // ------------------------
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
        printf("Total assignments: %d\n", assignments);
        printf("Total comparisons: %d\n", comparisons);
    }

    void resetStats() {
        assignments = comparisons = 0;
    }

    void printRoot() {
        if (root) {
            printf("Root: %d\n", root->data);
        } else {
            printf("Tree is empty.\n");
        }
    }
};

int main(int argc, char* argv[]) {
    Splay_Tree myTree;
    bool printsHelp = (argc>1 && argv[1][0]=='h');

    if (printsHelp) {
        printf("Splay Tree Commands:\n"
               " i<n>  Insert n\n"
               " d<n>  Delete n\n"
               " s<n>  Search n (access)\n"
               " a<n>  Access n (splay to root)\n"
               " p     Print tree\n"
               " h     Height\n"
               " r     Show root\n"
               " min   Get minimum\n"
               " max   Get maximum\n"
               " stat  Show stats\n"
               " rst   Reset stats\n"
               " q     Quit\n");
    }

    while (true) {
        if (printsHelp) printf("Cmd> ");
        char cmd[10];
        if (scanf("%s", cmd) != 1) break;
        if (cmd[0] == 'q') break;

        int v;
        switch (cmd[0]) {
            case 'i': 
                if (scanf("%d", &v) == 1) {
                    myTree.insertNode(v); 
                    if (printsHelp) printf("Inserted %d\n", v);
                }
                break;
            case 'd': 
                if (scanf("%d", &v) == 1) {
                    myTree.deleteNode(v);
                }
                break;
            case 's':
                if (cmd[1] == 't' && cmd[2] == 'a' && cmd[3] == 't') {
                    // "stat" command
                    myTree.printStats();
                } else {
                    // "s<n>" search command
                    if (scanf("%d", &v) == 1) {
                        bool found = myTree.searchNode(v);
                        if (printsHelp) {
                            printf("Value %d %s\n", v, found ? "found" : "not found");
                        }
                    }
                }
                break;
            case 'a': 
                if (scanf("%d", &v) == 1) {
                    myTree.accessNode(v);
                    if (printsHelp) printf("Accessed %d\n", v);
                }
                break;
            case 'p': myTree.printTree(); break;
            case 'h': printf("Height: %d\n", myTree.getHeight()); break;
            case 'r': 
                if (cmd[1] == 's' && cmd[2] == 't') {
                    // "rst" reset command
                    myTree.resetStats(); 
                    printf("Stats reset.\n");
                } else {
                    // "r" root command
                    myTree.printRoot();
                }
                break;
            case 'm':
                if (cmd[1] == 'i' && cmd[2] == 'n') {
                    printf("Minimum: %d\n", myTree.getMin());
                } else if (cmd[1] == 'a' && cmd[2] == 'x') {
                    printf("Maximum: %d\n", myTree.getMax());
                }
                break;
            default: printf("Unknown cmd '%s'\n", cmd);
        }
    }
    return 0;
}