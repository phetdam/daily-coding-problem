# daily coding problem #124 (easy)
#
# Changelog:
#
# 10-10-2019
#
# verified answer with monte carlo routine. finally completed.
#
# 10-07-2019
#
# finally got around to looking at this again, but made no real changes.
#
# 10-04-2019
#
# initial creation. finished the avg_flips function but did not finish the monte
# carlo version that we will use to verify the answer.

__doc__ = """
This problem was asked by Microsoft.

You have n fair coins and you flip them all at the same time. Any that come up
tails you set aside. The ones that come up heads you flip again. How many rounds
do you expect to play before only one coin remains?

Write a function that, given n, returns the number of rounds you'd expect to
play until one coin remains.
"""

# import evaluation function
from ufunc_eval import ufunc_eval
# import logarithm
from math import log
# import random function
from random import random

def avg_flips(n):
    """
    since the probability of a fair coin getting heads is 1/2, we expect that
    with every round of tosses, we wipe out half of the coins. so the expected
    number of rounds, given n coins, is log2(n) if n is even, add one if odd.
    """
    assert n > 0
    if n % 2 == 0: return int(log(n, 2))
    return int(log(n, 2)) + 1

def avg_flips_mc(n, N = 1000):
    """
    uses monte carlo to determine the average number of flips. big N controls
    the number of simulations that are to be conducted to get an estimate.
    """
    assert (n > 0) and (N > 0)
    # total number of rounds played
    tsum = 0
    # for each of the N simulations
    for i in range(N):
        # temporary n
        nt = n
        # while number of coins is greater than 1
        while nt > 1:
            # tail count
            tc = 0
            # "flip" nt times, and every time we get a tails (0 <= x < 0.5),
            # increment tail count
            for i in range(nt):
                if random() < 0.5: tc = tc + 1
            # subtract tail count from nt; we discard tails
            nt = nt - tc
            # add 1 to tsum; one more round has been played
            tsum = tsum + 1
    # return tsum divided by N for monte carlo estimate (ceil if not int)
    if tsum % N == 0: return int(tsum / N)
    return int(tsum / N) + 1

# main
if __name__ == "__main__":
    func_a = avg_flips
    func_b = avg_flips_mc
    # compare expectation with monte carlo estimate, for differing n
    n = 98
    ufunc_eval(func_a, n)
    ufunc_eval(func_b, n)
    n = 47
    ufunc_eval(func_a, n)
    ufunc_eval(func_b, n)
    n = 333
    ufunc_eval(func_a, n)
    ufunc_eval(func_b, n)
    # note that as n gets bigger, the approximation gets closer. we could also
    # increase N:
    N = 10000
    n = 98
    ufunc_eval(func_a, n)
    ufunc_eval(func_b, n, N = N)
    n = 47
    ufunc_eval(func_a, n)
    ufunc_eval(func_b, n, N = N)
    n = 333
    ufunc_eval(func_a, n)
    ufunc_eval(func_b, n, N = N)
