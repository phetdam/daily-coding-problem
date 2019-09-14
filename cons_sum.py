# daily coding problem #102 (medium)
#
# Changelog:
#
# 09-14-2019
#
# initial creation. used sliding window technique for contiguous elements, so
# the runtime of the function is O(n).

__doc__ = """
This problem was asked by Lyft.

Given a list of integers and a number K, return which contiguous elements of the
list sum to K. For example, if the list is [1, 2, 3, 4, 5] and K is 9, then it
should return [2, 3, 4], since 2 + 3 + 4 = 9.
"""

# import evaluation function
from ufunc_eval import ufunc_eval

def cons_sum(ar, k):
    """
    given an array of integers ar and value k, returns, if possible, a list of
    contiguous elements that sum up to k, else it will return None. uses sliding
    window and allows k or any other array values to be negative. O(n).
    """
    assert (ar is not None) and (len(ar) > 0)
    # get length of array
    n = len(ar)
    # beginning index of sequence, ending index of sequence
    si = ei = 0
    # temporary sum
    temp = ar[si]
    # while temp does not equal k and while ei has not reached end of array
    while (temp != k) and (ei < n):
        # if temp + ar[ei + 1] <= k, update temp. increment ei
        if temp + ar[ei + 1] <= k:
            temp = temp + ar[ei + 1]
            ei = ei + 1
            # else temp + ar[ei + 1] > k, increment si, set ei to max(ei, si),
            # and subtract ar[si] from ar since it is not included anymore
        else:
            temp = temp - ar[si]
            si = si + 1
            ei = max(ei, si)
    # if temp != k, return None
    if temp != k: return None
    # else return the subarray ar[si:(ei + 1)]
    return ar[si:(ei + 1)]

# main
if __name__ == "__main__":
    func = cons_sum
    # problem input, answer is [2, 3, 4]
    ar = [1, 2, 3, 4, 5]
    k = 9
    ufunc_eval(func, ar, k)
    # another input with same k, answer is [2, 4, 3]
    ar = [5, 2, 4, 3, 1]
    ufunc_eval(func, ar, k)
    # another input, answer is [7, 1, 8, 12]
    ar = [5, 3, 7, 1, 8, 12, 6, 3]
    k = 28
    ufunc_eval(func, ar, k)
