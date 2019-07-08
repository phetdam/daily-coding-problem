# daily coding problem #34 (medium, actually quite difficult unless i am doing
# something completely wrong)
#
# Changelog:
#
# 07-07-2019
#
# initial creation. it says that it is a "medium" question but it actually is
# not so easy. the approach i took was to first find the longest palindrome in
# the input string that was also lexicographically latest; if the palindrome
# includes either the first or last characters of the string, then we can simply
# append the remaining characters to the other side of the string that the
# palindrome is not on. else, we have to take all the characters except for the
# single character on the left/right end that is earlier lexicographically (but
# we actually have to first check the string against its reversed self if there
# are duplicate characters) and append/prepend all onto the other side in the
# proper order. the most difficult part is writing the function that returns the
# longest palindrome in the string. it could be further optimized with dp.

__doc__ = """
This problem was asked by Quora.

Given a string, find the palindrome that can be made by inserting the fewest
number of characters as possible anywhere in the word. If there is more than one
palindrome of minimum length that can be made, return the lexicographically
earliest one (the first one alphabetically). For example, given the string
"race", you should return "ecarace", since we can add three letters to it (which
is the smallest amount to make a palindrome). There are seven other palindromes
that can be made from "race" by adding three letters, but "ecarace" comes first
alphabetically. As another example, given the string "google", you should return
"elgoogle".
"""

# import evaluation function
from ufunc_eval import ufunc_eval
from math import inf

def make_palin(s):
    """
    in order to make the shortest possible, lexicographically earliest possible
    palindrome, we first need to check if the current string itself is also a
    palindrome. the function get_subpalin() returns the starting and ending
    indices of the "sub-palindrome" of the string s that is the longest/latest
    lexicographically. we only care if the tuple returns is of the form (0, x)
    or (x, n - 1), where n is the length of the string, as that means that we
    can simply append the non-sub-palindrome characters to make a palindrome.
    any other combination of indices that are not (0, n - 1), where s itself is
    a palindrome, means that we need to add all extra characters (n - 1
    characters) from the side of s that is lexicographically lower in reverse
    order to the other side of s.
    """
    # assume that s is a valid string and get the starting and ending indices
    # for its longest sub-palindrome, using the lexicographically largest
    # sub-palindrome that can be found. for example, with a string "aaaassss",
    # where "aaaa" or "ssss" can be picked, "ssss" will be picked so that we
    # can make the palindrome "aaaassssaaaa", which is lexicographically the
    # earliest one that we can make.
    n = len(s)
    spl, spr = sub_palin(s)
    # string to add to s to make it the shortest, lexicographically earliest
    # possible palindrome
    ps = ""
    # useful only if spl == 0 or spr == n - 1
    if spl == 0:
        # prepend all characters from spr + 1 onwards to the front of s + return
        for i in range(spr + 1, n): ps = s[i] + ps
        # return ps + s
        return ps + s
    elif spr == n - 1:
        # append all characters from 0 to spl - 1 to the rear of s + return
        for i in range(spl): ps = s[i] + ps
        return s + ps
    # else
    # if the left end is lower lexicographically (forward vs reverse string)
    if s < s[::-1]:
        for i in range(n - 1): ps = s[i] + ps
        return s + ps
    # else the right end is lower lexicographically
    for i in range(n - 1, 0, -1): ps = ps + s[i]
    return ps + s

def sub_palin(s, l = 0, r = None):
    """
    returns the starting and ending indices of a "sub-palindrome" of string s,
    by default trying to get the longest possible one that is also greatest in
    lexicographical terms as well. can get a speed boost if we use dp, since
    currently this is an exponential time algorithm.
    """
    # sanity check
    if (s is None) or (s == ""): return (inf, inf)
    # we don't need wrapper if we use optional arguments
    if r is None: r = len(s) - 1
    # base case (invalid indexing)
    if l > r: return (inf, inf)
    # base case (length 1 and length 2 palindrome)
    if (l == r) or ((l == r - 1) and (s[l] == s[r])): return (l, r)
    # else the section we are looking at is wider than one character. so we
    # shorten the window by one character at each end individually and together,
    # and only return the indices of the longer palindrome. if spl_c == l + 1
    # and spr_c == r - 1, then decrement spl_c and increment spr_c since the
    # sub-palindrome is contiguous with l and r.
    spl_a, spr_a = sub_palin(s, l = l, r = r - 1)
    spl_b, spr_b = sub_palin(s, l = l + 1, r = r)
    spl_c, spr_c = sub_palin(s, l = l + 1, r = r - 1)
    if (s[l] == s[r]) and (spl_c == l + 1) and (spr_c == r - 1):
        spl_c = spl_c - 1
        spr_c = spr_c + 1
    # palindrome lengths
    spa_n = spb_n = spc_n = 0
    # for any non tuple that is not (inf, inf), get its length
    if (spl_a, spr_a) != (inf, inf): spa_n = spr_a - spl_a + 1
    if (spl_b, spr_b) != (inf, inf): spb_n = spr_b - spl_b + 1
    if (spl_c, spr_c) != (inf, inf): spc_n = spr_c - spl_c + 1
    # get max of the lengths
    max_spn = max(spa_n, spb_n, spc_n)
    # left and right to return, initially spl_a and spr_a
    spl, spr = spl_a, spr_a
    # return tuple that has longer length. if there is a tie, then use the sp
    # is lexicographically behind (since we have to make a palindrome later that
    # needs to be the earliest lexicographically).
    if spb_n == max_spn:
        # if b combination has longer length than a, update spl, spr; else if
        # the lengths are the same, update only if b is after a in a
        # lexicographical sense
        if (spb_n > spa_n) or ((spb_n == spa_n) and (s[spl_b:(spr_b + 1)] >
                                                     s[spl_a:(spr_a + 1)])):
            spl, spr = spl_b, spr_b
        # do same if c combination also has length max_spn
    elif spc_n == max_spn:
        if (spc_n > spa_n) or ((spc_n == spa_n) and (s[spl_c:(spr_c + 1)] >
                                                     s[spl_a:(spr_a + 1)])):
            spl, spr = spl_c, spr_c
    # return chosen indices
    return spl, spr


# main
if __name__ == "__main__":
    func = make_palin
    # given problem input 1, answer is "ecarace"
    s = "race"
    ufunc_eval(make_palin, s)
    # given problem input 2, answer is "elgoogle"
    s = "google"
    ufunc_eval(make_palin, s)
    # more difficult input, answer is "ggeestrrtseegg"
    s = "eestrrtseegg"
    ufunc_eval(make_palin, s)
