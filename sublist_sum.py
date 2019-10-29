# daily coding problem #149 (hard, but it's easy? so confused)
#
# Changelog:
#
# 10-29-2019
#
# initial creation. not sure why this is hard; the problem statement is the only
# thing about this that is hard (since it is confusing). you could just do this
# faster without relying on the builtin sum() function.

__doc__ = """
This problem was asked by Goldman Sachs.

Given a list of numbers L, implement a method sum(i, j) which returns the sum
from the sublist L[i:j] (including i, excluding j).

For example, given L = [1, 2, 3, 4, 5], sum(1, 3) should return sum([2, 3]),
which is 5. You can assume that you can do some pre-processing. sum() should be
optimized over the pre-processing step.
"""

# import evaluation function
from ufunc_eval import ufunc_eval

def subsum(ar, i, j):
    """
    returns sum() on the desired sublist from ar
    """
    assert (ar is not None) and (i > -1)
    # since python will truncate ranges, i.e. if length is n, and i > n - 1, it
    # will not throw any exception due to the smart indexing. so just call sum()
    return sum(ar[i:j])

def subsum_m(ar, i, j):
    """
    can also calculate the sublist sum manually
    """
    assert (ar is not None) and (i > -1)
    # total is 0
    tot = 0
    # start at i and end at min(len(ar), j)
    for c in range(i, min(len(ar), j)):
        tot = tot + ar[c]
    # return answer
    return tot

# main
if __name__ == "__main__":
    func_a = subsum
    func_b = subsum_m
    # problem input, answer is 5
    ar = [1, 2, 3, 4, 5]
    i, j = 1, 3
    ufunc_eval(func_a, ar, i, j)
    ufunc_eval(func_b, ar, i, j)
    # another input, answer is 34 (purposefully set j to be too large)
    ar = [5, 2, 5, 6, 2, 56, 2, 9, 1, 22]
    i, j = 6, 100
    ufunc_eval(func_a, ar, i, j)
    ufunc_eval(func_b, ar, i, j)
