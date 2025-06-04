import random

class Node:
    def __init__(self, value):
        self.value = value
        self.next = None
        self.previous = None

class CyclicDoubleLinkedList:
    def __init__(self):
        self.pointer = None
        self.size = 0
        
    def add(self, value):
        new_node = Node(value)
        if self.is_empty():
            self.pointer = new_node
            new_node.next = new_node
            new_node.previous = new_node

        else:
            previous = self.pointer.previous
            previous.next = new_node
            new_node.previous = previous
            new_node.next = self.pointer
            self.pointer.previous = new_node
            
        self.size += 1
    
    def search(self, value):
        if not self.pointer:
            return -1
        temp1 = self.pointer
        temp2 = self.pointer.previous
        comparisons = 0
        search_forward = random.choice([True, False])
        
        for _ in range(self.size):
            comparisons += 1
            if search_forward:
                if temp1.value == value:
                    return comparisons
                temp1 = temp1.next
            else:
                if temp2.value == value:
                    return comparisons
                temp2 = temp2.previous
        return comparisons
    
    def peek(self):
        if self.is_empty():
            raise IndexError("can't peek, list is empty")
        return self.pointer
    
    def remove(self):
        if self.is_empty():
            raise IndexError("can't remove, list is empty")
        else:
            last = self.pointer.previous
            value = self.pointer.value
            last.next = self.pointer.next
            self.pointer = self.pointer.next
            self.size -= 1
            return value

    def is_empty(self):
        if self.size == 0:
            return True
        return False
        
    def print_list(self):
        if self.is_empty():
            print("can't print, list is empty")
            return
        
        current = self.pointer
        for _ in range(self.size):
            print(current.value, end=" -> ")
            current = current.next
        print(f"(cyclicly returns to: {self.pointer.value})")