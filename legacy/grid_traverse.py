# daily coding problem #62 (medium, but straightforward)
#
# Changelog:
#
# 08-04-2019
#
# initial creation. definitely haven't done any of these for a while; been busy.
# i kind of remembered how to do a problem like this by just thinking it through
# and also optimized the dynamic programming approach by saving on memory costs,
# reducing an O(mn) usage to O(2n). straightforward problem.

__doc__ = """
This problem was asked by Facebook.

There is an N by M matrix of zeroes. Given N and M, write a function to count
the number of ways of starting at the top-left corner and getting to the bottom-
right corner. You can only move right or down. For example, given a 2 by 2
matrix, you should return 2, since there are two ways to get to the bottom-right:

Right, then down
Down, then right

Given a 5 by 5 matrix, there are 70 ways to get to the bottom-right.
"""

# import evaluation function
from ufunc_eval import ufunc_eval

def num_paths(rows, cols):
    """
    O(nm) solution with O(2m) space. the idea is that we can "fill" each cell in
    the grid with the number of ways to get to it from the top left corner if we
    can only go down or right. so the first row and column will always be 1, and
    any other cell v[r, c] = v[r - 1, c] + v[r, c - 1]. so we use dp to solve.
    """
    assert rows > 0 and cols > 0
    # if there is only one row, then there is only one path to the bottom right
    # hand corner (end) of the grid (array).
    if rows == 1: return 1
    # else set up two arrays, one for prev row and one for next row. we could
    # set up an entire matrix, but this is more memory efficient.
    prev = [1 for _ in range(cols)]
    cur = [0 for _ in range(cols)]
    # need to set cur[0] to 1; remember first column is all 1 as well
    cur[0] = 1
    # for each row starting from the second row, starting from col index 1
    for _ in range(1, rows):
        for c in range(1, cols):
            cur[c] = cur[c - 1] + prev[c]
        # swap prev and cur to save on memory realloc costs
        old_prev = prev
        prev = cur
        cur = old_prev
    # return last element in prev
    return prev[cols - 1]

# main
if __name__ == "__main__":
    func = num_paths
    # problem input
    rows, cols = 5, 5
    ufunc_eval(func, rows, cols)
    # simple input, answer is 3
    rows, cols = 2, 3
    ufunc_eval(func, rows, cols)
