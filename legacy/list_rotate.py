# daily coding problem #126 (medium, definitely took some thinking)
#
# Changelog:
#
# 10-11-2019
#
# initial creation. got better than O(n^2) from the naive swapping method; did
# not perform formal algorithmic analysis but it is definitely faster. the
# method relies on an initial n, k. if k >= n, then we set k := k - n. we have
# a starting index si := 0, and while k > 0 (we still have to swap elements),
# we swap each index from si + k to si + n - 1, i, with i - k. however, on
# each pass, we only swap n - k elements; the remaining k elements are still
# shifted by n % k, so we set n := k and k := k - (n % k) simultaneously, and
# set si := si + n - k to consider the last remaining (n % k)-shifted k elements
# left in the array. we repeat the process until k == 0, then we return.

__doc__ = """
This problem was asked by Facebook.

Write a function that rotates a list by k elements. For example, [1, 2, 3, 4, 5,
6] rotated by two becomes [3, 4, 5, 6, 1, 2]. Try solving this without creating
a copy of the list. How many swap or move operations do you need?
"""

# import evaluation function
from ufunc_eval import ufunc_eval

def k_rotate(ar, k):
    """
    the O(n) approach is to make another list, for O(n) swaps. to do it in-place
    without taking advantage of appending or popping from the python list (which
    anyways involved memory re-allocation internally), we can only swap. what we
    do instead of slowing shuffling all the elements left, which would be O(n^2)
    in complexity, is to start at the kth index and swap each element at i with
    its corresponding neighbor at i - k. if n % k is not 0, then we have k
    items that need to be re-shifted k - (n % k) times, as they are shifted
    n % k places, and we continue this process until k == 0.
    """
    assert (ar is not None) and (k >= 0)
    # length of array
    n = len(ar)
    # degenerate cases
    if (n == 0) or (k == 0): return ar
    # if k >= n, set k to k = k - n
    if k >= n: k = k - n
    # starting swap index; begins at 0
    si = 0
    # while k > 0
    while (k > 0):
        # start from index i := k, and swap with index i - k to the end
        for i in range(si + k, si + n):
            ar[i], ar[i - k] = ar[i - k], ar[i]
        # add n - k to si
        si = si + (n - k)
        # set n to k; may have k values that still need n % k shifts, and set
        # k to k - (n % k), as the subarray is incorrectly shifted n % k spots.
        n, k = k, k - (n % k)
        # if k >= n, set k to k = k - n
        if k >= n: k = k - n
    # return k-left rotated array
    return ar

# main
if __name__ == "__main__":
    func = k_rotate
    # to make everything easy to check visually, we make each array a list of
    # consecutive increasing natural numbers. n controls cardinality.
    # problem input, answer is [3, 4, 5, 6, 1, 2]
    n, k = 6, 2
    ar = [(i + 1) for i in range(n)]
    ufunc_eval(func, ar, k)
    # another input, answer is [7, 8, 9, ... 5, 6]
    n, k = 14, 6
    ar = [(i + 1) for i in range(n)]
    ufunc_eval(func, ar, k)
    # another input, answer is [9, 10, 11, ... 7, 8]
    n, k = 19, 8
    ar = [(i + 1) for i in range(n)]
    ufunc_eval(func, ar, k)
