# daily coding problem #113 (medium)
#
# Changelog:
#
# 09-25-2019
#
# initial creation. first a little stuck on the follow up until i realized that
# by flipping the string across the midpoint, we had reversed the order of the
# words and the words themselves, and that all we had to do was reverse the
# characters for each word.

__doc__ = """
This problem was asked by Google.

Given a string of words delimited by spaces, reverse the words in string. For
example, given "hello world here", return "here world hello"

Follow-up: given a mutable string representation, can you perform this operation
in-place?
"""

# import evaluation function
from ufunc_eval import ufunc_eval

def word_reverse(s):
    """
    reverses by splitting s into a list of strings and then swapping indices
    before re-joining back into a string.
    """
    assert s is not None
    # split into words
    wl = s.split()
    # get number of words in the list
    n = len(wl)
    # swap elements symmetrically across the midpoint
    for i in range(n // 2):
        wl[i], wl[n - i - 1] = wl[n - i - 1], wl[i]
    # rejoin and return
    return " ".join(wl)

def word_reverse_inplace(s):
    """
    "in-place" word reversal, where we use a list of single-character strings to
    simlulate a mutable string. reflects the characters of the string across the
    midpoint of the string, and for each space-delimited group, reverses the
    characters in that group to un-backwards the word.
    """
    assert s is not None
    # convert to list of characters
    wl = [e for e in s]
    # get length of wl
    n = len(wl)
    # swap elements symmetrically across the midpoint
    for i in range(n // 2):
        wl[i], wl[n - i - 1] = wl[n - i - 1], wl[i]
    # starting and ending indices of each chunk of characters to reverse
    si = ei = 0
    # for each chunk of non-whitespace characters (assumes no leading spaces)
    for i in range(n + 1):
        # stop at whitespace or if we are past last character of the string
        if (i == n) or (wl[i] == " "):
            # reverse indices in the substring, indices si to i - 1
            for j in range(si, (si + i) // 2):
                wl[j], wl[si + i - j - 1] = wl[si + i - j - 1], wl[j]
            # set si to i + 1
            si = i + 1
        # else do nothing
    # join wl and return
    return "".join(wl)

# main
if __name__ == "__main__":
    func_a = word_reverse
    func_b = word_reverse_inplace
    # problem input, answer is "here world hello"
    s = "hello world here"
    ufunc_eval(func_a, s)
    ufunc_eval(func_b, s)
    # another input, answer is "now doing you are what"
    s = "what are you doing now"
    ufunc_eval(func_a, s)
    ufunc_eval(func_b, s)
