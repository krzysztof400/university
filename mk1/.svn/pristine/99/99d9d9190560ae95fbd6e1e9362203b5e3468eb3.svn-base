from cyclic_double_linked_list import CyclicDoubleLinkedList

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
    
    last1 = list1.pointer.previous
    last2 = list2.pointer.previous
    
    last1.next = list2.pointer
    last2.next = list1.pointer
    list1.size += list2.size