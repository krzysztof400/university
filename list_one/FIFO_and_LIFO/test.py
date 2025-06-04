from fifo import FIFOQueue
from lifo import LIFOQueue

def test_queues():
    fifo = FIFOQueue()
    lifo = LIFOQueue()

    for i in range(50):
        fifo.add(i)
        lifo.add(i)

    print("FIFO - first in first out")
    while not fifo.is_empty():
        removed = fifo.remove()
        print(f"Removed: {removed}")

    print("LIFO - last in first out")
    while not lifo.is_empty():
        removed = lifo.remove()
        print(f"Removed: {removed}")