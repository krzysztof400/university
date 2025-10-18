#include <iostream>
#include <memory>
#include <string>
#include <functional>
#include <stdexcept>

template <typename T>
class BinarySearchTree {
private:
    struct Node {
        T value;
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;
        Node(T val) : value(val), left(nullptr), right(nullptr) {}
    };
    std::unique_ptr<Node> root;

    void insertRecursive(std::unique_ptr<Node>& node, T value) {
        if (!node) {
            node = std::make_unique<Node>(value);
        } else if (value < node->value) {
            insertRecursive(node->left, value);
        } else if (value > node->value) {
            insertRecursive(node->right, value);
        }
    }

    Node* deleteRecursive(std::unique_ptr<Node>& node, T value) {
        if (!node) return nullptr;

        if (value < node->value) {
            node->left.reset(deleteRecursive(node->left, value));
        } else if (value > node->value) {
            node->right.reset(deleteRecursive(node->right, value));
        } else {
            if (!node->left) return node->right.release();
            if (!node->right) return node->left.release();
            node->value = findMinValue(node->right.get());
            node->right.reset(deleteRecursive(node->right, node->value));
        }
        return node.release();
    }

    T findMinValue(Node* node) {
        while (node && node->left) {
            node = node->left.get();
        }
        return node->value;
    }

    void printRecursive(Node* node, std::string prefix, bool isTail) {
        if (node) {
            std::cout << prefix << (isTail ? "└── " : "├── ") << node->value << std::endl;
            printRecursive(node->left.get(), prefix + (isTail ? "    " : "│   "), node->right == nullptr);
            printRecursive(node->right.get(), prefix + (isTail ? "    " : "│   "), true);
        }
    }

public:
    void insert(T value) {
        if (!root) {
            root = std::make_unique<Node>(value);
        } else {
            insertRecursive(root, value);
        }
    }

    void deleteNode(T value) {
        root.reset(deleteRecursive(root, value));
    }

    bool search(T value) {
        Node* current = root.get();
        while (current) {
            if (value == current->value) return true;
            current = value < current->value ? current->left.get() : current->right.get();
        }
        return false;
    }

    void print() {
        printRecursive(root.get(), "", true);
    }
};
