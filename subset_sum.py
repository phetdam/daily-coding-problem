# daily coding problem #42 (hard, not really if you can do subset sum)
#
# Changelog:
#
# initial creation. this one took me forever to do initially because i didn't
# remember how to do dp subset sum, but after some thinking, i got it. to do the
# backtracking for the actual elements, i used the fact that the lookup array
# tests for membership s.t. ar[c - 1] is included if from some point look[r][c],
# we have that look[r - ar[c - 1][c - 1] is True (and also within array bounds).

__doc__ = """
This problem was asked by Google.

Given a list of integers S and a target number k, write a function that returns
a subset of S that adds up to k. If such a subset cannot be made, then return
null. Integers can appear more than once in the list. You may assume all numbers
in the list are positive. For example, given S = [12, 1, 61, 5, 9, 2] and k = 24,
return [12, 9, 2, 1] since it sums up to 24.
"""

# import evaluation function
from ufunc_eval import ufunc_eval

def subset(ar, k):
    """
    O(kn) solution, where n is the number of elements in the set, and k is the
    target sum. performs dp subset sum in quasi-polynomial time and then
    backtracks through the O(nk) table to retrieve the elements used in the sum.
    """
    # sanity check
    assert (ar is not None) and (k >= 0)
    # get length of ar
    n = len(ar)
    # create lookup table
    look = [[False for _ in range(n + 1)] for _ in range(k + 1)]
    look[0] = [True for _ in range(n + 1)]
    # fill using bottom up approach
    for r in range(1, k + 1):
        for c in range(1, n + 1):
            # need to index ar[c - 1] due to 0 indexing
            look[r][c] = look[r][c - 1] or (r - ar[c - 1] >= 0 and
                                            look[r - ar[c - 1]][c - 1])
    # if we cannot make the subset sum to k, return None
    if look[k][n] == False: return None
    # else backtrack from look[k][n] to get the elements used in sum
    # list of elements used in the subset sum
    sl = []
    # row and column indices
    r = k
    c = n
    # while we have not reached the row 0 (finished subset sum; note that
    # since we have a proper subset sum, this is guaranteed for c >= 0)
    while (r > 0):
        # if r - ar[c - 1] >= 0 (i.e. it may be part of the sum) and
        # look[r - ar[c - 1]][c - 1] is True (i.e. it is part of the sum),
        # append ar[c - 1] to sl
        if (r - ar[c - 1] >= 0) and (look[r - ar[c - 1]][c - 1] == True):
            # append to sl and backtrack row (include ar[c - 1] in subset)
            sl.append(ar[c - 1])
            r = r - ar[c - 1]
        # else only backtrack column (exclude ar[c - 1] from subset)
        c = c - 1
    # return sl
    return sl

# main
if __name__ == "__main__":
    func = subset
    # problem input, answer is [12, 9, 2, 1]; order does not matter
    ar = [12, 1, 61, 5, 9, 2]
    k = 24
    ufunc_eval(func, ar, k)
    # another input, answer is None
    ar = [5, 4, 7, 1, 23, 11]
    k = 25
    ufunc_eval(func, ar, k)
    # another input, answer is [19, 14, 65]
    ar = [1, 4, 65, 14, 19, 3, 2, 1, 2, 100]
    k = 98
    ufunc_eval(func, ar, k)
