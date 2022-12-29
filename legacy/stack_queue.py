# daily coding problem #53 (medium, not really)
#
# Changelog:
#
# 07-25-2019
#
# initial creation. pretty simple once you realize that popping everything from
# a queue essentially results in an opposite ordering of the stack elements.

__doc__ = """
his problem was asked by Apple.

Implement a queue using two stacks. Recall that a queue is a FIFO (first-in,
first-out) data structure with the following methods: enqueue, which inserts an
element into the queue, and dequeue, which removes it.
"""

# import evaluation function
from ufunc_eval import ufunc_eval

# queue implemented using stacks
class stk_queue:

    def __init__(self):
        self.main = []
        self.aux = []
        self.siz = 0

    def enqueue(self, data):
        """
        when implementing a queue with stacks, either the enqueue or dequeue
        operation must be O(n) in order to maintain the queue order. i choose
        to make enqueueing O(n), so that dequeueing corresponds to popping off
        the topmost element in O(1).
        """
        for i in range(self.siz): self.aux.append(self.main.pop())
        self.main.append(data)
        for i in range(self.siz): self.main.append(self.aux.pop())
        self.siz = self.siz + 1

    def dequeue(self):
        e = self.main.pop()
        self.siz = self.siz - 1
        return e

    def __str__(self):
        """
        returns the queue in string form with useful annotations
        """
        # get list as string
        ars = str(self.main)
        # get count string
        cs = "count = " + str(self.siz)
        # get front/back string
        fbs = (max(len(ars) - 4, 0) * " ") + "head"
        # return
        return fbs + "\n" + ars + " " + cs

# main
if __name__ == "__main__":
    # ascending collection of floats
    n = 12
    ar = [e + 1 for e in range(n)]
    sq = stk_queue()
    # enqueue all and print
    for e in ar: sq.enqueue(e)
    print(sq)
    # dequeue half and print
    for i in range(n // 2): sq.dequeue()
    print(sq)
    # dequeue the rest and print empty
    for i in range(sq.siz): sq.dequeue()
    print(sq)
