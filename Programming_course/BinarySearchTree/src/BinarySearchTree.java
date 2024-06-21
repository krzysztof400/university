public class BinarySearchTree<T extends Comparable<T>>{
    private class Node {
        private T value;
        private Node left;
        private Node right;

        public Node(T value) {
            this.value = value;
        }
    }
    public Node root;

    public void insert(T value) {
        if (root == null) {
            root = new Node(value);
        } else {
            insertRecursive(root, value);
        }
    }

    public void delete(T value) {
        root = deleteRecursive(root, value);
    }

    public boolean search(T value) {
        Node current = root;
        while (current != null) {
            if (value.compareTo(current.value) == 0) {
                return true;
            } else if (value.compareTo(current.value) < 0) {
                current = current.left;
            } else {
                current = current.right;
            }
        }
        return false;
    }

    public void print() {
        printRecursive(root, "", true);
    }

    private void insertRecursive(Node node, T value) {
        if (value.compareTo(node.value) < 0) {
            if (node.left == null) {
                node.left = new Node(value);
            } else {
                insertRecursive(node.left, value);
            }
        } else if (value.compareTo(node.value) > 0) {
            if (node.right == null) {
                node.right = new Node(value);
            } else {
                insertRecursive(node.right, value);
            }
        }
    }

    private Node deleteRecursive(Node node, T value) {
        if (node == null) {
            return null;
        }

        if (value.compareTo(node.value) < 0) {
            node.left = deleteRecursive(node.left, value);
        } else if (value.compareTo(node.value) > 0) {
            node.right = deleteRecursive(node.right, value);
        } else {
            if (node.left == null) {
                return node.right;
            } else if (node.right == null) {
                return node.left;
            }

            node.value = findMinValue(node.right);
            node.right = deleteRecursive(node.right, node.value);
        }

        return node;
    }

    private T findMinValue(Node node) {
        T minValue = node.value;
        while (node.left != null) {
            minValue = node.left.value;
            node = node.left;
        }
        return minValue;
    }

    private void printRecursive(Node node, String prefix, boolean isTail) {
        if (node != null) {
            System.out.println(prefix + (isTail ? "└── " : "├── ") + node.value);
            printRecursive(node.left, prefix + (isTail ? "    " : "│   "), node.right == null);
            printRecursive(node.right, prefix + (isTail ? "    " : "│   "), true);
        }
    }
}