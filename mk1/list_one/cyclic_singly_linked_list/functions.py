from cyclic_singly_linked_list import CyclicSinglyLinkedList

def insert(list, index):
    list.add(index)

# merges adds elements from list2 to list1
def merge(list1, list2):
    # empty lists
    if list1.is_empty():
        list1.pointer = list2.pointer
        list1.size = list2.size
        return
    if list2.is_empty():
        return
    
    last1 = list1.pointer
    last2 = list2.pointer
    while last2.next != list2.pointer:
        last2 = last2.next
    while last1.next != list1.pointer:
        last1 = last1.next
    
    last1.next = list2.pointer
    last2.next = list1.pointer
    list1.size += list2.size