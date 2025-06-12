from cyclic_singly_linked_list import CyclicSinglyLinkedList
from functions import insert
from functions import merge
import random

def test():
    # examine merge
    L_11 = CyclicSinglyLinkedList()
    L_12 = CyclicSinglyLinkedList()

    for i in range(10):
        insert(L_11, random.randint(10, 99))
        insert(L_12, random.randint(10, 99))

    L_11.print_list()
    L_12.print_list()

    merge(L_11, L_12)
    L_11.print_list()

    # examine searching cost
    T = []
    for i in range(10000):
        T.append(random.randint(0, 100000))
    
    L = CyclicSinglyLinkedList()
    for i in range(len(T)):
        insert(L, T[i])

    search_samples = random.sample(T, 1000)
    random_samples = [random.randint(0, 100000) for _ in range(1000)]

    search_costs = [L.search(num) for num in search_samples]
    random_search_costs = [L.search(num) for num in random_samples]

    print("Average cost (found):", sum(search_costs) / len(search_costs))
    print("Average cost (random):", sum(random_search_costs) / len(random_search_costs))