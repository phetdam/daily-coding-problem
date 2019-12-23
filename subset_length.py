# daily coding problem #189 (easy)
#
# Changelog:
#
# 12-23-2019
#
# initial creation. pretty easy question. i put two different solutions because
# in the python context, one would just use the set class for this task. but i
# also added a lookup table solution as a non-python-specific alternative.

__doc__ = """
This problem was asked by Google.

Given an array of elements, return the length of the longest subarray where all
its elements are distinct.

For example, given the array [5, 1, 3, 5, 2, 3, 4, 1], return 5 as the longest
subarray of distinct elements is [5, 2, 3, 4, 1].
"""

# import evaluation function
from ufunc_eval import ufunc_eval

def n_distinct_native(ar):
    """
    native python solution to the problem by using the set class.
    """
    assert ar is not None
    if len(ar) == 0: return 0
    # create set and return length of set
    return len(set(ar))

def n_distinct_dict(ar):
    """
    solution using a dictionary, so requires O(n) time and O(n) space.
    """
    assert ar is not None
    # if length is 0, just return 0
    if len(ar) == 0: return 0
    # lookup of unique values
    look = {}
    # for all elements in ar, increment the count in the lookup table for the
    # values that already have a key there and add new entries
    for e in ar:
        if e in look: look[e] = look[e] + 1
        else: look[e] = 1
    # return length of look
    return len(look)

# main
if __name__ == "__main__":
    func_a = n_distinct_native
    func_b = n_distinct_dict
    # problem input, answer is 5
    ar = [5, 1, 3, 5, 2, 3, 4, 1]
    ufunc_eval(func_a, ar)
    ufunc_eval(func_b, ar)
    # another input, answer is 8
    ar = [6, 4, 5, 2, 6, 4, 5, 7, 8, 2, 4, 65, 2, 3]
    ufunc_eval(func_a, ar)
    ufunc_eval(func_b, ar)
