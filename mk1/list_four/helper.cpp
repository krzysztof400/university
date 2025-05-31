#include <iostream>

enum Color { RED, BLACK };

struct Node {
    int data;
    bool color;
    Node *left, *right, *parent;

    Node(int data) : data(data), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
};

class RBTree {
private:
    Node *root;

    void rotateLeft(Node *&x) {
        Node *y = x->right;
        x->right = y->left;
        if (y->left != nullptr)
            y->left->parent = x;

        y->parent = x->parent;
        if (x->parent == nullptr)
            root = y;
        else if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;

        y->left = x;
        x->parent = y;
    }

    void rotateRight(Node *&x) {
        Node *y = x->left;
        x->left = y->right;
        if (y->right != nullptr)
            y->right->parent = x;

        y->parent = x->parent;
        if (x->parent == nullptr)
            root = y;
        else if (x == x->parent->right)
            x->parent->right = y;
        else
            x->parent->left = y;

        y->right = x;
        x->parent = y;
    }

    void fixInsert(Node *&x) {
        Node *parent = nullptr;
        Node *grandparent = nullptr;

        while (x != root && x->color == RED && x->parent->color == RED) {
            parent = x->parent;
            grandparent = parent->parent;

            // Case A: Parent is left child of grandparent
            if (parent == grandparent->left) {
                Node *uncle = grandparent->right;

                // Case 1: Uncle is red → recolor
                if (uncle != nullptr && uncle->color == RED) {
                    grandparent->color = RED;
                    parent->color = BLACK;
                    uncle->color = BLACK;
                    x = grandparent;
                } else {
                    // Case 2: Node is right child → rotate left
                    if (x == parent->right) {
                        rotateLeft(parent);
                        x = parent;
                        parent = x->parent;
                    }

                    // Case 3: Node is left child → rotate right
                    rotateRight(grandparent);
                    std::swap(parent->color, grandparent->color);
                    x = parent;
                }
            }
            // Case B: Parent is right child of grandparent
            else {
                Node *uncle = grandparent->left;

                if (uncle != nullptr && uncle->color == RED) {
                    grandparent->color = RED;
                    parent->color = BLACK;
                    uncle->color = BLACK;
                    x = grandparent;
                } else {
                    if (x == parent->left) {
                        rotateRight(parent);
                        x = parent;
                        parent = x->parent;
                    }

                    rotateLeft(grandparent);
                    std::swap(parent->color, grandparent->color);
                    x = parent;
                }
            }
        }

        root->color = BLACK;
    }

public:
    RBTree() : root(nullptr) {}

    void insert(int data) {
        Node *node = new Node(data);
        Node *y = nullptr;
        Node *x = root;

        // BST insert
        while (x != nullptr) {
            y = x;
            if (node->data < x->data)
                x = x->left;
            else
                x = x->right;
        }

        node->parent = y;
        if (y == nullptr)
            root = node;
        else if (node->data < y->data)
            y->left = node;
        else
            y->right = node;

        // Fix the red-black tree
        fixInsert(node);
    }

    void inorderHelper(Node *node) {
        if (node == nullptr) return;
        inorderHelper(node->left);
        std::cout << node->data << (node->color == RED ? "R " : "B ");
        inorderHelper(node->right);
    }

    void printInOrder() {
        inorderHelper(root);
        std::cout << std::endl;
    }
};

int main() {
    RBTree tree;
    tree.insert(10);
    tree.insert(20);
    tree.insert(30);
    tree.insert(15);
    tree.insert(25);

    std::cout << "In-order traversal of the Red-Black Tree: ";
    tree.printInOrder();

    return 0;
}