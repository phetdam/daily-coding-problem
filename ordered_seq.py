# daily coding problem #99 (medium, but relatively easy)
#
# Changelog:
#
# 09-10-2019
#
# initial creation. wasn't too sure what to do until i remembered that i can
# use a lookup table to make the indexing much faster.

__doc__ = """
This problem was asked by Microsoft.

Given an unsorted array of integers, find the length of the longest consecutive
elements sequence. For example, given [100, 4, 200, 1, 3, 2], the longest
consecutive element sequence is [1, 2, 3, 4]. Return its length: 4.

Your algorithm should run in O(n) complexity.
"""

# import evaluation function
from ufunc_eval import ufunc_eval

def max_cseq(ar):
    """
    returns the max consecutive sequence in the array. we can achieve O(n) of
    the input size by storing all the values in a dict, which allows O(1)
    checking of each element; then we iterate to construct the rest of the
    possible sequence as possible.
    """
    assert ar is not None
    # create dict from ar
    look = {k: None for k in ar}
    # length of longest consecutive sequence
    lcs = 1
    # for each element in ar
    for e in ar:
        # check if it is a starting sequence element, that e - 1 not in look
        if e - 1 not in look:
            # number of elements in this sequence
            ccs = 1
            # count all the consecutive elements
            ne = e + 1
            while ne in look:
                ccs = ccs + 1
                ne = ne + 1
            # max lcs and ccs
            lcs = max(lcs, ccs)
        # else it is not a starting element so skip it
    # return lcs
    return lcs

# main
if __name__ == "__main__":
    func = max_cseq
    # problem input, answer is 4
    ar = [100, 4, 200, 1, 3, 2]
    ufunc_eval(func, ar)
    # another input, answer is 8
    ar = [5, 4, 7, 98, 22, 14, 1, 2, 3, 6, 8, 10]
    ufunc_eval(func, ar)
