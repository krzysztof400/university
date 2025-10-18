public class BinaryTree<T extends Comparable<T>> {
    private Node<T> root;

    BinaryTree() {
        root = null;
    }

    public void insert(Node<T> node) {
        if (getRoot() == null) {
            setRoot(node);
            return;
        }

        Node<T> current = getRoot();
        Node<T> parent = null;

        while (current != null) {
            parent = current;
            if (node.getValue().toString().compareTo(current.getValue().toString()) < 0) {
                current = current.getLeft();
            } else {
                current = current.getRight();
            }
        }

        if (node.getValue().toString().compareTo(parent.getValue().toString()) < 0) {
            parent.setLeft((Node<T>) node);
        } else {
            parent.setRight((Node<T>) node);
        }
    }

    public boolean search(Node<T> node) {
        Node<T> current = getRoot();

        while (current != null) {
            if (node.getValue().toString().compareTo(current.getValue().toString()) == 0) {
                return true;
            } else if (node.getValue().toString().compareTo(current.getValue().toString()) < 0) {
                current = current.getLeft();
            } else {
                current = current.getRight();
            }
        }
        return false;
    }

    public void delete(Node<T> node) {
        Node<T> current = getRoot();
        Node<T> parent = null;

        while (current != null) {
            if (node.getValue().toString().compareTo(current.getValue().toString()) == 0) {
                break;
            }
            parent = current;
            if (node.getValue().toString().compareTo(current.getValue().toString()) < 0) {
                current = current.getLeft();
            } else {
                current = current.getRight();
            }
        }

        if (current == null) {
            return;
        }

        if (current.getLeft() == null && current.getRight() == null) {
            if (current != getRoot()) {
                if (parent.getLeft() == current) {
                    parent.setLeft(null);
                } else {
                    parent.setRight(null);
                }
            } else {
                setRoot(null);
            }
        } else if (current.getLeft() != null && current.getRight() != null) {
            Node<T> successor = getSuccessor(current);
            delete(successor);
            current.setValue(successor.getValue());
        } else {
            Node<T> child = current.getLeft() != null ? current.getLeft() : current.getRight();
            if (current != getRoot()) {
                if (current == parent.getLeft()) {
                    parent.setLeft(child);
                } else {
                    parent.setRight(child);
                }
            } else {
                setRoot(child);
            }
        }
    }

    public Node<T> getSuccessor(Node<T> node) {
        Node<T> current = node.getRight();
        Node<T> parent = node;

        while (current.getLeft() != null) {
            parent = current;
            current = current.getLeft();
        }

        return current;
    }

    public Node<T> getRoot() {
        return root;
    }

    public void setRoot(Node<T> root) {
        this.root = root;
    }

    public void draw() {
        drawTree(root, "", true);
    }

    private void drawTree(Node<T> node, String prefix, boolean isTail) {
        if (node != null) {
            System.out.println(prefix + (isTail ? "└── " : "├── ") + node.value);
            drawTree(node.right, prefix + (isTail ? "    " : "│   "), false);
            drawTree(node.left, prefix + (isTail ? "    " : "│   "), true);
        }
    }

    public static void main(String[] args) {
        BinaryTree<Integer> tree = new BinaryTree<>();

        // Inserting different integers
        tree.insert(new Node<Integer>(5));
        tree.insert(new Node<Integer>(3));
        tree.insert(new Node<Integer>(7));
        tree.insert(new Node<Integer>(1));
        tree.insert(new Node<Integer>(9));
        tree.insert(new Node<Integer>(4));
        tree.insert(new Node<Integer>(6));

        // tree.delete(new Node<Integer>(5));

        System.out.println("Binary Tree with integers:");
        tree.draw();


        if(tree.search(new Node<Integer>(4))) {
            System.out.println("4 is in the tree");
        } else {
            System.out.println("4 is not in the tree");
        }

        if(tree.search(new Node<Integer>(8))) {
            System.out.println("8 is in the tree");
        } else {
            System.out.println("8 is not in the tree");
        }
    }
}
