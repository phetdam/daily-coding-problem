# daily coding problem #47 (easy, i don't think my solutio was most efficient)
#
# Changelog:
#
# 07-19-2019
#
# initial creation. haven't done one of these in a while since i have been busy
# with work. i think i did a problem similar to this before however.

__doc__ = """
This problem was asked by Facebook.

Given a array of numbers representing the stock prices of a company in
chronological order, write a function that calculates the maximum profit you
could have made from buying and selling that stock once. You must buy before you
can sell it. For example, given [9, 11, 8, 5, 7, 10], you should return 5, since
you could buy the stock at 5 dollars and sell it at 10 dollars.
"""

# import evaluation function
from ufunc_eval import ufunc_eval
# import inf
from math import inf

def stock_strat(ar, l = 0, r = None):
    """
    O(nlogn) solution. divide and conquer solution to the problem. we don't need
    a wrapper by using optional arguments. for convenience, we assume that we
    can buy and sell on the same day for a profit of 0.
    """
    # wrapper: if r is None, set it to length - 1
    if r is None: r = len(ar) - 1
    # if l equals r, we can only return 0
    if l == r: return 0
    # get middle index
    m = (l + r) // 2
    # get profit from trading left half and right half separately
    lp = stock_strat(ar, l = l, r = m)
    rp = stock_strat(ar, l = m + 1, r = r)
    # get min of left half, max of right half
    lo = inf
    for i in range(m, l - 1, -1): lo = min(lo, ar[i])
    hi = -inf
    for i in range(m + 1, r + 1): hi = max(hi, ar[i])
    # middle profit is therefore the difference
    mp = hi - lo
    # return max of all three
    return max(lp, rp, mp)

# main
if __name__ == "__main__":
    func = stock_strat
    # given problem input
    ar = [9, 11, 8, 5, 7, 10]
    ufunc_eval(func, ar)
    # another random input
    ar = [1, 9, 43, 7, 3, 4, 2, 76, 5, 2]
    ufunc_eval(func, ar)
