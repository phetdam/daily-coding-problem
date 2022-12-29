# daily coding problem #79 (medium, but probably should be an easy)
#
# Changelog:
#
# 08-20-2019
#
# initial creation. simple O(n) solution that can be done by checking all of the
# adjaceny pairs to see how many of them are decreasing pairs.

__doc__ = """
This problem was asked by Facebook.

Given an array of integers, write a function to determine whether the array
could become non-decreasing by modifying at most 1 element.

For example, given the array [10, 5, 7], you should return true, since we can
modify the 10 into a 1 to make the array non-decreasing. Given the array
[10, 5, 1], you should return false, since we can't modify any one element to
get a non-decreasing array.
"""

# import evaluation function
from ufunc_eval import ufunc_eval

def nd_mod_one(ar):
    """
    O(n) solution. all we have to do is check the n - 1 pairs of adjacent values
    and count the number of pairs that are decreasing. if this number > 1, we
    then return False, else we return True.
    """
    assert ar is not None
    # length of array
    n = len(ar)
    # number of times we have a decreasing pair
    ndp = 0
    # for each of the n - 1 adjacent pairs
    for i in range(n - 1):
        # if the pair is decreasing, increment ndp
        if ar[i] > ar[i + 1]: ndp = ndp + 1
    # return True if ndp <= 1, else False
    return ndp <= 1

# main
if __name__ == "__main__":
    func = nd_mod_one
    # problem input 1, answer is True
    ar = [10, 5, 7]
    ufunc_eval(func, ar)
    # problem input 2, answer is False
    ar = [10, 5, 1]
    ufunc_eval(func, ar)
    # other input, answer is False
    ar = [1, 3, 4, 5, 4, 7, 6, 8, 7, 8]
    ufunc_eval(func, ar)
