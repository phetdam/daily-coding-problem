# daily coding problem #197 (easy)
#
# Changelog:
#
# 12-21-2019
#
# initial creation. added two solutions; one is the O(nk) in-place, vanilla swap
# method, while the other is an O(n) solution that requires O(n) extra space,
# although it is also rotates the array in-place.

__doc__ = """
This problem was asked by Amazon.

Given an array and a number k that's smaller than the length of the array,
rotate the array to the right k elements in-place.
"""

# import evaluation function
from ufunc_eval import ufunc_eval

def slow_rotate(ar, k):
    """
    O(nk) solution that naively shifts all elements to the right k times.
    """
    assert (ar is not None) and (k >= 0)
    # get length of array
    n = len(ar)
    # trivial return
    if n == 0: return ar
    # if k >= len(ar), simply subtract len(ar) from it
    if k >= n: k = k - n
    # perform k shifts
    for i in range(k):
        # shift all the elements to the right by 1; put last element at front
        last = ar[-1]
        for i in range(n - 1):
            ar[n - i - 1] = ar[n - i - 2]
        ar[0] = last
    # return ar
    return ar

def fast_rotate(ar, k):
    """
    O(n) solution requiring O(n) extra space. better for large n but also has
    memory allocation overhead due to the space requirement. computes final
    position for each element, stores (element, index) pairs.
    """
    assert (ar is not None) and (k >= 0)
    # get length of array
    n = len(ar)
    # trivial return
    if (n == 0) or (k == 0): return ar
    # if k >= len(ar), simply subtract len(ar) from it
    if k >= n: k = k - n
    # compute (item, index) pairs for each element in ar
    pairs = [None for _ in range(n)]
    for i in range(n):
        # either increase index by k; if i + k > n - 1, subtract n from sum
        pairs[i] = (ar[i], (i + k) if (i + k < n) else (i + k - n))
    # put elements directly where they belong
    for i in range(n): ar[pairs[i][1]] = pairs[i][0]
    # return ar
    return ar

# main
if __name__ == "__main__":
    func_a, func_b = slow_rotate, fast_rotate
    # input, answer is [5, 4, 6, 2, 4, 9, 10]
    ar, k = [2, 4, 9, 10, 5, 4, 6], 3
    br = [e for e in ar]
    ufunc_eval(func_a, ar, k)
    ufunc_eval(func_b, br, k)
