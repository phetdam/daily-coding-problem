# daily coding problem #195 (hard, but not really unless i missed something)
#
# Changelog:
#
# 12-22-2019
#
# initial creation. i don't think this a hard problem; the naive way is O(mn)
# for an m x n matrix, but the method i came up with is position dependent in
# its runtime, although it does exploit the property of the matrix being sorted
# by both rows and columns to reduce total checks.

__doc__ = """
This problem was asked by Google.

Let A be an N by M matrix in which every row and every column is sorted.

Given i1, j1, i2, and j2, compute the number of elements of M smaller than
M[i1, j1] and larger than M[i2, j2].

For example, given the following matrix:

[[1, 3, 7, 10, 15, 20],
 [2, 6, 9, 14, 22, 25],
 [3, 8, 10, 15, 25, 30],
 [10, 11, 12, 23, 30, 35],
 [20, 25, 30, 35, 40, 45]]

And i1 = 1, j1 = 1, i2 = 3, j2 = 3, return 15 as there are 15 numbers in the
matrix smaller than 6 or greater than 23.
"""

# import evaluation function
from ufunc_eval import ufunc_eval

def larger_smaller(mat, ra, ca, rb, cb):
    """
    for m x n matrix mat, the naive solution is O(mn). we take advantage of the
    fact that the matrix is sorted by both row and column, so for any element
    located at (r, c), we know that any elements (s, t) where s >= r, t >= c are
    larger than (r, c) and that any elements where s <= r, t <= c are smaller.
    so if for some (r, c) we want to find elements in mat that are greater, we
    will know which elements are for sure smaller than (r, c), i.e. any (s, t)
    where s <= r and r <= c, and we will have to check the remaining elements
    to see which ones are greater than (r, c).
    """
    assert (mat is not None) and (len(mat) > 0) and (len(mat[0]) > 0)
    assert (ra > -1) and (ca > -1) and (rb > -1) and (cb > -1)
    # get dimensions
    nrow, ncol = len(mat), len(mat[0])
    assert (ra < nrow) and (ca < ncol) and (rb < nrow) and (cb < ncol)
    # total number of target elements
    tot = 0
    # find all elements smaller than mat[ra][ca]. we know all elements mat[i][j]
    # where i >= ra and j >= ca are greater than mat[ra][ca]. for rows < ra,
    # check all the elements, while for any rows >= ra, check up to col ca - 1.
    # since the matrix is sorted both by rows and columns, if we encounter
    # mat[i][j] >= mat[ra][ca], we can break the inner loop immediately, as we
    # are going in ascending element order for each row.
    for i in range(ra):
        for j in range(ncol):
            if mat[i][j] < mat[ra][ca]: tot = tot + 1
            else: break
    for i in range(ra, nrow):
        for j in range(ca):
            if mat[i][j] < mat[ra][ca]: tot = tot + 1
            else: break
    # find all elements greater than mat[rb][cb]. we know all elements mat[i][j]
    # where i <= ra and j <= ca are less than mat[ra][ca]. for rows <= ra, check
    # columns starting from ca + 1, while for rows > ra, check all elements.
    # since the matrix is sorted both by rows and columns, if we encounter
    # mat[i][j] <= mat[ra][ca], we can break the inner loop immediately, as we
    # are going in descending element order for each row.
    for i in range(rb + 1):
        for j in range(ncol - 1, cb, -1):
            if mat[i][j] > mat[rb][cb]: tot = tot + 1
            else: break
    for i in range(rb + 1, nrow):
        for j in range(ncol - 1, -1, -1):
            if mat[i][j] > mat[rb][cb]: tot = tot + 1
            else: break
    # return tot
    return tot

# main
if __name__ == "__main__":
    func = larger_smaller
    # problem input, answer is 14 ("actual" answer is 15, which may be a typo)
    mat = [[1, 3, 7, 10, 15, 20],
           [2, 6, 9, 14, 22, 25],
           [3, 8, 10, 15, 25, 30],
           [10, 11, 12, 23, 30, 35],
           [20, 25, 30, 35, 40, 45]]
    ra, ca, rb, cb = 1, 1, 3, 3
    args = (mat, ra, ca, rb, cb)
    ufunc_eval(func, *args)
    # another input, answer is 20
    mat = [[1, 2, 4, 5, 6, 9],
           [2, 3, 5, 8, 18, 19],
           [4, 8, 10, 12, 14, 18],
           [7, 11, 14, 15, 17, 20],
           [9, 12, 16, 19, 20, 29]]
    ra, ca, rb, cb = 2, 2, 3, 4
    args = (mat, ra, ca, rb, cb)
    ufunc_eval(func, *args)
