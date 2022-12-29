# daily coding problem #58 (medium)
#
# Changelog:
#
# 07-30-2019
#
# initial creation. took a little bit of thinking before i realized that it is
# possible to take advantage of the sorting order to apply binary search. the
# difference is that instead of testing whether k > M or k < M (M is the middle
# element in the partition), we have to check the endpoints of the ranges.

__doc__ = """
This problem was asked by Amazon.

An sorted array of integers was rotated an unknown number of times.

Given such an array, find the index of the element in the array in faster than
linear time. If the element doesn't exist in the array, return null. For example,
given the array [13, 18, 25, 2, 8, 10] and the element 8, return 4 (the index of
8 in the array). You can assume all the integers in the array are unique.
"""

# import evaluation function
from ufunc_eval import ufunc_eval

def rotated_bs(ar, k, l = 0, r = None):
    """
    O(logn) solution as required. we make use of the fact that since all the
    values in the array are in sorted order, any range [a, b] is in ascending
    order, although we may reach the end of the largest values before starting
    again with smaller values. so instead of using just the middle element
    as selection criteria, we use the endpoints of the left and right ranges.
    """
    # assume that array is not None and has length greater than 0
    # takes place of wrapper: if r is None, set to length of ar - 1
    if r is None: r = len(ar) - 1
    # if l == r, return l if ar[l] == k, else return None
    if l == r:
        if ar[l] == k: return l
        return None
    # get middle index to partition range
    m = (l + r) // 2
    # best case scenarios
    if ar[m] == k: return m
    elif ar[l] == k: return l
    elif ar[r] == k: return r
    # denote ar[l] as L and ar[r] as R. choose left partition only if L < k < R
    # or if (L > R) and (k < R or k > L).
    if (((ar[l] < ar[m]) and (ar[l] < k) and (ar[m] > k)) or
        ((ar[l] > ar[m]) and (k < ar[m] or k > ar[l]))):
        return rotated_bs(ar, k, l = l, r = m)
    # go to right hand side only if M < k < R or (M > R and (k > M or k < R))
    return rotated_bs(ar, k, l = m + 1, r = r)

# main
if __name__ == "__main__":
    func = rotated_bs
    # problem input, answer is 4
    ar = [13, 18, 25, 2, 8, 10]
    k = 8
    ufunc_eval(func, ar, k)
    # another randomly chosen input, answer is 3
    ar = [7, 9, 13, 16, 78, 1, 2, 4, 5]
    k = 16
    ufunc_eval(func, ar, k)
