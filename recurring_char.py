# daily coding problem #159 (easy)
#
# Changelog:
#
# 11-12-2019
#
# initial creation. pretty straightforward use of a hash table.

__doc__ = """
This problem was asked by Google.

Given a string, return the first recurring character in it, or null if there is
no recurring character.

For example, given the string "acbbac", return "b". Given the string "abcdef",
return null.
"""

# import evaluation function
from ufunc_eval import ufunc_eval

def first_recurring(s):
    """
    just save seen characters in a dictionary and return the first hit. if all
    the characters are unique, None will be returned.
    """
    assert (s is not None) and (len(s) > 0)
    # lookup table
    look = {}
    # for each element of s
    for e in s:
        # if e is in s, return e
        if e in look: return e
        # else make new dict entry
        else: look[e] = 0
    # if all characters are unique, return None
    return None

# main
if __name__ == "__main__":
    func = first_recurring
    # problem input, answer is "b"
    s = "acbbac"
    ufunc_eval(func, s)
    # problem input, answer is None
    s = "abcdef"
    ufunc_eval(func, s)
    # another input, answer is "g"
    s = "osg0awbgiwpjgaisdsajsaidhfos"
    ufunc_eval(func, s)
