# daily coding problem #4 (hard; literally had no idea how to solve it besides
# thinking about the binary search approach which only works when all the values
# are positive).
#
# Changelog:
#
# 06-07-2019
#
# figured out and implemented the solution. after shifting all the negative
# values to one side using a lomuto partition inspired scheme, we have the value
# fp, which indicates the location of the first nonnegative value in the array.
# we then adopt a relative 1-indexing scheme, where fp is index 1. the absolute
# value of any A[i], i >= fp, is recorded and if |A[i]| + fp - 1 is within the
# array index, we flip the sign of the value there to indicate the presence of
# |A[i]| in the array. we return the first index i >= fp where A[i] > 0, which
# indicates that value i - fp + 1 is the smallest positive number missing. this
# is also true when i == len(A). note that since we are changing signs, we only
# change the signs of those indices that still contain a positive value.
#
# 06-06-2019
#
# initial creation. didn't know what the hell to do so i looked at the solution
# on geeksforgeeks. very clever solution.

__doc__ = """
This problem was asked by Stripe.

Given an array of integers, find the first missing positive integer in linear
time and constant space. In other words, find the lowest positive integer that
does not exist in the array. The array can contain duplicates and negative
numbers as well. For example, the input [3, 4, -1, 1] should give 2. The input
[1, 2, 0] should give 3. You can modify the input array in-place.
"""

# import evaluation function
from ufunc_eval import ufunc_eval

def lmissing(A):
    """
    O(n) solution. the geeksforgeeks solution: shift all the negative numbers
    to the left hand side of the array (i decided to use the lomuto partition
    which we studied in algos). could have used a smarter scheme but whatever.
    """
    # index that marks the position of the first positive number
    fp = 0
    # for all elements in A
    for i in range(len(A)):
        # if A[i] is not positive (this is important!)
        if A[i] < 1:
            # if A[fp] is positive, swap the two
            if A[fp] > 0:
                temp = A[fp]
                A[fp] = A[i]
                A[i] = temp
            # else only advance fp; advance it as long as A[i] < 0
            fp = fp + 1
        # else just advance i and do nothing with fp
    # if fp exceeds index range, return 1; all elements are negative
    if fp > len(A) - 1: return 1
    # else if we find the number abs(k) at an index i, flip sign of A[i + k - 1]
    # if the index i + k - 1 is within the array range. we subtract by 1 to
    # effectively use 1-indexing. we need to use absolute value since some
    # values will be negative due to prior sign changes, and we need to check
    # that the cell is positive so we don't accidentally reset a cell that had
    # its sign flipped.
    for i in range(fp, len(A)):
        k = abs(A[i])
        if fp + k - 1 < len(A) and A[fp + k - 1] > 0:
            A[fp + k - 1] = -A[fp + k - 1]
    # return index of first int k > 0 minus fp - 1 for the answer, starting
    # fp so we consider only the known segment of nonnegative values.
    for i in range(fp, len(A)):
        if A[i] > 0: return i - (fp - 1)
    # if we exhaust the loop, then return len(A) - (fp - 1)
    return len(A) - (fp - 1)

# main
if __name__ == "__main__":
    A = [6, 3, 4, 5, 3, 4, 5, -1, -43, -12, -9]
    B = sorted(A)
    func = lmissing
    ufunc_eval(func, A)
    print("sorted: {0}".format(B))
