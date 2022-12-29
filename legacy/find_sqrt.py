# daily coding problem #129 (medium, kind of requires prior knowledge to do)
#
# Changelog:
#
# 10-11-2019
#
# initial creation. definitely had no idea how to do this problem so i looked
# online and saw that newton's method is one of the common ways to compute the
# square root. so i implemented that. not sure how someone could do this if
# they don't know newton's method or another root-finding algorithm.

__doc__ = """
Given a real number n, find the square root of n. For example, given n = 9,
return 3.
"""

# import evaluation function
from ufunc_eval import ufunc_eval
# import sqrt function
from math import sqrt

def newton_sqrt(x, guess = None, tol = 10 ** (-6)):
    """
    find the square root of x by using newton's method to solve r^2 - x = 0,
    where r is the desired square root. our initial guess is by default x / 10.
    can adjust tolerance if desired.
    """
    assert x >= 0
    # set guess if None
    if guess is None: guess = x / 10
    # calculate first iterate
    new_guess = guess - (pow(guess, 2) - x) / (2 * guess)
    # while not within tolerance
    while abs(new_guess - guess) > tol:
        guess = new_guess
        new_guess = guess - (pow(guess, 2) - x) / (2 * guess)
    # return new_guess
    return new_guess

# main
if __name__ == "__main__":
    func = newton_sqrt
    # problem input, answer is 3
    n = 9
    ufunc_eval(func, n)
    print(sqrt(n))
    # another input, answer is ~11.4891
    n = 132
    ufunc_eval(func, n)
    print(sqrt(n))
    # another input, answer is 7
    n = 49
    ufunc_eval(func, n)
    print(sqrt(n))
