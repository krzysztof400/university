/**
 * Klasa reprezentująca węzeł drzewa binarnego.
 * @param <T> Typ danych przechowywanych w węźle.
 */

public class Node<T extends Comparable<T>> {
    T value;
    Node<T> left;
    Node<T> right;

    Node(T value) {
        this.value = value;
        this.left = null;
        this.right = null;
    }

    public T getValue() {
        return value;
    }

    public Node<T> getLeft() {
        return left;
    }

    public Node<T> getRight() {
        return right;
    }

    public void setValue(T value) {
        this.value = value;
    }

    public void setLeft(Node<T> left) {
        this.left = left;
    }

    public void setRight(Node<T> right) {
        this.right = right;
    }
}