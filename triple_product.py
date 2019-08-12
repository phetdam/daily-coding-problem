# daily coding problem #69 (easy, but still not the optimal solution)
#
# Changelog:
#
# 08-11-2019
#
# initial creation. wasn't sure what would be an optimal way of doing this, as
# it's obvious that O(n^3) is simply too slow. the sorting version if faster
# (assuming quick sort or merge sort is used), but still not the optimal method,
# which i checked on geeksforgeeks and is O(n). that implementation is harder.

__doc__ = """
This problem was asked by Facebook.

Given a list of integers, return the largest product that can be made by
multiplying any three integers. For example, if the list is [-10, -10, 5, 2],
we should return 500, since that's -10 * -10 * 5. You can assume the list has at
least three integers.
"""

# import evaluation function
from ufunc_eval import ufunc_eval
from math import inf

def triple_sum_naive(ar):
    """
    O(n^3) naive method of finding the greatest triple sum. note that the
    entries in the array are allowed to be negative, so we cannot just take
    three O(n) max passes and then call it a day.
    """
    assert (ar is not None) and (len(ar) > 2)
    n = len(ar)
    # max product
    max_prod = -inf
    # for all possible combinations of three elements
    for i in range(n):
        for j in range(i + 1, n):
            for k in range(j + 1, n):
                max_prod = max(max_prod, ar[i] * ar[j] * ar[k])
    return max_prod

def triple_sum_sort(x):
    """
    O(nlogn) solution, slightly better. sort all the elements and then return
    max of product of max three elements and then product of two smallest and
    the max element. this handles the case of negative integers.
    """
    assert (ar is not None) and (len(ar) > 2)
    n = len(ar)
    # sorted array
    sar = sorted(ar)
    # return max
    return max(sar[0] * sar[1] * sar[-1], sar[-3] * sar[-2] * sar[-1])

# main
if __name__ == "__main__":
    func_a = triple_sum_naive
    func_b = triple_sum_sort
    # problem input, answer is 500
    ar = [-10, -10, 5, 2]
    ufunc_eval(func_a, ar)
    ufunc_eval(func_b, ar)
    # random input, answer is 304920
    ar = [-45, 22, -121, 7, -90, -11, -23, 28, 2]
    ufunc_eval(func_a, ar)
    ufunc_eval(func_b, ar)
