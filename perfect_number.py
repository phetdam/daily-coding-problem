# daily coding problem #70 (easy)
#
# Changelog:
#
# 08-11-2019
#
# initial creation. by the way, this is not the definition of a perfect number.
# geeksforgeeks has a constant time "solution", but i think it is wrong since it
# drastically undercounts the number of outliers from the 19 + 9 * n "rule".

__doc__ = """
This problem was asked by Microsoft.

A number is considered perfect if its digits sum up to exactly 10. Given a
positive integer n, return the n-th perfect number. For example, given 1, you
should return 19. Given 2, you should return 28.
"""

# import evaluation function
from ufunc_eval import ufunc_eval

def nth_perfect(n):
    """
    O(n) solution. we can observe that the first perfect number is 19, and all
    others are necessarily a multiple of 19 + 9 * k, for natural k. however,
    this is not a sufficient condition, and we must check for outliers.
    """
    assert n > 0
    # starting number
    k = 10
    # while n is not 0
    while n > 0:
        # add 9 to k
        k = k + 9
        # check that all digits add up to 10
        # temp for k, sum of digits
        t = k
        ds = 0
        while t > 0:
            # get digit value and add to ds; remove from t
            ds = ds + (t % 10)
            t = t // 10
        # if ds == 10, then decrement n
        if ds == 10: n = n - 1
    # return k
    return k

# main
if __name__ == "__main__":
    func = nth_perfect
    # problem input (trivial)
    n = 2
    ufunc_eval(func, n)
    # harder input, answer should be 109
    n = 10
    ufunc_eval(func, n)
    # harder input, answer should be 406
    n = 37
    ufunc_eval(func, n)
