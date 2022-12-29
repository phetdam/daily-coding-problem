# daily coding problem #35 (hard, but not really)
#
# Changelog:
#
# 07-07-2019
#
# initial creation. this problem is actually quite easy to solve as long as you
# take advantage of the fact that there are only 3 characters to segregate, so
# you can perform a selection sort like in-place partitioning in O(n) easily.

__doc__ = """
This problem was asked by Google.

Given an array of strictly the characters 'R', 'G', and 'B', segregate the
values of the array so that all the Rs come first, the Gs come second, and the
Bs come last. You can only swap elements of the array. Do this in linear time
and in-place. For example, given the array ['G', 'B', 'R', 'R', 'B', 'R', 'G'],
it should become ['R', 'R', 'R', 'G', 'G', 'B', 'B'].
"""

# import evaluation function
from ufunc_eval import ufunc_eval

# import generator of numbers in interval [0, 1) and random integers
from random import random, randint

def rgb_part(ar):
    """
    O(n) solution. given an array of 'G', 'R', 'B' characters only, partitions
    the array with all Rs on the left, Gs in the middle, and Bs on the right.
    this can be done easily in O(n) because there are only 3 characters to
    paritition; we use a modified selection sort to partition.
    """
    # assume the array is valid; get length of the array
    n = len(ar)
    # index to insert next R, index to insert next B (Gs go in the middle, so we
    # do not need a separate insertion point for the Gs)
    ri = 0
    bi = n - 1
    # insert R characters in the correct place
    for i in range(n):
        # if the character is an 'R', swap elements at ri and i and advance ri
        if ar[i] == 'R':
            ar[ri], ar[i] = ar[i], ar[ri]
            ri = ri + 1
    # starting from n - 1 and ending at the last value of ri, insert Bs. it is
    # easier for us to proceed backwards.
    for i in range(n - 1, ri - 1, -1):
        # if the character is a 'B', swap elements at bi and i and decrement bi
        if ar[i] == 'B':
            ar[bi], ar[i] = ar[i], ar[bi]
            bi = bi - 1
    # return result (array will be changed in-place)
    return ar

# helper function for generating random inputs. assumes 0 <= x < 1
def _rgb_select(x):
    if x <= 1 / 3: return 'R'
    elif x <= 2 / 3: return 'G'
    return 'B'

# main
if __name__ == "__main__":
    func = rgb_part
    # given input from problem
    ar = ['G', 'B', 'R', 'R', 'B', 'R', 'G']
    ufunc_eval(func, ar)
    # arbitary input array of R, G, B characters only. equal probabilities of
    # any three of the characters showing up in the input.
    ar = [_rgb_select(random()) for _ in range(randint(5, 15))]
    ufunc_eval(func, ar)
