# daily coding problem #138 (hard, takes a little thought)
#
# Changelog:
#
# 11-15-2019
#
# initial creation. i stayed away from this one for the longest time until today
# but it is done now. the naive recursive solution is of course much better
# improved through use of dynamic programming.

__doc__ = """
This problem was asked by Google.

Find the minimum number of coins required to make n cents.

You can use standard American denominations, that is, 1c, 5c, 10c, and 25c. For
example, given n = 16, return 3 since we can make it with a 10c, a 5c, and a 1c.
"""

# import evaluation function
from ufunc_eval import ufunc_eval
from math import inf

def min_coins_slow(n, coins = [1, 5, 10, 25]):
    """
    the slow recursive solution to the problem. we generalize by allowing a list
    coins to hold whatever types of coins that you want, although it defaults to
    the american coins, i.e. [1, 5, 10, 25]. exponential runtime. note that if
    there is no possible summation to n, inf is returned.
    """
    # sanity check; use to indicate wrong answer as well
    if n < 0: return inf
    # base case
    if n == 0: return 0
    # minimum number of coins
    mc = inf
    # try for each coin in the list
    for c in coins:
        # if n == c, return 1
        if n == c: return 1
        # else if n > c, set mc as min between mc and recursive result
        elif n > c: mc = min(mc, 1 + min_coins_slow(n - c, coins = coins))
    # return answer
    return mc

def min_coins_fast(n, coins = [1, 5, 10, 25], look = None):
    """
    top-down dynamic programming variant. we avoid unnecessary recursion by
    referring to a lookup table that we maintain during runtime. O(n) space
    and runtime reduced to O(mn), where m is the number of coins.
    """
    # base cases
    if n < 0: return inf
    # if look is None, we need to make our lookup table
    if look is None:
        look = [inf for _ in range(n + 1)]
        look[0] = 0
    # if look[n] computed, return that instead
    if look[n] < inf: return look[n]
    # else for each coin in the list, we need to compute look[n]
    for c in coins:
        look[n] = min(look[n], 1 + min_coins_fast(n - c, coins = coins,
                                                  look = look))
    # return answer
    return look[n]

# main
if __name__ == "__main__":
    func_a, func_b = min_coins_slow, min_coins_fast
    # problem input, answer is 3
    n = 16
    ufunc_eval(func_a, n)
    ufunc_eval(func_b, n)
    # another input, answer is 5; notice the delay for func_a
    n = 42
    ufunc_eval(func_a, n)
    ufunc_eval(func_b, n)
    # another input; does not have an answer
    n = 11
    coins = [3, 6]
    ufunc_eval(func_a, n, coins = coins)
    ufunc_eval(func_b, n, coins = coins)
