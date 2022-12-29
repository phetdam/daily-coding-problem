# daily coding problem #154 (easy, once you realize there is a trick to it)
#
# Changelog:
#
# 11-04-2019
#
# initial creation. not sure that i did it correctly, but technically this is a
# max heap based on the size of the max heap, so that the most recent element
# always has the largest key and thus will be removed upon pop(). this makes the
# problem much simpler than it might initially seem.

__doc__ = """
Implement a stack API using only a heap. A stack implements the following
methods:

push(item), which adds an element to the stack
pop(), which removes and returns the most recently added element (or throws an
error if there is nothing on the stack)

Recall that a heap has the following operations:

push(item), which adds a new key to the heap
pop(), which removes and returns the max value of the heap
"""

# import evaluation function
from ufunc_eval import ufunc_eval

# the trick is to create a max heap where each key is based on an internal
# counter that increases or decreases depending on whether an item is pushed or
# popped. thus, it is a heap, but it is essentially acting as a stack, as the
# max key on the heap will always be the last inserted item's insertion number.
class heap_stack:

    def __init__(self):
        # size of the heap, also generates keys for insertion
        self.siz = 0
        self.data = []

    def push(self, val):
        """
        push a value onto the heap. the heap key will be self.siz, and the tuple
        (self.siz, val) appended to the end of self.data. the heap is guaranteed
        to have keys in ascending order, and that is why a sufficient pop() is
        to pop the last element in self.data, as the "max" element due to
        largest insertion number is always the last element.
        """
        # append tuple of (heap_key, val). see that the heap will always be in
        # ascending order of heap_key. use self.siz as heap_key.
        self.data.append((self.siz, val))
        # increment self.siz to keep track of size and generate the next
        # insertion number for the next value to be pushed
        self.siz = self.siz + 1

    def pop(self):
        """
        pop from the stack, removing "largest" item in the heap. if the heap is
        empty than an exception will be raised.
        """
        # exception for empty heap
        if self.siz == 0: raise Exception("empty max heap")
        # return data in the heap tuple
        return self.data.pop()[1]

    def values(self):
        """return values only, in list order"""
        outl = []
        for (a, b) in self.data: outl.append(b)
        return outl

# main
if __name__ == "__main__":
    # insert some items, popping them should result in a reversed order
    ar = [5, 1, -6, 3, -9, 11, 14, 99, -900, -2]
    print(ar)
    # our heap stack
    stk = heap_stack()
    for e in ar: stk.push(e)
    # output list of stack contents in insertion order
    outl = []
    for e in ar: outl.append(stk.pop())
    print(outl)
    # push 3, pop 2 3 times. outl should then be [5, 11]
    stk = heap_stack()
    k = 0
    for e in ar:
        # push onto stack 3 times
        if k < 3:
            stk.push(e)
            k = k + 1
        # pop 2 times
        elif k > 1:
            stk.pop()
            k = k + 1
            # reset to start pushing again
            if k == 5: k = 0
    # print stack contents
    print(stk.values())
