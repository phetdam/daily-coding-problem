# daily coding problem #122 (medium)
#
# Changelog:
#
# 10-21-2019
#
# initial creation. was a little hesitant to answer at first until i realized
# that i was just overthinking and that this was a simple recursive pathfinding
# problem. after that, it was smooth sailing. kind of forgot about this problem
# and probably some other earlier ones too.

__doc__ = """
This question was asked by Zillow.

You are given a 2-d matrix where each cell represents number of coins in that
cell. Assuming we start at matrix[0][0], and can only move right or down, find
the maximum number of coins you can collect by the bottom right corner.

For example, in this matrix

0 3 1 1
2 0 0 4
1 5 3 1

The most we can collect is 0 + 2 + 1 + 5 + 3 + 1 = 12 coins.
"""

# import evaluation function
from ufunc_eval import ufunc_eval

def max_coins(mat, row = 0, col = 0):
    """
    recursive solution to the problem. since we are only moving right and down,
    we don't need to worry about retracing prior paths. we just max and return.
    optional parameters indicate where to start the search, default at 0, 0.
    """
    assert mat is not None
    # get dimensions
    nrow = len(mat)
    ncol = len(mat[0])
    # if we are out of bounds, return 0
    if (row < 0) or (row > nrow - 1) or (col < 0) or (col > ncol - 1): return 0
    # we reached the end? great
    if (row == nrow - 1) and (col == ncol - 1): return mat[row][col]
    # else return number of coins at this node plus the max of whatever we find
    # either going down a row or across a column
    return mat[row][col] + max(max_coins(mat, row = row + 1, col = col),
                               max_coins(mat, row = row, col = col + 1))

# main
if __name__ == "__main__":
    func = max_coins
    # problem input, answer is 12
    mat = [[0, 3, 1, 1],
           [2, 0, 0, 4],
           [1, 5, 3, 1]]
    ufunc_eval(func, mat)
    # another input, answer is 31
    mat = [[4, 5, 1, 0],
           [1, 0, 0, 2],
           [5, 6, 1, 7],
           [0, 4, 0, 0],
           [9, 9, 1, 1]]
    ufunc_eval(func, mat)
