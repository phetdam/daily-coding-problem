# daily coding problem #77 (easy)
#
# Changelog:
#
# 08-18-2019
#
# initial creation. i feel like i have done this one before. the simple way to
# tackle the problem is to sort the intervals based on their infinum and then
# check if subsequent intervals are overlapping or not.

__doc__ = """
This problem was asked by Snapchat.

Given a list of possibly overlapping intervals, return a new list of intervals
where all overlapping intervals have been merged. The input list is not
necessarily ordered in any way. For example, given [(1, 3), (5, 8), (4, 10),
(20, 25)], you should return [(1, 3), (4, 10), (20, 25)].
"""

# import evaluation function
from ufunc_eval import ufunc_eval
from math import inf

def merge_ranges(ar):
    """
    O(nlogn) solution. sort and then check all of the ranges. O(n) space.
    """
    assert ar is not None
    # length of ar
    n = len(ar)
    # output list
    outl = []
    # if n == 0; return empty list
    if n == 0: return outl
    # sort the ranges by the first element
    ar = sorted(ar, key = lambda x: x[0])
    # current infinum and supremum of first interval
    lo, hi = ar[0]
    # for all elements in the sorted ar besides the first
    for i in range(1, n):
        # current range infinum and supremum
        clo, chi = ar[i]
        # if there is no overlap, add (lo, hi) to outl and set (lo, hi) to
        # (clo, chi) as the next non-overlapping interval
        if (chi < lo) or (clo > hi):
            outl.append((lo, hi))
            lo, hi = clo, chi
        # else there is an overlap, so update lo and hi
        else: lo, hi = min(lo, clo), max(hi, chi)
    # add the last overlapping interval to outl and return
    outl.append((lo, hi))
    return outl

# main
if __name__ == "__main__":
    func = merge_ranges
    # problem input, answer is [(1, 3), (4, 10), (20, 25)]
    ar = [(1, 3), (5, 8), (4, 10), (20, 25)]
    ufunc_eval(func, ar)
    # another input, answer is [(1, 4), (6, 19)]
    ar = [(10, 19), (11, 12), (14, 18), (1, 4), (6, 9), (7, 10), (8, 15)]
    ufunc_eval(func, ar)
