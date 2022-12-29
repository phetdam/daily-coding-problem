# daily coding problem #134 (easy)
#
# Changelog:
#
# 10-17-2019
#
# initial creation. used a hash table to hold (index, value) pairs for nonzero
# values, while zero values were simply omitted from the table.

__doc__ = """
This problem was asked by Facebook.

You have a large array with most of the elements as zero.

Use a more space-efficient data structure, SparseArray, that implements:

init(arr, size): initialize with the original large array and size.
set(i, val): updates index at i with val.
get(i): gets the value at index i.
"""

# import evaluation function
from ufunc_eval import ufunc_eval

# the idea is to use a hash table to store values, with the index being a key.
# any elements that are not found in the table are assumed to be 0.
class sparse_array:

    def __init__(self, ar):
        """
        constructor. automatically uses the size of ar to initialize.
        """
        # get length of array and initialize dictionary
        self.siz = len(ar)
        self.look = {}
        # insert all nonzero elements into the dictionary
        for i in range(self.siz):
            if ar[i] != 0: self.look[i] = ar[i]

    def set(self, i, val):
        """
        sets index i with val. if the index contains a 0 (not in look, then a
        new lookup entry with key i is created in look. note that if val is a
        0, then if the key i exists, it will be deleted, while nothing will
        happen if it does not exist in look.
        """
        # if the key exists
        if i in self.look:
            # if val is 0, pop the key that exists
            if val == 0: self.look.pop(i)
            # else set the key with val
            else: self.look[i] = val
        # else the key does not exist
        else:
            # if val is 0, do nothing
            if val == 0: pass
            # else create a new key/value pair
            else: self.look[i] = val

    def get(self, i):
        """
        returns the value at index i, if 0 <= i < self.siz. if the index is not
        a key in look, then we know its value is zero.
        """
        if (i < 0) or (i > self.siz - 1): return None
        if i in self.look: return self.look[i]
        return 0

    def nonzero(self, keys = False):
        """
        returns a list of the nonzero elements in index order. if keys is True,
        then a sorted list of (key, value) pairs is returned, else just a list
        of the values will be returned.
        """
        if keys is True:
            out = []
            for k, v in self.look.items():
                out.append((k, v))
            out.sort(key = lambda x: x[0])
            return out
        return list(self.look.values())

# main
if __name__ == "__main__":
    # some input to test
    ar = [0, 0, -5, 0, -1, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 6, 0, 10, 0, 0]
    # new sparse_array, and we test some functions
    sar = sparse_array(ar)
    # return all the nonzero entries
    print("nonzero entries: {0}".format(sar.nonzero(keys = True)))
    # set some of the nonzero entries to 0 and some 0 entries to nonzero
    sar.set(0, -19)
    sar.set(8, 89)
    sar.set(2, 0)
    sar.set(19, 0)
    print("nonzero entries: {0}".format(sar.nonzero(keys = True)))
    # get some arbitrary elements
    print(sar.get(0), sar.get(2), sar.get(8), sar.get(19))
