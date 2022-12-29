# daily coding problem #18 (hard, but actually not that hard if you spend some
# time to really think about it. a good problem.)
#
# Changelog:
#
# 06-20-2019
#
# initial creation. added a pretty decent solution if i do say so myself. read
# the docstring for the function for my reasoning. the second evaluation uses
# non-deterministic input so each time you run it the result will change.

__doc__ = """
This problem was asked by Google.

Given an array of integers and a number k, where 1 <= k <= length of the array,
compute the maximum values of each subarray of length k.

For example, given array = [10, 5, 2, 7, 8, 7] and k = 3, we should get:
[10, 7, 8, 8], since:

10 = max(10, 5, 2)
7 = max(5, 2, 7)
8 = max(2, 7, 8)
8 = max(7, 8, 7)

Do this in O(n) time and O(k) space. You can modify the input array in-place and
you do not need to store the results. You can simply print them out as you
compute them.
"""

# import evaluation function
from ufunc_eval import ufunc_eval

# import math for math.inf
from math import inf

# generate random integers
from random import randint

def ksubmax(A, k):
    """
    O(n) solution, with O(1) space not counting the storage used to store the
    supremums for each subarray of length k (i.e. contiguous subset of the
    ordered set A). we don't have to store, but i did for convenience. the naive
    method would be for each window of k elements, to take the max and print it,
    but that would suffer a complexity of O(k(n - k + 1)), where if k was n / 2,
    would result in an O(n^2) complexity. therefore, the solution is to only
    use the max when necessary: we remember the last max taken in previous
    windows and its position, so we only need to update with each new value that
    we see in the array. this brings the runtime down to O(n).
    """
    # beginning and ending index of sliding window of k contiguous elements
    si = 0
    ei = k - 1
    # current largest/max and the index of the current max
    cl = cli = -inf
    # list of supremums for each window of k elements
    ml = []
    # while we have not reached the last sliding window
    while ei < len(A):
        # if the max is outside the window, we need to update
        if cli < si:
            # reset cl to -inf
            cl = -inf
            # find new max in A[si...ei] and index of max
            for i in range(si, ei + 1):
                if A[i] > cl:
                    cl = A[i]
                    cli = i
        # else the max is still inside the window, so we only need to compare
        # to the newest element introduced into the window, which is A[ei]
        # if A[ei] >= cl, then we need to update cl and cli first
        if A[ei] >= cl:
            cl = A[ei]
            cli = ei
        # else just append cl to ml
        ml.append(cl)
        # increment si and ei to move the window
        si = si + 1
        ei = ei + 1
    # return ml
    return ml

# main
if __name__ == "__main__":
    # input from the example
    A = [10, 5, 2, 7, 8, 7]
    k = 3
    ufunc_eval(ksubmax, A, k)
    # "special" input
    # limiting integer
    limit = 20
    A = [randint(-10 * limit, 10 * limit)
         for _ in range(randint(limit // 2, limit))]
    k = randint(1, limit)
    ufunc_eval(ksubmax, A, k)
