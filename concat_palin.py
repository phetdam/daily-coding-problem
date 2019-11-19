# daily coding problem #167 (hard, but definitely not hard)
#
# Changelog:
#
# 11-18-2019
#
# initial creation. a very straightforward question, so not sure why it deserves
# to be classified as a hard question. daily coding problem definitely has some
# issues with how it classifies the difficulties of questions.

__doc__ = """
This problem was asked by Airbnb.

Given a list of words, find all pairs of unique indices such that the
concatenation of the two words is a palindrome. For example, given the list
["code", "edoc", "da", "d"], return [(0, 1), (1, 0), (2, 3)].
"""

# import evaluation function
from ufunc_eval import ufunc_eval

def palin_pairs(ar):
    """
    given a list of words ar, finds all pairs of unique indices such that the
    concatenation of the two words is a palindrome. O(len(ar)^2); we check all
    pairs (i, j), (j, i) of indices, i < j, as there are times where we see
    ar[i] + ar[j] is a palindrome, but the converse is not true.
    """
    assert (ar is not None)
    n = len(ar)
    # if length is not 2 or greater, return empty list (no pairs)
    if n < 2: return []
    # else check all (i, j), (j, i) pairs, where i < j
    pairs = []
    for i in range(n):
        for j in range(i + 1, n):
            # check if ar[i] + ar[j] is a palindrome
            if is_palin(ar[i] + ar[j]) == True: pairs.append((i, j))
            # check if ar[j] + ar[i] is a palindrome
            if is_palin(ar[j] + ar[i]) == True: pairs.append((j, i))
    # return pairs
    return pairs

def is_palin(s):
    """
    checks whether or not the given string s is a palindrome or not. note that
    the empty string is considered a palindrome as well. O(len(s)).
    """
    assert s is not None
    # if empty string or just one character, definitely a palindrome.
    if len(s) < 2: return True
    # else starting from both ends, check that opposing characters are the same
    n = len(s)
    for i in range(n // 2):
        # if opposing characters differ, not a palindrome so return False
        if s[i] != s[n - i - 1]: return False
    # else return True
    return True

# main
if __name__ == "__main__":
    func = palin_pairs
    # problem input, answer is [(0, 1), (1, 0), (2, 3)]
    ar = ["code", "edoc", "da", "d"]
    ufunc_eval(func, ar)
    # another input, answer is [(0, 5), (2, 3), (3, 2), (4, 6)]
    ar = ["asdf", "fosj", "ffdf", "fdff", "sdw", "dsa", "ds"]
    ufunc_eval(func, ar)
