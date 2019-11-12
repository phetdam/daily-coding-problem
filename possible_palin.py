# daily coding problem #157 (easy)
#
# Changelog:
#
# 11-12-2019
#
# initial creation. was a little concerned on how to solve it until i realized
# that all palindromes only have one character that has an odd frequency.
# finally back to some practice after a brutal academic week.

__doc__ = """
This problem was asked by Amazon.

Given a string, determine whether any permutation of it is a palindrome.

For example, carrace should return true, since it can be rearranged to form
racecar, which is a palindrome. daily should return false, since there's no
rearrangement that can form a palindrome.
"""

# import evaluation function
from ufunc_eval import ufunc_eval

def possible_palin(s):
    """
    the trick is that for a string to be a palindrome, a necessary condition is
    that all its characters except one must appear an even number of times. thus
    we will use some extra memory to count all the characters, and if we find
    that more than one character occurs an odd number of times, we return False,
    else we can return True as the string can be a palindrome.
    """
    assert (s is not None) and (len(s) > 0)
    # length of string
    n = len(s)
    # count characters with lookup dict
    look = {}
    for c in s:
        # increment character count if we already keyed in
        if c in look: look[c] = look[c] + 1
        # else create new entry
        else: look[c] = 1
    # count number of times we have odd character frequencies
    nof = 0
    for k, v in look.items():
        # if odd, increase count of of number of characters with odd frequencies
        if v % 2 > 0: nof = nof + 1
        # if nof > 1, return False
        if nof > 1: return False
    # else return True
    return True

# main
if __name__ == "__main__":
    func = possible_palin
    # problem input, answer is True
    s = "carrace"
    ufunc_eval(func, s)
    # problem input, answer is False
    s = "daily"
    ufunc_eval(func, s)
    # another input, answer is True
    s = "bbabaabdeaaaedf"
    ufunc_eval(func, s)
