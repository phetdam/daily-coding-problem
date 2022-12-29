# daily coding problem #49 (medium, but doesn't take too much thought)
#
# Changelog:
#
# 07-21-2019
#
# easy once you think it through. the important thing is to develop a criteria
# on when to reset the current sum to 0, as not all negative numbers should be
# excluded. the criteria for resetting the current sum is when a negative
# number encountered drops the sum below 0. else continue to max with max_sum.

__doc__ = """
This problem was asked by Amazon.

Given an array of numbers, find the maximum sum of any contiguous subarray of
the array. For example, given the array [34, -50, 42, 14, -5, 86], the maximum
sum would be 137, since we would take elements 42, 14, -5, and 86. Given the
array [-5, -1, -8, -9], the maximum sum would be 0, since we would not take any
elements. Do this in O(N) time.
"""

# import evaluation function
from ufunc_eval import ufunc_eval

def max_contig_sum(ar):
    """
    O(n). note that if all the elements in ar are positive, then the maximum
    contiguous sum is simply all the elements in the array. we are willing to
    add any negative elements to the array as long as there is no element that
    can make the current sum drop below 0, as then we cannot include that
    element or any other elements preceding it.
    """
    # assume array is length 1 or more
    n = len(ar)
    # max sum and current sum
    max_sum = cur_sum = 0
    # for each element in ar
    for i in range(n):
        # update cur_sum
        cur_sum = cur_sum + ar[i]
        # max with max_sum
        max_sum = max(max_sum, cur_sum)
        # if cur_sum is less than 0, we have to set it to 0
        if cur_sum < 0: cur_sum = 0
    return max_sum

# main
if __name__ == "__main__":
    func = max_contig_sum
    # first input given by the problem
    ar = [34, -50, 42, 14, -5, 86]
    ufunc_eval(func, ar)
    # second input given by the problem
    ar = [-5, -1, -8, -9]
    ufunc_eval(func, ar)
    # arbitrary array, answer should be 100
    ar = [5, 7, 4, -19, 10, 90, -100, 19, 15]
    ufunc_eval(func, ar)
