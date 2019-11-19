# daily coding problem #168 (medium, but straightforward)
#
# Changelog:
#
# 11-18-2019
#
# initial creation. pretty straightforward question as well. again i have issues
# with how daily coding problem sometimes classifies its questions. here we can
# simply rotate counterclockwise in-place by firs transposing the matrix across
# the main diagonal and then swapping opposite columns.

__doc__ = """
This problem was asked by Facebook.

Given an N by N matrix, rotate it by 90 degrees clockwise.

For example, given the following matrix:

[[1, 2, 3],
 [4, 5, 6],
 [7, 8, 9]]

you should return:

[[7, 4, 1],
 [8, 5, 2],
 [9, 6, 3]]

Follow-up: What if you couldn't use any extra space?
"""

# import evaluation function
from ufunc_eval import ufunc_eval

def rotate_cw(mat, as_str = False):
    """
    also handles the followup as well, as it is in place. one can see that one
    way to quickly solve this problem, since the matrix is square, is to simply
    transpose the matrix and then swap opposite columns of the matrix. O(N^2).
    the as_str option lets you return a string that will have the list of
    lists in matrix format (so it looks nice).
    """
    assert mat is not None
    # assume that the matrix is a square matrix
    n = len(mat)
    # transpose the matrix
    for i in range(n):
        for j in range(i + 1, n):
            mat[i][j], mat[j][i] = mat[j][i], mat[i][j]
    # swap the columns of the matrix
    for i in range(n):
        for j in range(n // 2):
            mat[i][j], mat[i][n - j - 1] = mat[i][n - j - 1], mat[i][j]
    # if as_str is True, return as a nice formatted string
    if as_str == True: return str(mat).replace("], [", "],\n [")
    # else return mat (although modified in place)
    return mat

# main
if __name__ == "__main__":
    func = rotate_cw
    # problem input, answer is [[7, 4, 1],
    #                          [8, 5, 2],
    #                          [9, 6, 3]]
    mat = [[1, 2, 3],
           [4, 5, 6],
           [7, 8, 9]]
    ufunc_eval(func, mat, as_str = True)
    # another input, answer is [[-9, 3, 1, 5],
    #                          [-6, -3, 5, -4],
    #                          [0, 8, 99, 7],
    #                          [4, -5, 2, -1]]
    mat = [[5, -4, 7, -1],
           [1, 5, 99, 2],
           [3, -3, 8, -5],
           [-9, -6, 0, 4]]
    ufunc_eval(func, mat, as_str = True)
