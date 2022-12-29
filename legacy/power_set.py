# daily coding problem #37 (easy, but easy to mess up too. i think there is also
# most likely a better solution to this problem. or at least a nicer one.)
#
# Changelog:
#
# 07-09-2019
#
# initial creation. first tried a recursive approach, then an iterative one, but
# realized the problem of creating duplicate sets, i.e. permutations of the same
# sublists. the solution is when creating a larger sublist from a smaller one by
# adding a new element, to only add the elements that the original sublist does
# not contain that are also greater than the supremum of the original sublist.

__doc__ = """
This problem was asked by Google.

The power set of a set is the set of all its subsets. Write a function that,
given a set, generates its power set. For example, given the set {1, 2, 3}, it
should return {{}, {1}, {2}, {3}, {1, 2}, {1, 3}, {2, 3}, {1, 2, 3}}. You may
also use a list or array to represent a set.
"""

# import evaluation function
from ufunc_eval import ufunc_eval
# for random integers
from random import randint

def power_set(S):
    """
    generates power set iteratively from a list. prevents adding duplicate
    sublists by enforcing that inclusion of new elements is restricted to those
    that are both not in the list and greater than the maximum of the list.
    """
    # get length of S
    n = len(S)
    # current power set
    pset = [[]]
    # working queue of sets to expand
    queue = [[]]
    # while the queue is not empty
    while len(queue) > 0:
        # pop a set
        u = queue.pop(0)
        # the new set must have cardinality less than S itself
        if len(u) < n - 1:
            # for each element in S
            for e in S:
                # if the element is not in the set, insert only if u is empty
                # set (one case) or if e is greater than the maximum of u.
                # this prevents duplicates from being formed.
                if (e not in u) and ((len(u) == 0) or (e > max(u))):
                    v = u + [e]
                    # append to queue and pset
                    queue.append(v)
                    pset.append(v)
    # append S itself to pset
    pset.append(S)
    # return
    return pset

# main
if __name__ == "__main__":
    func = power_set
    # given example input
    S = [1, 2, 3]
    ufunc_eval(func, S)
    # size of new list
    k = 4
    # range parameter
    a = 9
    # new set
    S = set()
    while len(S) < k:
        S.add(randint(-a * a, a * a))
    S = sorted(list(S))
    ufunc_eval(func, S)
