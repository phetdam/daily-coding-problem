# daily coding problem #90 (medium)
#
# Changelog:
#
# 06-04-2019
#
# initial creation. this is the the "stupid" way, i.e. generating random numbers
# until we get one that is not in our exclusion list ar. it seems silly, but is
# O(k) in the probabilistic sense, where k is the length of the exclusion list.
# the number of times that we have to generate in order to get a value not in
# ar follows a binomial distribution, where p = k / n, since we are selecting
# uniformly. since the expectation is np = k, we are probabilistic O(k), which
# is the fastest possible. note that we are excluding the O(k) traversal through
# the through the exclusion list; technically we are probabilistic O(k^2),
# although we can achieve probabilistic O(k) by first converting ar to a
# dictionary (which we do).

__doc__ = """
Given an integer n and a list of integers l, write a function that randomly
generates a number from 0 to n-1 that isn't in l (uniform).
"""

# import evaluation function
from ufunc_eval import ufunc_eval
# randint from random
from random import randint

def xrandint(n, ar):
    """
    uniformly generates random integers in the range [0, n - 1] except for
    those that are in the exclusion list ar. O(k) run and space.
    """
    assert (n > 0) and (ar is not None)
    # make dictionary out of the array for O(1) lookup
    look = {ar[i]: None for i in range(len(ar))}
    # generate once, and if u is in look, repeat
    u = randint(0, n - 1)
    while u in look: u = randint(0, n - 1)
    # return
    return u

# main
if __name__ == "__main__":
    func = xrandint
    # generate a bunch of random numbers, except for those in ar
    ar = [5, 4, 7, 2, 8, 22, 19]
    n = 50
    k = 10
    for _ in range(k): ufunc_eval(func, n, ar)
