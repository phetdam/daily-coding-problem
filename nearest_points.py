# daily coding problem #150 (hard, but not really)
#
# Changelog:
#
# 10-31-2019
#
# initial creation. i will admit that i took the lazy approach to this problem
# which is supposed to work better on average but technically also does require
# O(n) extra space as well due to returning a new list. for small k, it really
# is better to just select elements manually instead. happy halloween!

__doc__ = """
This problem was asked by LinkedIn.

Given a list of points, a central point, and an integer k, find the nearest k
points from the central point.

For example, given the list of points [(0, 0), (5, 4), (3, 1)], the central
point (1, 2), and k = 2, return [(0, 0), (3, 1)].
"""

# import evaluation function
from ufunc_eval import ufunc_eval
from math import sqrt

def nearest(pts, cp, k):
    """
    given a list of (x, y) tuples pts, a central tuple cp, and k > 0, returns
    the nearest k points to the central tuple by sorting using euclidean
    distance from cp as the criteria for sorting, so O(nlogn). clearly, however,
    for small k this is slower than linear search.
    """
    assert (pts is not None) and (cp is not None) and (k > 0)
    # get length of pts
    n = len(pts)
    # edge cases
    if (n == 0) or (k > n): return None
    # get new list of points, sorted by distance from cp
    spts = sorted(pts, key = lambda a: sqrt((a[0] - cp[0]) ** 2 +
                                            (a[1] - cp[1]) ** 2))
    # return the first k
    return spts[:k]

# main
if __name__ == "__main__":
    func = nearest
    # problem input, answer is [(0, 0), (3, 1)]
    pts = [(0, 0), (5, 4), (3, 1)]
    cp, k = (1, 2), 2
    ufunc_eval(func, pts, cp, k)
    # another input, answer is [(0, 0), (-2, 1), (-2, -2), (2, 3), (-1, 4)]
    pts = [(1, 5), (2, 3), (-1, 4), (4, -1), (-2, -2), (0, 0), (-2, 1), (3, 4)]
    cp, k = (0, 0), 5
    ufunc_eval(func, pts, cp, k)
