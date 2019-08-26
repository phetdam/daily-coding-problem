# daily coding problem #81 (easy, as inwhat you need to do, but implementation
# has a few fine details)
#
# Changelog:
#
# 08-26-2019
#
# finally got around to looking at this problem again and finished it.
#
# 08-22-2019
#
# initial creation. worked on the problem a little bit and figured out how to
# do it, but got stuck on how exactly to print out all the permutations.

__doc__ = """
This problem was asked by Yelp.

Given a mapping of digits to letters (as in a phone number), and a digit string,
return all possible letters the number could represent. You can assume each
valid number in the mapping is a single digit. For example if {"2": ["a", "b",
"c"], "3": ["d", "e", "f"], ...} then "23" should return ["ad", "ae", "af", "bd",
"be", "bf", "cd", "ce", "cf"].
"""

# import evaluation function
from ufunc_eval import ufunc_eval

def all_maps(mp, s):
    """
    poop
    """
    assert (mp is not None) and (s is not None) and len(s) > 0
    # get length of string
    n = len(s)
    # number of possible combinations
    N = 1
    # list of how many times each possible mapping for each character needs
    # to be repeated before printing the next possible mapping
    mreps = [None for _ in range(n)]
    # for each element e in the string, multiply len(mp[e]) with N to get
    # total number of permutations to return
    for e in s: N = N * len(mp[e])
    # output list becomes N empty strings
    outl = ["" for _ in range(N)]
    # populate mreps; mreps[n - 1] = 1, mreps[n - 2] = len(mp[str(n - 1)]),
    # mreps[n - 3] = mreps[n - 2] * len(mp[str(n - 2)]), etc.
    mreps[n - 1] = 1
    for i in range(1, n):
        mreps[n - i - 1] = mreps[n - i] * len(mp[s[n - i]])
    # for each ith character in the string
    for i in range(n):
        # current mapping of the ith character to use
        j = 0
        # for all possible combinations to put in outl
        for c in range(N):
            # append mapping to cth string in outl
            outl[c] = outl[c] + mp[s[i]][j]
            # if c + 1 % mreps[i] == 0, increment j if (j + 1) < len(mp[s[i]]),
            # else reset j to 0 again
            if (c + 1) % mreps[i] == 0:
                jl = len(mp[s[i]])
                if (j + 1) < jl: j = j + 1
                else: j = 0
    # return outl
    return outl

# main
if __name__ == "__main__":
    func = all_maps
    # truncated problem input (smaller mapping), answer is ["ad", "ae", "af",
    # "bd", "be", "bf", "cd", "ce", "cf"]
    mp = {"2": ["a", "b", "c"], "3": ["d", "e", "f"]}
    s = "23"
    ufunc_eval(func, mp, s)
    # another input, answer is ["iaf", "iag", "iah", "ibf", "ibg", "ibh", "icf",
    # "icg", "ich", "jaf", "jag", "jah", "jbf", "jbg", "jbh", "jcf", "jcg", "jch"]
    mp = {"1": ["a", "b", "c"], "2": ["d", "e"], "3": ["f", "g", "h"],
          "4": ["i", "j"], "5": ["k"]}
    s = "413"
    ufunc_eval(func, mp, s)
