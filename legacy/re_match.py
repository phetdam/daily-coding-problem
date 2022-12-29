# daily coding problem #25 (hard, but not really once you think it through)
#
# Changelog:
#
# 06-28-2019
#
# added an extra check for the special case where the wildcard is the last char
# in the string but we have already traversed all of the other string. done.
#
# 06-27-2019
#
# initial creation. thought i had the answer but turns out that under some more
# complicated inputs the matching fails.

__doc__ = """
This problem was asked by Facebook.

Implement regular expression matching with the following special characters:

. (period) which matches any single character
* (asterisk) which matches zero or more of the preceding element
That is, implement a function that takes in a string and a valid regular
expression and returns whether or not the string matches the regular expression.
For example, given the regular expression "ra." and the string "ray", your
function should return true. The same regular expression on the string "raymond"
should return false. Given the regular expression ".*at" and the string "chat",
your function should return true. The same regular expression on the string
"chats" should return false.
"""

# import evaluation function
from ufunc_eval import ufunc_eval

def re_match(rx, s):
    """
    pretty standard way of implementing greedy matching, O(n) of string
    """
    if (rx is None) or (s is None): return True
    # get length of both strings
    nr = len(rx)
    ns = len(s)
    # pointers
    rp = sp = 0
    # while both pointers are within bounds
    while (rp < nr) and (sp < ns):
        # match if chars are the same or if we match a dot
        if (rx[rp] == s[sp]) or (rx[rp] == "."):
            rp = rp + 1
            sp = sp + 1
        # else if the character is a star, we do greedy character matching
        elif rx[rp] == "*":
            # continue matching until we match all of s or run into a character
            # that is not preceding the star. however, if we encounter a char
            # that is equal to the char after the star, stop matching
            while (sp < ns) and (rx[rp - 1] == s[sp] or rx[rp - 1] == "."):
                if (rp + 1 < nr) and rx[rp + 1] == s[sp]: break
                sp = sp + 1
            # now increment rp to move away from the star
            rp = rp + 1
        # else we have no match, so break the loop
        else: break
    # handles the special case where the regex is longer than the string and
    # contains the wildcard character as the last character
    if (rp == nr - 1) and (rx[rp] == "*"): rp = rp + 1
    # if both pointers have not traversed all of the regex and string, then
    # we have failed to match everything
    return (rp == nr) and (sp == ns)

# main
if __name__ == "__main__":
    func = re_match
    # given problem inputs
    rx = "ra."
    s = "ray"
    ufunc_eval(func, rx, s)
    s = "raymond"
    ufunc_eval(func, rx, s)
    rx = ".*at"
    s = "chat"
    ufunc_eval(func, rx, s)
    s = "chats"
    ufunc_eval(func, rx, s)
    rx = "pig.*nh.*"
    s = "pigeonhole"
    ufunc_eval(func, rx, s)
    rx = "adeis.*"
    s = "adeis"
    ufunc_eval(func, rx, s)
