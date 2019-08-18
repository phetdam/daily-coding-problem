# daily coding problem #71 (easy)
#
# Changelog:
#
# 08-18-2019
#
# initial creation. this made me scratch my head for a while because i wanted to
# have rand5() be O(1), but this is the easy way to do it/

__doc__ = """
This problem was asked by Two Sigma.

Using a function rand7() that returns an integer from 1 to 7 (inclusive) with
uniform probability, implement a function rand5() that returns an integer from 1
to 5 (inclusive).
"""

# import evaluation function
from ufunc_eval import ufunc_eval
# return random integers uniformly
from random import randint

def rand7():
    """
    the function that we are given. returns 1, ... 7 inclusive uniformly
    """
    return randint(1, 7)

def rand5():
    """
    using rand7(), returns integers 1, ... 5 inclusive uniformly
    """
    # get integer from [1, 7]
    k = rand7()
    # while k > 5, continue to try again
    while k > 5: k = rand7()
    # else return k
    return k

# main
if __name__ == "__main__":
    # run the function a lot of times
    n = 20
    outl = [None for _ in range(n)]
    for i in range(n): outl[i] = rand5()
    print(outl)
