# daily coding problem #65 (easy)
#
# Changelog:
#
# 08-15-2019
#
# initial creation. simple problem, but one has to be careful about how many
# times to traverse the outer layer and how to handle, for cases where the
# number of rows or columns are odd, the problem of printing out that last
# row or column.

__doc__ = """
This problem was asked by Amazon.

Given a N by M matrix of numbers, print out the matrix in a clockwise spiral.

For example, given the following matrix:

[[1,  2,  3,  4,  5],
 [6,  7,  8,  9,  10],
 [11, 12, 13, 14, 15],
 [16, 17, 18, 19, 20]]

You should print out the following: *

1, 2, 3, 4, 5, 10, 15, 20, 19, 18, 17, 16, 11, 6, 7, 8, 9, 14, 13, 12

* changed line by line output to comma-separated to save space
"""

# import evaluation function
from ufunc_eval import ufunc_eval

def cw_spiral(mat):
    """
    returns all values in the matrix in clockwise order in a list. decided to
    return a list instead of print everything to save screen space.
    """
    assert mat is not None
    # get dimensions
    nrow = len(mat)
    ncol = len(mat[0])
    # the idea is to iteratively shrink the traversal range (outer layer of the
    # matrix), which is done floor(min(nrow, ncol) / 2) times.
    nt = min(nrow, ncol) // 2
    # output list
    outl = []
    # for all traversals
    for i in range(nt):
        # append top row to outl
        for c in range(i, ncol - i): outl.append(mat[i][c])
        # append rightmost column (minus top and bottom elements) to outl
        for r in range(i + 1, nrow - i - 1): outl.append(mat[r][ncol - i - 1])
        # append bottom row to outl in reverse order
        for c in range(ncol - i - 1, i - 1, -1):
            outl.append(mat[nrow - i - 1][c])
        # append reversed leftmost column (sans top and bottom elements) to outl
        for r in range(nrow - i - 2, i, -1): outl.append(mat[r][i])
    # if there are an odd number of rows, we have to read the elements from the
    # middle row (row nt) from index nt to ncol - nt - 1
    if nrow % 2 > 0: outl = outl + mat[nt][nt:(ncol - nt)]
    # else if there are an odd number of columns, we have to read the elements
    # from the middle column (column nt) from row nt to nrow - nt - 1
    elif ncol % 2 > 0:
        for r in range(nt, nrow - nt): outl.append(mat[r][nt])
    # return outl
    return outl

def g(x):
    """
    description of solution runtime and any other comments. usually necessary
    because follow up question are often asked.
    """
    return False

# main
if __name__ == "__main__":
    func = cw_spiral
    # problem input, answer is [1, 2, 3, 4, 5, 10, 15, 20, 19, 18, 17, 16, 11,
    # 6, 7, 8, 9, 14, 13, 12]
    mat = [[1,  2,  3,  4,  5],
           [6,  7,  8,  9,  10],
           [11, 12, 13, 14, 15],
           [16, 17, 18, 19, 20]]
    ufunc_eval(func, mat)
    # another input, answer is [5, 4, 7, -1, 12, -5, 11, 98, 67, -4, 41, -1, 12,
    # -98, -2, -1, 2, 4, 23, 6, 23, 23, 11, 15]
    mat = [[5, 4, 7],
           [4, 23, -1],
           [2, 6, 12],
           [-1, 23, -5],
           [-2, 23, 11],
           [-98, 11, 98],
           [12, 15, 67],
           [-1, 41, -4]]
    ufunc_eval(func, mat)
    # another input, answer is [9, 8, 1, -8, 15, 14, 11, -2, 5]
    mat = [[9, 8, 1],
           [-2, 5, -8],
           [11, 14, 15]]
    ufunc_eval(func, mat)
