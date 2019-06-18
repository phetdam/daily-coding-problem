# daily coding problem #15 (medium, but i did not really know how to start.)
#
# Changelog:
#
# 06-17-2019
#
# initial creation. definitely needed some help with this one. my initial idea
# was to split the stream into segments, and then select an element to save
# from each segment. but the problem is, given N elements and say a buffer of
# size B, the maximum N that works is equal to B ^ 2 / 4 + B / 2. because if
# i have a buffer of size B and choose to read in only B / k elements at once
# so i can select one from each these partitions, which means i can save up
# to B * (1 - 1 / k) + 1 elements. this scheme can only handle N where
# N(k) = (B / k) * B * (1 - 1 / k) + 1  = B ^ 2 * (k - 1) / (k ^ 2) + (B / k).
# it is clear that N(k) reaches a mximum at N(2), i.e. k = 2. so i had to go
# and look the answer up :( and i was quite impressed by the simplicity and
# efficiency of the solution. 

__doc__ = """
This problem was asked by Facebook.

Given a stream of elements too large to store in memory, pick a random element
from the stream with uniform probability.
"""

# import evaluation function
from ufunc_eval import ufunc_eval

# needed to generate uniform randoms
import random

def stream(N):
    """
    O(n) solution with O(1) space. based off of the "reservoir" algorithm. the
    idea is that for each kth element that is encountered, one should have a
    one in k chance of picking it, and a k - 1 / k chance of picking a previous
    element. we enforce this by generating a uniform random from [0, k], and
    select only the current element if the generated random equals k.
    """
    # index of element to return, index counter (more efficient than range)
    k = i = 0
    # for each element in range
    while i < N:
        # generate random from [0, i]
        r = random.randint(0, i)
        # if r == 1, replace k with i, else do nothing
        if r == i: k = i
        # increment i
        i = i + 1
    # return k
    return k

# main
if __name__ == "__main__":
    # use a pretty big number
    BIG_N = 2 ** 20
    ufunc_eval(stream, BIG_N)
