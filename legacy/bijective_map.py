# daily coding problem #176 (easy)
#
# Changelog:
#
# 11-25-2019
#
# initial creation. pretty straightforward problem, if i am correct in my
# belief that the mapping from s1 to s2 must also be onto.

__doc__ = """
This problem was asked by Bloomberg.

Determine whether there exists a one-to-one character mapping from one string s1
to another s2. For example, given s1 = abc and s2 = bcd, return true since we
can map a to b, b to c, and c to d. Given s1 = foo and s2 = bar, return false
since the o cannot map to two characters.
"""

# import evaluation function
from ufunc_eval import ufunc_eval

def has_bijective_map(a, b):
    """
    O(min{len(a), len(b)}). if the lengths aren't the same, we have no bijection
    as the map needs to be onto as well as injective, so return False. else if
    the lengths are the same, we iteratively build a dictionary of character
    mappings; adding a mapping for any x -> y if it does not exist, and if it
    does exist, using the mapping if we have a pair (x, y), or returning False
    if we have a pair (x, z) where our map is x -> y (not injective).
    """
    assert (a is not None) and (b is not None) and (len(a) > 0) and (len(b) > 0)
    # get lengths of a and b
    na, nb = len(a), len(b)
    # if they don't match, not onto so return False
    if na != nb: return False
    # else start biulding the dictionary of mappings
    mdict = {}
    for i in range(na):
        # if there is a mapping for a[i] in mdict
        if a[i] in mdict:
            # if the mapping matches a[i] -> b[i], then pass
            if mdict[a[i]] == b[i]: pass
            # else return False; no bijection
            else: return False
        # else no mapping for character a[i], so add map a[i] -> b[i] to mdict
        else: mdict[a[i]] = b[i]
    # return True if we made it all the way through the string
    return True

# main
if __name__ == "__main__":
    func = has_bijective_map
    # problem input, answer is True
    a, b = "abc", "bcd"
    ufunc_eval(func, a, b)
    # another problem input, answer is False
    a, b = "foo", "bar"
    ufunc_eval(func, a, b)
    # another input, answer is False (no onto mapping)
    a, b = "foo", "baabaablacksheep"
    ufunc_eval(func, a, b)
