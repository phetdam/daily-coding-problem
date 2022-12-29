# daily coding problem #108 (easy)
#
# Changelog:
#
# 09-19-2019
#
# initial creation. pretty straightforward problem.

__doc__ = """
This problem was asked by Google.

Given two strings A and B, return whether or not A can be shifted some number of
times to get B. For example, if A is abcde and B is cdeab, return true. If A is
abc and B is acb, return false.
"""

# import evaluation function
from ufunc_eval import ufunc_eval

def is_shifted(a, b):
    """
    O(n). we search for the start of the string b in a and wrap around indices
    as needed. this allows us to avoid having to actually move memory.
    """
    assert (a is not None) and (b is not None) and (len(a) > 0) and (len(b) > 0)
    # length of a and b
    na, nb = len(a), len(b)
    # if the lengths are different, return False automatically
    if na != nb: return False
    # set a single length to avoid any ambiguiyu
    n = na
    # start index for the shifted string b
    si = 0
    # find the start index for b
    while si < n:
        if a[si] == b[0]: break
        si = si + 1
    # if we have no match at all, return False
    if si == n: return False
    # else we matched the starting character, so try to match all n
    for i in range(n):
        # if no match, return False
        if a[si] != b[i]: return False
        # if si is the final index, reset to 0 for wraparound
        if si == n - 1: si = 0
        # else just increment si
        else: si = si + 1
    # if successful, return True
    return True

# main
if __name__ == "__main__":
    func = is_shifted
    # problem input 1, answer is True
    a, b = "abcde", "cdeab"
    ufunc_eval(func, a, b)
    # problem input 2, answer is False
    a, b = "abc", "acb"
    ufunc_eval(func, a, b)
    # another input 1, answer is False
    a, b = "ajsjfdol", "dolajfjs"
    ufunc_eval(func, a, b)
    # another input 2, answer is True
    a, b = "sodsofief", "iefsodsof"
    ufunc_eval(func, a, b)
