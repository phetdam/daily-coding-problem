# daily coding problem #166 (medium, a good one)
#
# Changelog:
#
# 11-23-2019
#
# initial creation. this is an interesting medium problem. took me a little
# while since in next() i forgot to increment self.cur_mem_el, so that i was
# never advancing to the next element in self.data. i am happy that my logic
# with hasnext() was more or less correct on the first try.

__doc__ = """
This problem was asked by Uber.

Implement a 2D iterator class. It will be initialized with an array of arrays,
and should implement the following methods:

next(): returns the next element in the array of arrays. If there are no more
elements, raise an exception.
has_next(): returns whether or not the iterator still has elements left.

For example, given the input [[1, 2], [3], [], [4, 5, 6]], calling next()
repeatedly should output 1, 2, 3, 4, 5, 6.

Do not use flatten or otherwise clone the arrays. [Some] arrays can be empty.
"""

# import evaluation function
from ufunc_eval import ufunc_eval

# custom exception class, totally unnecessary
class OutOfElementsException(Exception):

    def __init__(self): pass

    def __str__(self): return "no more elements in iterator_2d"

# 2d iterator class, no flattening
class iterator_2d:

    def __init__(self, list_2d):
        """
        takes a two-dimensional list, i.e. a list of lists. we assume that the
        list is actually two-dimensional, and that it is non-empty, although
        some of its member lists may be empty themselves.
        """
        assert (list_2d is not None) and (len(list_2d) > 0)
        # store the list (make a pointer to it)
        self.data = list_2d
        # number of members in 2dlist; store this to prevent recomputing it
        self.data_len = len(self.data)
        # compute and store the lengths of all the member lists
        self.mem_lens = [None for _ in range(self.data_len)]
        for i in range(self.data_len): self.mem_lens[i] = len(self.data[i])
        # keep track of current member and current element within member
        self.cur_mem, self.cur_mem_el = 0, 0

    def hasnext(self, return_next = False):
        """
        returns True if the iterator has elements left, False if not. as long as
        we can find another element in self.data (note that we cannot just test
        if self.cur_mem < self.data_len, as we are allowed to have empty member
        lists), then hasnext will return True. the return_next will return the
        next element if any, and next() is just a wrapper around hasnext() with
        return_next as True. this way, after checking if there is another
        element with hasnext(), one can simply return that element. the position
        of self.cur_mem and self.cur_mem_el will also be updated.
        """
        # temporary current member, current member element
        cur_mem, cur_mem_el = self.cur_mem, self.cur_mem_el
        # look for the next element
        for i in range(cur_mem, self.data_len):
            for j in range(cur_mem_el, self.mem_lens[i]):
                # if there is an element in member i's sublist from j to end
                if len(self.data[i][j:]) > 0:
                    # update self.cur_mem and self.cur_mem_el to mark the
                    # position of the next element that can be returned
                    self.cur_mem, self.cur_mem_el = i, j
                    # if return_next is True, return self.data[i][j]
                    if return_next == True: return self.data[i][j]
                    # else just return True
                    return True
            # (re)set cur_mem_el to 0 if we are going to the next member
            cur_mem_el = 0
        # if there are no more elements, update self.cur_mem to self.data_len
        # and self.cur_mem_el to self.mem_lens[-1] so that the double for loop
        # will not run when hasnext() is called again
        self.cur_mem, self.cur_mem_el = self.data_len, self.mem_lens[-1]
        # if return_next is True, return None
        if return_next == True: return None
        # else just return False
        return False

    def next(self):
        """
        returns the next element. if there are no more elements, raises a
        custom exception (sorry, i was bored). wrapper around a call to
        hasnext() with the return_next argument as True.
        """
        # get next value
        val = self.hasnext(return_next = True)
        # update self.cur_mem_el
        self.cur_mem_el = self.cur_mem_el + 1
        # if val is None, no more elements, so raise exception
        if val is None: raise OutOfElementsException()
        # else just return val
        return val

def iterator_2d_test(iter):
    """
    call next until all elements returned, then call one more time to test the
    implementation of the exception.
    """
    assert iter is not None
    # elements in iter
    outl = []
    # add all elements by using the next() call
    while iter.hasnext() == True: outl.append(iter.next())
    # catch the exception
    ooee = None
    try: iter.next()
    except OutOfElementsException as e: ooee = e
    # return elements and the exception's string message
    return outl, str(ooee)

# main
if __name__ == "__main__":
    # test function that will call next to return all elements and then call
    # next an extra time simply to catch the exception
    func = iterator_2d_test
    # start with problem input
    iter = iterator_2d([[1, 2], [3], [], [4, 5, 6]])
    ufunc_eval(func, iter)
    # test with another input
    iter = iterator_2d([[-4, -3], [], [-2, -1], [0, 1, 2], [], [3], [4]])
    ufunc_eval(func, iter)
    # show what the actual output looks like for the exception
    iter.next()
