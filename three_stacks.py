# daily coding problem #141 (hard, not really hard, just tedious)
#
# Changelog:
#
# 10-25-2019
#
# initial creation. i think this problem is a little vague; if you really want
# to use only 1 list with no other auxiliary variables, then my first class
# would be a solution but suffers in terms of search speed. the second class,
# which virtually segments the list and tracks each stack's top index, is faster
# but does use three extra variables. implemented a test function to add values
# and remove from the stacks as test.

__doc__ = """
This problem was asked by Microsoft.

Implement 3 stacks using a single list:

class Stack:

    def __init__(self):
        self.list = []

    def pop(self, stack_number):
        pass

    def push(self, item, stack_number):
        pass
"""

# import evaluation function
from ufunc_eval import ufunc_eval

# no auxiliary variables. stack numbers are 0, 1, and 2
class stack_iii:

    def __init__(self): self.list = []

    def pop(self, sn):
        """
        pop an item from the snth stack. this is done by looking for the first
        pair that has the stack number that matches sn.
        """
        assert (sn >= 0) and (sn <= 2)
        # search from the rear of the list to find the first (val, sn) pair that
        # contains the desired stack number sn
        for i in range(len(self.list) - 1, -1, -1):
            if self.list[i][1] == sn: return self.list.pop(i)[0]
        # if nothing, raise an exception
        raise Exception("empty stack {0}".format(sn))

    def push(self, val, sn):
        """
        push an item onto the snth stack. pretty simple; just append a pair
        (val, sn) to the end of the list that the pop function will look for
        when trying to remove the top element from the snth stack
        """
        assert (sn >= 0) and (sn <= 2)
        # append onto list (val, sn) pair
        self.list.append((val, sn))

# auxiliary variables. generalized to allow k stacks.
class kstack:

    def __init__(self, k):
        assert k > 0
        # array of ints that tell you where the top element of the kth stack
        # resides; is of length k
        self.tops = [0 for _ in range(k)]
        # array of ints that tell you the size of each of the k stacks
        self.sizes = [0 for _ in range(k)]
        # the "stack" itself
        self.stk = []

    def pop(self, sn):
        """
        pop an item from the snth stack. use self.tops to find the top element
        of the stack. all other values in tops at higher indices must be thus
        adjusted down accordingly. check self.sizes to make sure stack sn is
        not empty; if so, then we raise an exception.
        """
        assert sn >= 0
        # if empty, raise exception
        if self.sizes[sn] == 0: raise Exception("empty stack {0}".format(sn))
        # else pop element from that index, decrement self.tops[sn] and every
        # other element at index sn + 1 and above in self.tops, and then also
        # decrement self.sizes[sn] too
        self.stk.pop(self.tops[sn])
        for i in range(sn + 1, len(self.tops)):
            self.tops[i] = self.tops[i] - 1
        self.sizes[sn] = self.sizes[sn] - 1

    def push(self, val, sn):
        """
        push an item onto the snth stack. since python list insert inserts
        before the target index, insert at the right self.tops[sn], and then
        increment the elements in self.tops at indices sn and above. also
        accordingly increment self.sizes[sn].
        """
        assert sn >= 0
        # insert before self.tops[sn]
        self.stk.insert(self.tops[sn], val)
        # increment all the self.tops values sn and above
        for i in range(sn, len(self.tops)):
            self.tops[i] = self.tops[i] + 1
        # increment the size of the snth stack
        self.sizes[sn] = self.sizes[sn] + 1

def multi_stack_driver(stk_class, pairs, sn, verbose = False):
    """
    driver function for testing each of the classes. takes a pointer to the
    class that is being instantiated, (value, stack number) pairs, and the
    number of stacks that the stack class should take (sn). if the stack class
    is stack_iii, then the sn parameter is simply ignored. verbose simply
    toggles whether or not each push and pop will be echoed to the command line.
    """
    assert (stk_class is not None) and (pairs is not None) and (len(pairs) > 0)
    # create multi stack based on type
    mstk = None
    if stk_class == stack_iii:
        mstk = stk_class()
        # set sn to 3 since we need it later
        sn = 3
    elif stk_class == kstack: mstk = kstack(sn)
    else: raise Exception("unknown stack type {0}".format(stk_class))
    # add each pair to each stack
    for e in pairs:
        mstk.push(*e)
        # if verbose is True, echo to command line
        if verbose == True: print("push {0} onto stack {1}".format(*e))
    # remove elements from the stacks until empty, rotating across stacks. when
    # number of consecutive exceptions is 3, then we stop (all stacks empty)
    nce = 0
    while nce < 3:
        for i in range(sn):
            # pop until exception (i know this is kind of silly, but the
            # stack_iii class does not track the sizes of the stacks
            try:
                v = mstk.pop(i)
                # if verbose, echo to command line
                if verbose == True: print("pop {0} from stack {1}".format(v, i))
                # set number of consecutive exceptions to 0
                nce = 0
            except:
                # if verbose, indicate that the stack is empty
                if verbose == True: print("empty stack {0}".format(i))
                # increment number of consecutive exceptions
                nce = nce + 1

# main
if __name__ == "__main__":
    # driver program to test out each different stack and input.
    pairs = [(5, 0), (6, 0), (-3, 1), (4, 1), (1, 2), (99, 0), (90, 2), (22, 2),
             (-8, 0), (7, 1), (9, 1), (-1, 0)]
    multi_stack_driver(stack_iii, pairs, 3, verbose = True)
    multi_stack_driver(kstack, pairs, 3, verbose = True)
