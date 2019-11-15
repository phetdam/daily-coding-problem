# daily coding problem #165 (medium, easy naive solution)
#
# Changelog:
#
# 11-15-2019
#
# initial creation. thought i had a more efficient trick to solve the problem
# but the naive solution works fine, so i left it at that.

__doc__ = """
This problem was asked by Google.

Given an array of integers, return a new array where each element in the new
array is the number of smaller elements to the right of that element in the
original input array.

For example, given the array [3, 4, 9, 6, 1], return [1, 1, 2, 1, 0], since:

There is 1 smaller element to the right of 3
There is 1 smaller element to the right of 4
There are 2 smaller elements to the right of 9
There is 1 smaller element to the right of 6
There are no smaller elements to the right of 1
"""

# import evaluation function
from ufunc_eval import ufunc_eval

def smaller_right_els(ar):
    """
    the naive solution is O(n^2); look and scan.
    """
    assert (ar is not None) and (len(ar) > 0)
    n = len(ar)
    # output array; each ith element is the number of elements in ar that are
    # less than ar[i] and to the right of ar[i].
    outl = [0 for _ in range(n)]
    # for each element in ar (skip last one; always 0)
    for i in range(n - 1):
        # scan right
        for j in range(i + 1, n):
            # if ar[i] > ar[j], increment outl[i]
            if ar[i] > ar[j]: outl[i] = outl[i] + 1
    # return outl
    return outl

# main
if __name__ == "__main__":
    func = smaller_right_els
    # problem input, answer is [1, 1, 2, 1, 0]
    ar = [3, 4, 9, 6, 1]
    ufunc_eval(func, ar)
    # another input, answer is [0, 6, 4, 0, 2, 1, 1, 1, 0]
    ar = [-54, 34, 11, 0, 9, 4, 33, 99, 1]
    ufunc_eval(func, ar)
