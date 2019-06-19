# daily coding problem #16 (easy)
#
# Changelog:
#
# 06-18-2019
#
# initial creation. completed the problem by trivially implementing (using
# python's built in data structure) a dynamic array for O(1) amortized
# insertion and O(1) search. i also made a linked list that would be O(1)
# insertion but O(n) search worst case; pointers also take up more memory.
# i decided not to use a hash table because it can also end up using quite a
# bit of memory as well, and is not much faster than the dynamic array.

__doc__ = """
This problem was asked by Twitter.

You run an e-commerce website and want to record the last N order ids in a log.
Implement a data structure to accomplish this, with the following API:

record(order_id): adds the order_id to the log
get_last(i): gets the ith last element from the log. i is guaranteed to be
smaller than or equal to N.

You should be as efficient with time and space as possible.
"""

# import evaluation function
from ufunc_eval import ufunc_eval

# simple doubly linked list class, with head and tail pointers. even though this
# is not the answer, there is nothing wrong with more practice. we cannot use
# xor linked list (although that would have been fun) since there is no raw
# pointer exposure in python natively.
class node:
    def __init__(self, data):
        self.data = data
        self.prev = None
        self.next = None

# linked list version of the log
class ll_log:
    def __init__(self):
        self.head = None
        self.tail = None
        self.size = 0

    def record(self, order_id):
        """
        O(1) addition of an ID to the log, since we are using a linked list
        """
        # create new node
        ent = node(order_id)
        # base case: if self.head is None, then set it to ent with tail
        if self.head is None:
            self.head = self.tail = ent
        # else just update tail to ent
        else:
            self.tail.next = ent
            ent.prev = self.tail
            self.tail = ent
        # increment number of entries in ll_log
        self.size = self.size + 1

    def get_last(self, i):
        """
        O(size) recovery of the ith last id entry in the log, by traversing
        backwards or forwards, depending on the size of i
        """
        # if i > size or is not positive, return None
        if i > self.size or i < 1: return None
        # current pointer
        cur = None
        # if i < floor(n / 2), start from tail pointer as usual
        if i < self.size // 2:
            cur = self.tail
            # since we are already pointing at the 1st smallest element
            while i - 1 > 0:
                # update cur
                cur = cur.prev
                # decrement i
                i = i - 1
            # return cur's data
            return cur.data
        # else i > floor(n / 2), so we set i to n + 1 - i and find the ith
        # largest element, starting from the head pointer (out of n elements,
        # the ith smallest element is the n - i + 1th largest)
        i = self.size + 1 - i
        cur = self.head
        # while we are not done iterating (again, we are already pointing at
        # the n - i + 1th largest)
        while i - 1 > 0:
            # update cur and decrement i
            cur = cur.next
            i = i - 1
        # return cur's data
        return cur.data

# dynamic array version of the log; trivial
class da_log:
    def __init__(self):
        self.ents = []
        self.size = 0

    def record(self, order_id):
        """
        O(1) amortized addition of ID to the log due to the dynamic array
        """
        # append and increment size
        self.ents.append(order_id)
        self.size = self.size + 1

    def get_last(self, i):
        """
        O(1) search since an array has indexes
        """
        # if i is greater than size or is nonpositive, return None
        if i > self.size or i < 1: return None
        # else return ith smallest
        return self.ents[self.size - i]

def g(x):
    """
    description of solution runtime and any other comments. usually necessary
    because follow up question are often asked.
    """
    return False

# main
if __name__ == "__main__":
    # max number of ids (please make at least 100 for fractions to be even)
    N = 1000
    # kth smallest to return (cast to int to prevent implicit float casts)
    k = int(N / 4)
    # list of ids
    idl = [i for i in range(N)]
    # check to see that linked list and dynamic array produce same output
    logll = ll_log()
    logda = da_log()
    # add everything in idl to logll and logda
    for e in idl:
        logll.record(e)
        logda.record(e)
    # return kth and N - kth last elements (answers: N - k and k)
    ufunc_eval(logll.get_last, k)
    ufunc_eval(logll.get_last, N - k)
    ufunc_eval(logda.get_last, k)
    ufunc_eval(logda.get_last, N - k)
