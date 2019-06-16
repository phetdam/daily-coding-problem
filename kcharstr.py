# daily coding problem #13 (hard, did require some thought.)
#
# Changelog:
#
# 06-15-2019
#
# initial creation. i used a "sliding window" approach to determine the string
# length and used a lookup table to store character counts. this is as close to
# the most efficient solution that i can get to.

__doc__ = """
This problem was asked by Amazon.

Given an integer k and a string s, find the length of the longest substring that
contains at most k distinct characters. For example, given s = "abcba" and k = 2,
the longest substring with k distinct characters is "bcb".
"""

# import evaluation function
from ufunc_eval import ufunc_eval

def kcharstr(s, k):
    """
    given string s, return the max length of the string that has at most k
    distinct characters. the idea is that given two indices, si (starting) and
    ei (ending), we first advance ei, maintaining the number of characters seen
    in a table (indexed by character) and the number of distinct characters seen
    (dcn). if dcn > k, we start to advance si, for each character decrementing
    its associated value in the lookup table. if one of the cells becomes 0,
    then we decrement k. we stop when k == dcn, and then continue advancing ei.
    whenever we have dcn == k, we take the max of the ei - si + 1 and the
    previously stored max length value (maxl). if we go through the whole string
    and maxl is still 0, we set maxl to ei - si. roughly O(n).
    """
    # alphabetical lookup table
    ca = [0 for _ in range(26)]
    # number of distinct characters encountered
    dcn = 0
    # max length of string with k or less distinct characters
    maxl = 0
    # starting and ending indices
    si = ei = 0
    # while ei < len(s)
    while ei < len(s):
        # if dcn <= k, advance ei, and increment the char's corresponding count
        if dcn <= k:
            # get char's index in ca
            ci = ord(s[ei]) - 97
            # if the char's corresponding count is 0, increment dcn
            if ca[ci] == 0: dcn = dcn + 1
            # if dcn == k, save to maxl if greater than current value of maxl
            if dcn == k: maxl = max(maxl, ei - si + 1)
            # increment character count and ei
            ca[ci] = ca[ci] + 1
            ei = ei + 1
        # else dcn > k, so start advancing si instead.
        else:
            # get char's index in ca
            ci = ord(s[si]) - 97
            # decrease char count in ca
            ca[ci] = ca[ci] - 1
            # if ca[ci] == 0, then decrement dcn
            if ca[ci] == 0: dcn = dcn - 1
            # increment si
            si = si + 1
    # if dcn <= k, take max of maxl and ei - si
    if dcn <= k: maxl = max(maxl, ei - si)
    return maxl

def g(x):
    """
    description of solution runtime and any other comments. usually necessary
    because follow up question are often asked.
    """
    return False

# main
if __name__ == "__main__":
    s = "abcbaffffvbfffff"
    k = 3
    ufunc_eval(kcharstr, s, k)
