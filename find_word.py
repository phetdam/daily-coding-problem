# daily coding problem #63 (easy)
#
# Changelog:
#
# 08-09-2019
#
# initial creation. really behind on these problems; kind of just cherry picking
# whichever ones i feel like doing. relatively simple problem.

__doc__ = """
This problem was asked by Microsoft.

Given a 2D matrix of characters and a target word, write a function that returns
whether the word can be found in the matrix by going left-to-right, or
up-to-down. For example, given the following matrix:

[['F', 'A', 'C', 'I'],
 ['O', 'B', 'Q', 'P'],
 ['A', 'N', 'O', 'B'],
 ['M', 'A', 'S', 'S']]

and the target word 'FOAM', you should return true, since it's the leftmost
column. Similarly, given the target word 'MASS', you should return true, since
it's the last row.
"""

# import evaluation function
from ufunc_eval import ufunc_eval

def find_word(mat, wd):
    """
    O(mn) solution, where m and n are the number of rows/cols in the matrix.
    we find all the starting occurrences of wd and then do a best-only search
    for each of the occurrences.
    """
    # sanity checks
    assert (mat is not None) and (wd is not None) and wd
    # get dimensions of the matrix
    nrow = len(mat)
    ncol = len(mat[0])
    # get length of string
    wd_len = len(wd)
    # list of starting indices
    inits = []
    # find all indices that contain wd[0]
    for i in range(nrow):
        for j in range(ncol):
            if mat[i][j] == wd[0]: inits.append((i, j))
    # for each pair in inits
    for init in inits:
        # number of matched characters (we already matched the first one)
        mcs = 1
        # search left to right
        # number of columns from current col to last >= wd_len is required
        if ncol - init[1] + 1 >= wd_len:
            for i in range(1, wd_len):
                # if there is no match, break
                if mat[init[0]][init[1] + i] != wd[i]: break
                # else increment no. matched characters
                else: mcs = mcs + 1
            # if fully matched, return True
            if mcs == wd_len: return True
            # reset mcs
            mcs = 1
        # else proceed to search top to bottom
        # number of rows from current row to last >= wd_len is required
        if nrow - init[0] + 1 >= wd_len:
            for i in range(1, wd_len):
                if mat[init[0] + i][init[1]] != wd[i]: break
                else: mcs = mcs + 1
            if mcs == wd_len: return True
            mcs = 1
    # if we still haven't returned True, we have failed
    return False

# main
if __name__ == "__main__":
    func = find_word
    # problem inputs
    mat = [['F', 'A', 'C', 'I'],
           ['O', 'B', 'Q', 'P'],
           ['A', 'N', 'O', 'B'],
           ['M', 'A', 'S', 'S']]
    wa = "FOAM"
    wb = "MASS"
    ufunc_eval(func, mat, wa)
    ufunc_eval(func, mat, wb)
    # more inputs
    mat = [['a', 'h', 'f', 'c', 't'],
           ['c', 's', 'r', 'h', 'a'],
           ['a', 'r', 'a', 'e', 'h'],
           ['d', 'a', 'y', 'e', 's'],
           ['g', 'y', 'b', 's', 'n'],
           ['z', 'a', 'w', 'e', 'e']]
    wa = "cheese"
    wb = "ara"
    wc = "fray"
    wd = "academy"
    ufunc_eval(func, mat, wa)
    ufunc_eval(func, mat, wb)
    ufunc_eval(func, mat, wc)
    ufunc_eval(func, mat, wd)
