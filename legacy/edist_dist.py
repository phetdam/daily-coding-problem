# daily coding problem #31 (easy, but not if you do not know what levenshtein
# distance is, in which case you will need to spend time and think)
#
# Changelog:
#
# 07-05-2019
#
# initial creation. the easiest way to do this problem is to simply know what
# levenshtein distance is, as that is what the question is asking. there is a
# simple recurrence relation for the levenshtein distance that can be readily
# made more efficient using dynamic programming. i show recursive and top-down
# dynamic programming solutions.

__doc__ = """
This problem was asked by Google.

The edit distance between two strings refers to the minimum number of character
insertions, deletions, and substitutions required to change one string to the
other. For example, the edit distance between "kitten" and "sitting" is three:
substitute the "k" for "s", substitute the "e" for "i", and append a "g". Given
two strings, compute the edit distance between them.
"""

# import evaluation function
from ufunc_eval import ufunc_eval
# infinity from math
from math import inf
# geenerate random integers
from random import randint

def edist(a, b):
    """
    O(3^n) naive solution to computing the edit distance which in this case is
    the levenshtein distance for two strings a and b.
    """
    # assuming both strings are valid
    na = len(a)
    nb = len(b)
    # if one of the strings is 0, simply return longer string's length
    if min(na, nb) == 0: return max(na, nb)
    # else return min of these three function calls, abusing integer values of
    # booleans to act as indicator function
    return min(edist(a[:-1], b) + 1, edist(a, b[:-1]) + 1,
               edist(a[:-1], b[:-1]) + int(a[-1] != b[-1]))

def edist_dp(a, b, look = None):
    """
    O(mn) dynamic programming solution to providing edit distance, where m and
    n are the lengths of the respective strings a and b. uses O(mn) space for
    the lookup table, with optional arguments to get rid of need for wrapper.
    """
    # if called as wrapper, some or all of these may be None. assume strings
    # are valid, non-None stringss (can be empty).
    na = len(a)
    nb = len(b)
    if look is None:
        # initialize lookup table; any entries in row 0 or column 0 are
        # initialize to max(row, col). we need the lookup to be na + 1 by
        # nb + 1 for convenience.
        look = [[inf for _ in range(nb + 1)] for _ in range(na + 1)]
    # if the corresponding levenshtein distance is not inf, return value in look
    if look[na][nb] != inf: return look[na][nb]
    # else we need to compute. if one of the strings is empty, save into the
    # lookup table the length of the nonempty string.
    if min(na, nb) == 0: look[na][nb] = max(na, nb)
    # else we will have to compute recursively
    else: look[na][nb] = min(edist_dp(a[:-1], b, look = look) + 1,
                             edist_dp(a, b[:-1], look = look) + 1,
                             edist_dp(a[:-1], b[:-1], look = look) +
                             int(a[-1] != b[-1]))
    # return value in lookup table
    return look[na][nb]

# main
if __name__ == "__main__":
    # given inputs from problem, answer is 3
    a = "kitten"
    b = "sitting"
    ufunc_eval(edist, a, b)
    ufunc_eval(edist_dp, a, b)
    # use dynamic programming version on multiple random inputs
    pairs = [("".join([chr(randint(ord("a"), ord("z")))
                       for _ in range(randint(5, 10))]),
              "".join([chr(randint(ord("a"), ord("z")))
                       for _ in range(randint(5, 10))]),
              ) for _ in range(10)]
    for p in pairs: ufunc_eval(edist, p[0], p[1])
    for p in pairs: ufunc_eval(edist_dp, p[0], p[1])
