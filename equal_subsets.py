# daily coding problem #60 (medium)
#
# Changelog:
#
# 08-09-2019
#
# initial creation. again, been behind on the problems. the key insight here is
# that if you know the sum of the elements is odd, you must return False, and if
# not, then you just try to find a subset sum or sum(ar) // 2. to make it fast,
# use DP to get quasi-polynomial time for the subset sum algorithm.

__doc__ = """
This problem was asked by Facebook.

Given a multiset of integers, return whether it can be partitioned into two
subsets whose sums are the same. For example, given the multiset {15, 5, 20, 10,
35, 15, 10}, it would return true, since we can split it up into {15, 5, 10, 15,
10} and {20, 35}, which both add up to 55. Given the multiset {15, 5, 20, 10,
35}, it would return false, since we can't split it up into two subsets that add
up to the same sum.
"""

# import evaluation function
from ufunc_eval import ufunc_eval

def equal_split(ar):
    """
    O(kn) solution, where k is the target sum, and n is the number of elements
    in ar. this is subset sum, but our target sum is half sum(ar). use DP for
    subset sum and use first check to eliminate unnecessary subset sums.
    """
    # sanity check
    assert ar is not None
    # get sum of ar
    ar_sum = sum(ar)
    # if ar_sum is not divisble by 2, we definitely return False
    if ar_sum % 2 > 0: return False
    # get length of ar and the target value
    n = len(ar)
    k = ar_sum // 2
    # create lookup array (extra row and column for convenience)
    look = [[False for _ in range(n + 1)] for _ in range(k + 1)]
    # first row must be True
    look[0] = [True for _ in range(n + 1)]
    # fill lookup table bottom up; for each element ar[0], ... ar[n - 1], we
    # will either include it or not include it.
    for r in range(1, k + 1):
        for c in range(1, n + 1):
            # includes test to make sure that ar[c - 1] can actually be included
            look[r][c] = look[r][c - 1] or (r - ar[c - 1] >= 0 and
                                            look[r - ar[c - 1]][c - 1])
    # answer is the final entry
    return look[k][n]

# main
if __name__ == "__main__":
    func = equal_split
    # problem input, answer is True
    ar = [15, 5, 20, 10, 35, 15, 10]
    ufunc_eval(func, ar)
    # another problem input, answer is False
    ar = [15, 5, 20, 10, 35]
    ufunc_eval(func, ar)
    # more input, answer is True
    ar = [2, 5, 10, 1, 16, 24, 8]
    ufunc_eval(func, ar)
    # another input, answer is False
    ar = [1, 1, 2, 100]
    ufunc_eval(func, ar)
