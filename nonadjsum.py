# daily coding problem #9 (hard, actually hard. i was very stuck)
#
# Changelog:
#
# 06-11-2019
#
# initial creation. changed "and" to "+" in the problem statement to save space.
# honestly had no idea how to solve it, so geeksforgeeks came in clutch again.
# modified the original solution in order to handle cases where all the numbers
# in the array can be negative, since 0 would be erroneously returned.

__doc__ = """
This problem was asked by Airbnb.

Given a list of integers, write a function that returns the largest sum of
non-adjacent numbers. Numbers can be 0 or negative. For example, [2, 4, 6, 2, 5]
should return 13, since we pick 2, 6, and 5. [5, 1, 1, 5] should return 10,
since we pick 5 and 5. Follow-up: Can you do this in O(N) time + constant space?
"""

# import evaluation function
from ufunc_eval import ufunc_eval

def nonadjsum(A):
    """
    O(n) solution. we maintain two sums: for all elements A[i], there is a sum
    incl that will include the value of A[i], while there is a sum excl that
    will not include A[i]. both must be maximized. initially, incl is A[0],
    while excl is 0. starting from i = 1, we save the old value of incl to
    old_incl, set incl to excl plus A[i] (note that excl will never have an
    element adjacent to A[i]), and set excl to the max of itself and old_incl.
    this, unlike dynamic programming, takes no auxiliary space.
    """
    if (A is None) or (len(A) == 0): return None
    incl = A[0]
    excl = 0
    for i in range(1, len(A)):
        # save old value of incl, which includes A[i - 1]
        old_incl = incl
        # set incl to excl + A[i], so we exclude A[i - 1]
        incl = excl + A[i]
        # set excl to be the max of itself or old_incl; i.e. decide whether
        # including or excluding A[i - 1] leads to a larger maximum
        excl = max(excl, old_incl)
    # handle case of all values in A being negative: if incl < excl and
    # excl == 0, then return max(A)
    if (incl < excl) and (excl == 0): return max(A)
    # else return max of incl and excl
    return max(incl, excl)

# main
if __name__ == "__main__":
    # first given example
    A = [2, 4, 6, 2, 5]
    func = nonadjsum
    ufunc_eval(func, A)
    # arbitrary example, answer should be 10
    A = [-1, 2, 8, 6, 1, -5, 0, -4, 1]
    ufunc_eval(func, A)
    # the samr elements as above, but sorted; see how the answer depends on
    # the location of elements, as expected
    ufunc_eval(func, sorted(A))
    # another arbitary example, all elements are negative, so answer is -1
    A = [-1, -7, -3, -8, -9, -11, -1, -2]
    ufunc_eval(func, A)
