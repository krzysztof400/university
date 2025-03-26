class Node:
    def __init__(self, value):
        self.value = value
        self.next = None

class CyclicSinglyLinkedList:
    def __init__(self):
        self.pointer = None
        self.size = 0
        
    def add(self, value):
        new_node = Node(value)
        if self.is_empty():
            self.pointer = new_node
            new_node.next = new_node
        else:
            current = self.pointer
            while current.next != self.pointer:
                current = current.next
            current.next = new_node
            new_node.next = self.pointer
        self.size += 1
    
    def peek(self):
        if self.is_empty():
            raise IndexError("can't peek, list is empty")
        return self.pointer
    
    def remove(self):
        if self.is_empty():
            raise IndexError("can't remove, list is empty")
        else:
            last = self.pointer
            while last.next != self.pointer:
                last = last.next
            last.next = self.pointer.next
            value = self.pointer.value
            self.pointer = last.next
            self.size -= 1
            return value

    def is_empty(self):
        if self.size == 0:
            return True
        return False
    
    def search(self, value):
            if not self.pointer:
                return -1
            temp = self.pointer
            comparisons = 0
            for _ in range(self.size):
                comparisons += 1
                if temp.value == value:
                    return comparisons
                temp = temp.next
            return comparisons
        
    def print_list(self):
        if self.is_empty():
            print("can't print, list is empty")
            return
        
        current = self.pointer
        for _ in range(self.size):
            print(current.value, end=" -> ")
            current = current.next
        print(f"(cyclicly returns to: {self.pointer.value})")