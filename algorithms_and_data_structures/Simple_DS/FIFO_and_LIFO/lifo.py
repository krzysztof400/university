class LIFOQueue:
    def __init__(self):
        self.queue = []
    
    def add(self, item):
        self.queue.append(item)
    
    def remove(self):
        if self.is_empty():
            raise IndexError("Queue is empty")
        return self.queue.pop(-1)
    
    def is_empty(self):
        if len(self.queue) == 0:
            return True
        return False

    def peek(self):
        if self.is_empty():
            return None
        return self.queue[-1]