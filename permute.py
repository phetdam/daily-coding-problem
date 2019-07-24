# daily coding problem #51 (medium, but it's quite easy honestly)
#
# Changelog:
#
# 07-23-2019
#
# initial creation. just remember that to select each with equal probability,
# you cannot select any previously filled indices. everything else follows.

__doc__ = """
This problem was asked by Facebook.

Given a function that generates perfectly random numbers between 1 and k
(inclusive), where k is an input, write a function that shuffles a deck of cards
represented as an array using only swaps. It should run in O(N) time.

Hint: Make sure each one of the 52! permutations of the deck is equally likely.
"""

# import evaluation function
from ufunc_eval import ufunc_eval
# radnint function
from random import randint

def randnat(k):
    """
    wrapper around randint that returns with uniform probability an integer x in
    the range of [1, k]. just to make the problem specification explicit.
    """
    return randint(1, k)

def deck_shuffle(n = 52):
    """
    if we shuffle a deck of cards, we are simply permuting it. enough said.
    since randnat() is O(1), this is an O(n) solution.
    """
    # create array of cards, 1 to n
    ar = [i + 1 for i in range(n)]
    # for each index i, select the remaining n - i cards with equal probability
    for i in range(n):
        # select one of the remaining cards (decrease by 1 due to zero-indexing,
        # but offset by i to prevent selection of the previous i
        si = i + randnat(n - i) - 1
        # swap
        ar[i], ar[si] = ar[si], ar[i]
    # return permuted deck
    return ar

# main
if __name__ == "__main__":
    func = deck_shuffle
    ufunc_eval(func)
