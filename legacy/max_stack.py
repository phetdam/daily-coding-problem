# daily coding problem #43 (easy)
#
# Changelog:
#
# 07-15-2019
#
# initial creation. it was pretty easy.

__doc__ = """
This problem was asked by Amazon.

Implement a stack that has the following methods:

push(val), which pushes an element onto the stack
pop(), which pops off and returns the topmost element of the stack. If there are
no elements in the stack, then it should throw an error or return null.
max(), which returns the maximum value in the stack currently. If there are no
elements in the stack, then it should throw an error or return null. Each method
should run in constant time.
"""

# import evaluation function
from ufunc_eval import ufunc_eval
# import inf
from math import inf
# randint
from random import randint

# node class for stack
class cstack_node:
    def __init__(self, v):
        self.val = v
        self.next = None

# stack class. could just abuse dynamic array O(1) amortized operations but i
# choose to use linked list for demonstration purposes.
class cstack:
    def __init__(self):
        self.head = None
        # size is optional
        self.size = 0
        # necessary for O(1) max
        self.max = -inf

    def push(self, v):
        # new node
        new_node = cstack_node(v)
        # save new max if encountered
        if v > self.max: self.max = v
        # increment size
        self.size = self.size + 1
        # append as new head
        if self.head is not None: new_node.next = self.head
        self.head = new_node

    def pop(self):
        # return None if empty
        if self.size == 0: return None
        # else retrieve value, update head, and decrement size
        v = self.head.val
        self.head = self.head.next
        self.size = self.size - 1
        # reset max to -inf if size is 0 again
        if self.size == 0: self.max = -inf
        return v

    def get_max(self):
        # return max if any
        if self.max > -inf: return self.max
        return None

# main
if __name__ == "__main__":
    # parameter, > 0
    a = 50
    # array of integers
    ar = [randint(-int(a * 0.9), int(a * 1.3)) for _ in range(int(a * 0.75))]
    print(ar)
    # push, pop, and report max
    b = min(randint(int(a / 3), int(a / 2)), len(ar))
    stk = cstack()
    for i in range(b): stk.push(ar[i])
    print("max of stk: {0}".format(stk.get_max()))
    for i in range(b): stk.pop()
    print("max of (empty) stk: {0}".format(stk.get_max()))
    stk.pop()
    print("max of (empty) stk: {0}".format(stk.get_max()))
    stk.push(ar[-1])
    print("last pushed (now popped): {0}".format(stk.pop()))
