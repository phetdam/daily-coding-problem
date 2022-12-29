# daily coding problem #11 (medium; although it is easy to figure out that a
# trie is the data structure that they probably want you to use, implementing
# the strcmpl() function took some effort.)
#
# Changelog:
#
# 06-13-2019
#
# initial creation. i realized that a trie would be the best way to store a list
# of strings in memory in a way that a common prefix could speed up searches.
# however, i struggled with implementing the strcmpl() function until i realized
# that if each trie node returned a list of strings starting the value of the
# node, sibling nodes' lists could be combined and then each string in the
# combined list could be prefixed with the char value of the root node for each
# subtree in the trie. then add one last layer to add any remaining characters
# from the prefix to all the retrieved strings, and problem solved.

__doc__ = """
This problem was asked by Twitter.

Implement an autocomplete system. That is, given a query string s and a set of
all possible query strings, return all strings in the set that have s as a
prefix. For example, given the query string de and the set of strings [dog,
deer, deal], return [deer, deal]. Hint: Try preprocessing the dictionary into a
more efficient data structure to speed up queries.
"""

# import evaluation function
from ufunc_eval import ufunc_eval

import sys

# the trick is to use a trie to store the strings. given the common prefix s,
# we return all the substrings that are rooted by the last character in s.
class trie:
    def __init__(self, val = ""):
        """
        constructor for the trie node. by default, the list of 26 alphanumeric
        characters is all None upon initialization, and the root has val "".
        """
        self.val = val
        self.look = [None for i in range(26)]

def insert(tr, s):
    """
    insert a string s into the trie with root tr. O(len(s)).
    """
    # silently do nothing if the string is None or contains nothing; for
    # convenience, return the pointer to tr
    if (s is None) or (s == ""): return tr
    # get lowercase ASCII value - 97 of first character
    lcint = ord(s[0].lower()) - 97
    # if lcint is not between 0 and 25, print error and exit
    if (lcint < 0) or (lcint > 25):
        print("{0}: error: lowercase alphabetical characters only"
              "".format(insert.__name__), file = sys.stderr)
        quit(1)
    # if index at lcint is None, initialize new trie node at look[lcint]
    if tr.look[lcint] is None: tr.look[lcint] = trie(chr(lcint + 97))
    # call recursively for the trie node at self.look[lcint] (return None)
    return insert(tr.look[lcint], s[1:])

def search(tr, s):
    """
    search for a string s in a trie with root tr. returns True if if is in
    the trie, False if not. Again, O(len(s)). we actually don't even need
    this function but i wrote it anyways.
    """
    if s is None: return False
    # base case
    if s == "": return True
    # get lowercase ASCII value - 97 of first character
    lcint = ord(s[0].lower()) - 97
    # if lcint is not between 0 and 25, print error and exit
    if (lcint < 0) or (lcint > 25):
        print("{0}: error: lowercase alphabetical characters only"
              "".format(search.__name__), file = sys.stderr)
        quit(1)
    # if index at lcint is None, return False
    if tr.look[lcint] is None: return False
    # else call recursively on rest of string (sans first char) and return
    # the result from that call
    return search(tr.look[lcint], s[1:])

def strcmpl(tr, s = ""):
    """
    given a trie rooted at tr, return all the strings in tr that s prefixes.
    s by default is empty string; in that case, all the elements in the
    trie will be returned.
    """
    # if s is None, return empty list
    if s is None: return []
    # get memory location where we would insert words that start with s
    tr = insert(tr, s)
    # list of strings starting with tr.val to return
    sl = []
    # for all non-None entries in tr.look
    for i in range(len(tr.look)):
        # if tr.look[i] is not None, return list of strings prefixed with
        # tr.look[i].val, and combine with sl
        if tr.look[i] is not None:
            sl = sl + strcmpl(tr.look[i])
    # if sl is still empty, just return [tr.val] (we are at leaf node)
    if len(sl) == 0: return [tr.val]
    # else prepend tr.val to every element in sl
    for i in range(len(sl)): sl[i] = tr.val + sl[i]
    # if s is not the empty string, prepend all elements of s except for the
    # last letter onto all elements in sl as well
    if len(s) > 0:
        for i in range(len(sl)): sl[i] = s[:-1] + sl[i]
    return sl

# main
if __name__ == "__main__":
    # new trie
    tr = trie()
    # prefix
    pr = "de"
    # list of strings (from question example)
    sl = ["dog", "deer", "deal", "dodo", "duck", "dew", "door", "dill"]
    # insert strings into tr
    for i in range(len(sl)): insert(tr, sl[i])
    # for each element in sl, check if it exists in the trie
    for i in range(len(sl)): ufunc_eval(search, tr, sl[i])
    # return all strings in tr prefixed by pr
    ufunc_eval(strcmpl, tr, s = pr)
