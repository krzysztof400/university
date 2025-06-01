#include <stdio.h>
#include <algorithm>

int assignments = 0;
int comparisons = 0;

void assign(int &a, int b){
    a = b;
    assignments++;
}

bool compare(int a, int b){
    comparisons++;
    return a > b;
}

class RB_Tree {
private:
    struct Node {
        int data;
        bool red;     // true = red, false = black
        Node* left;
        Node* right;
        Node* parent;

        Node(int value)
            : data(value), red(true),
              left(nullptr), right(nullptr), parent(nullptr)
        {}
    };

    Node* root = nullptr;

    // Help for ASCII-art print
    char* left_trace;
    char* right_trace;
    static const int MAX_DEPTH = 1000;

    // ------------------------
    // ROTATIONS
    // ------------------------
    void rotateLeft(Node* pt) {
        if (!pt || !pt->right) return;  // Safety check
        
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
        if (!pt || !pt->left) return;  // Safety check
        
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
    // FIX INSERTION
    // ------------------------
    void fixInsertion(Node* pt) {
        if (!pt) return;  // Safety check
        
        Node* parent = nullptr;
        Node* grand = nullptr;

        while (pt != root && pt->red && pt->parent && pt->parent->red) {
            parent = pt->parent;
            grand = parent->parent;
            
            if (!grand) break;  // Safety check

            // parent is left child of grandparent
            if (parent == grand->left) {
                Node* uncle = grand->right;
                // case 1: uncle is red
                if (uncle && uncle->red) {
                    grand->red  = true;
                    parent->red = false;
                    uncle->red  = false;
                    pt = grand;
                } else {
                    // case 2: pt is right child
                    if (pt == parent->right) {
                        rotateLeft(parent);
                        pt = parent;
                        parent = pt->parent;
                    }
                    // case 3: pt is left child
                    rotateRight(grand);
                    std::swap(parent->red, grand->red);
                    pt = parent;
                }
            }
            // parent is right child of grandparent
            else {
                Node* uncle = grand->left;
                // case 1: uncle is red
                if (uncle && uncle->red) {
                    grand->red  = true;
                    parent->red = false;
                    uncle->red  = false;
                    pt = grand;
                } else {
                    // case 2: pt is left child
                    if (pt == parent->left) {
                        rotateRight(parent);
                        pt = parent;
                        parent = pt->parent;
                    }
                    // case 3: pt is right child
                    rotateLeft(grand);
                    std::swap(parent->red, grand->red);
                    pt = parent;
                }
            }
        }
        if (root) root->red = false;  // Safety check
    }

    // ------------------------
    // BST INSERT, returns inserted node
    // ------------------------
    Node* insertHelper(Node*& node, Node* parent, int value) {
        if (!node) {
            Node* newNode = new Node(value);
            assign(newNode->data, value);  // Track assignment
            newNode->parent = parent;
            node = newNode;
            return newNode;
        }
        
        // Use compare function for all comparisons
        if (!compare(node->data, value) && !compare(value, node->data)) {
            // Equal values - duplicate, do nothing
            return nullptr;
        } else if (compare(node->data, value)) {
            // node->data > value, go left
            return insertHelper(node->left, node, value);
        } else {
            // value > node->data, go right
            return insertHelper(node->right, node, value);
        }
    }

    // ------------------------
    // FIND MINIMUM NODE
    // ------------------------
    Node* findMin(Node* node) {
        if (!node) return nullptr;  // Safety check
        while (node->left) {
            node = node->left;
        }
        return node;
    }

    // ------------------------
    // REPLACE NODE IN PARENT
    // ------------------------
    void replaceNode(Node* oldNode, Node* newNode) {
        if (!oldNode) return;  // Safety check
        
        if (!oldNode->parent) {
            root = newNode;
        } else if (oldNode == oldNode->parent->left) {
            oldNode->parent->left = newNode;
        } else {
            oldNode->parent->right = newNode;
        }
        if (newNode) {
            newNode->parent = oldNode->parent;
        }
    }

    // ------------------------
    // FIX DELETION
    // ------------------------
    void fixDeletion(Node* pt) {
        if (!pt) return;  // Safety check
        
        while (pt != root && (!pt || !pt->red)) {
            if (!pt->parent) break;  // Safety check
            
            if (pt == pt->parent->left) {
                Node* sibling = pt->parent->right;
                
                // Case 1: sibling is red
                if (sibling && sibling->red) {
                    sibling->red = false;
                    pt->parent->red = true;
                    rotateLeft(pt->parent);
                    sibling = pt->parent->right;
                }
                
                if (!sibling) break;  // Safety check
                
                // Case 2: sibling's children are both black
                if ((!sibling->left || !sibling->left->red) && 
                    (!sibling->right || !sibling->right->red)) {
                    sibling->red = true;
                    pt = pt->parent;
                } else {
                    // Case 3: sibling's right child is black
                    if (!sibling->right || !sibling->right->red) {
                        if (sibling->left) sibling->left->red = false;
                        sibling->red = true;
                        rotateRight(sibling);
                        sibling = pt->parent->right;
                    }
                    
                    // Case 4: sibling's right child is red
                    if (sibling) {
                        sibling->red = pt->parent->red;
                        pt->parent->red = false;
                        if (sibling->right) sibling->right->red = false;
                        rotateLeft(pt->parent);
                    }
                    pt = root;
                }
            } else {
                Node* sibling = pt->parent->left;
                
                // Case 1: sibling is red
                if (sibling && sibling->red) {
                    sibling->red = false;
                    pt->parent->red = true;
                    rotateRight(pt->parent);
                    sibling = pt->parent->left;
                }
                
                if (!sibling) break;  // Safety check
                
                // Case 2: sibling's children are both black
                if ((!sibling->left || !sibling->left->red) && 
                    (!sibling->right || !sibling->right->red)) {
                    sibling->red = true;
                    pt = pt->parent;
                } else {
                    // Case 3: sibling's left child is black
                    if (!sibling->left || !sibling->left->red) {
                        if (sibling->right) sibling->right->red = false;
                        sibling->red = true;
                        rotateLeft(sibling);
                        sibling = pt->parent->left;
                    }
                    
                    // Case 4: sibling's left child is red
                    if (sibling) {
                        sibling->red = pt->parent->red;
                        pt->parent->red = false;
                        if (sibling->left) sibling->left->red = false;
                        rotateRight(pt->parent);
                    }
                    pt = root;
                }
            }
        }
        if (pt) pt->red = false;
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
        if (!node || depth >= MAX_DEPTH-1) return;  // Safety check for depth
        
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
        printf("[%d:%c]\n",
               node->data,
               node->red ? 'R' : 'B');

        left_trace[depth] = ' ';
        if (node->right) {
            right_trace[depth] = '|';
            printHelper(node->right, depth+1, '\\');
        }
    }

public:
    RB_Tree() {
        left_trace  = new char[MAX_DEPTH];
        right_trace = new char[MAX_DEPTH];
        std::fill(left_trace,  left_trace+MAX_DEPTH,  ' ');
        std::fill(right_trace, right_trace+MAX_DEPTH, ' ');
    }

    ~RB_Tree() {
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
            fixInsertion(inserted);
        }
    }

    // ------------------------
    // PUBLIC DELETE (with RB fix-up)
    // ------------------------
    void deleteNode(int value) {
        Node* nodeToDelete = root;
        
        // Find the node to delete using compare function
        while (nodeToDelete) {
            if (!compare(nodeToDelete->data, value) && !compare(value, nodeToDelete->data)) {
                // Found the node
                break;
            } else if (compare(nodeToDelete->data, value)) {
                // nodeToDelete->data > value, go left
                nodeToDelete = nodeToDelete->left;
            } else {
                // value > nodeToDelete->data, go right
                nodeToDelete = nodeToDelete->right;
            }
        }
        
        if (!nodeToDelete) {
            printf("Value %d not found.\n", value);
            return;
        }

        Node* deletedNode = nodeToDelete;
        Node* replacementNode = nullptr;
        bool deletedOriginalColor = deletedNode->red;

        if (!nodeToDelete->left) {
            replacementNode = nodeToDelete->right;
            replaceNode(nodeToDelete, nodeToDelete->right);
        } else if (!nodeToDelete->right) {
            replacementNode = nodeToDelete->left;
            replaceNode(nodeToDelete, nodeToDelete->left);
        } else {
            // Node has two children - find successor
            deletedNode = findMin(nodeToDelete->right);
            if (!deletedNode) {
                delete nodeToDelete;
                return;  // Safety check
            }
            
            deletedOriginalColor = deletedNode->red;
            replacementNode = deletedNode->right;

            if (deletedNode->parent == nodeToDelete) {
                if (replacementNode) replacementNode->parent = deletedNode;
            } else {
                replaceNode(deletedNode, deletedNode->right);
                deletedNode->right = nodeToDelete->right;
                if (deletedNode->right) deletedNode->right->parent = deletedNode;
            }

            replaceNode(nodeToDelete, deletedNode);
            deletedNode->left = nodeToDelete->left;
            if (deletedNode->left) deletedNode->left->parent = deletedNode;
            deletedNode->red = nodeToDelete->red;
            
            // Use assign for data replacement
            assign(deletedNode->data, nodeToDelete->data);
        }

        delete nodeToDelete;

        // Fix Red-Black properties if a black node was deleted
        if (!deletedOriginalColor && replacementNode) {
            fixDeletion(replacementNode);
        }
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
};

int main(int argc, char* argv[]) {
    RB_Tree myTree;
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