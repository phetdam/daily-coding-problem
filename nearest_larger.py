# daily coding problem #144 (medium)
#
# Changelog:
#
# 10-24-2019
#
# initial creation. not exactly sure what the problem is asking, as even if you
# do some preprocessing, it is still O(n) anyways. i ended up doing it in two
# O(n) methods, one of which is simpler than the other.

__doc__ = """
This problem was asked by Google.

Given an array of numbers and an index i, return the index of the nearest larger
number of the number at index i, where distance is measured in array indices.

For example, given [4, 1, 3, 5, 6] and index 0, you should return 3.

If two distances to larger numbers are the equal, then return any one of them.
If the array at i doesn't have a nearest larger integer, then return null.

Follow-up: If you can preprocess the array, can you do this in constant time?
"""

# import evaluation function
from ufunc_eval import ufunc_eval
from math import inf

def nearest_slow(ar, t):
    """
    O(n), simply scan in both directions from the starting index.
    """
    assert (ar is not None) and (len(ar) > 0) and (t >= 0)
    # length of array
    n = len(ar)
    # starting indices, on both sides
    li, ri = t - 1, t + 1
    # scan left, stop when larger
    while li > -1:
        if ar[li] > ar[t]: break
        li = li - 1
    # scan right, stop when larger
    while ri < n:
        if ar[ri] > ar[t]: break
        ri = ri + 1
    # if both li and ri are out of bounds, return None
    if (li < 0) and (ri > n - 1): return None
    # if li is out of bounds, return ri
    if li < 0: return ri
    # ri is out of bounds, return li
    if ri > n - 1: return li
    # else return whichever one is closer to i
    return li if (t - li <= ri - t) else ri

def nearest_fast(ar, t):
    """
    technically still O(n) due to preprocessing of the array, but the selection
    of the nearest index is O(1) due to lookup in a dictionary.
    """
    assert (ar is not None) and (len(ar) > 0) and (t >= 0)
    # length of array
    n = len(ar)
    # index of nearest largest, distance from l to t
    l = None
    dl = inf
    # preprocess by looping through the array and comparing distance
    for i in range(n):
        if (ar[i] > ar[t]) and (i - t < dl): l, dl = i, abs(i - t)
    # return l (could be None; technically this is O(1) i guess)
    return l

# main
if __name__ == "__main__":
    func_a = nearest_slow
    func_b = nearest_fast
    # problem input, answer is 3
    ar = [4, 1, 3, 5, 6]
    t = 0
    ufunc_eval(func_a, ar, t)
    ufunc_eval(func_b, ar, t)
    # another input, answer is None
    ar = [5, 1, 6, 5, 13, 8, 2, 9, 7]
    t = 4
    ufunc_eval(func_a, ar, t)
    ufunc_eval(func_b, ar, t)
