# daily coding problem #12 (hard, but i have done this kind of problem before)
#
# Changelog:
#
# 06-15-2019
#
# initial creation. this is similar to a "chocolate bar" problem i did in school
# so i already knew how to handle this type of problem. dp is the obvious
# solution since this function is monotonically non-decreasing in n, and because
# the subproblems overlap very nicely.

__doc__ = """
This problem was asked by Amazon.

There exists a staircase with N steps, and you can climb up either 1 or 2 steps
at a time. Given N, write a function that returns the number of unique ways you
can climb the staircase. The order of the steps matters. For example, if N is 4,
then there are 5 unique ways:

1, 1, 1, 1
2, 1, 1
1, 2, 1
1, 1, 2
2, 2

What if, instead of being able to climb 1 or 2 steps at a time, you could climb
any number from a set of positive integers X? For example, if X = {1, 3, 5}, you
could climb 1, 3, or 5 steps at a time.
"""

# import evaluation function
from ufunc_eval import ufunc_eval

def steps_r(n, steps = [1, 2]):
    """
    O(k^n) solution, where k is the number of steps that can be taken at a time,
    and n is the total number of steps. this is the native recursive method, and
    we use this to check our answer.
    """
    # base cases
    if n < 0: return 0 # if the step is too big
    if n == 0: return 1
    # result
    res = 0
    # for each step length, return number of ways to go up the steps starting
    # with that given step length
    for e in steps: res = res + steps_r(n - e, steps = steps)
    return res

def steps_dp(n, steps = [1, 2], look = None):
    """
    O(kn) solution, where k and n are as described in the docstring for steps_r.
    we use dynamic programming to memoize the past results and just reuse them.
    to avoid making a wrapper, we use the optional named argument to pass the
    lookup array in and out.
    """
    # wrapper; look[0] set to 1 for base case
    if look is None:
        look = [0 for _ in range(n + 1)]
        look[0] = 1
    # base cases
    if n < 0: return 0
    if look[n] > 0: return look[n]
    # else the value we want is not in the lookup array so recurse (look[n] = 0)
    for e in steps:
        look[n] = look[n] + steps_dp(n - e, steps = steps, look = look)
    # return look[n]
    return look[n]

# main
if __name__ == "__main__":
    # number of steps to climb. you will see the speed improvement from dp
    n = 30
    ufunc_eval(steps_r, n)
    ufunc_eval(steps_dp, n)
