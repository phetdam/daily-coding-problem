# daily coding problem #76 (medium, but it is pretty easy)
#
# Changelog:
#
# 08-18-2019
#
# initial creation. problem statement makes the problem seem harder than it
# actually is, and maybe confuses rows with columns.

__doc__ = """
This problem was asked by Google.

You are given an N by M 2D matrix of lowercase letters. Determine the minimum
number of columns that can be removed to ensure that each row is ordered from
top to bottom lexicographically. That is, the letter at each column is
lexicographically later as you go down each row. It does not matter whether each
row itself is ordered lexicographically.

For example, given the following table:

cba
daf
ghi

This is not ordered because of the a in the center. We can remove the second
column to make it ordered:

ca
df
gi

So your function should return 1, since we only needed to remove 1 column. As
another example, given the following table:

abcdef

Your function should return 0, since the rows are already ordered (there's only
one row). As another example, given the following table:

zyx
wvu
tsr

Your function should return 3, since we would need to remove all the columns to
order it.
"""

# import evaluation function
from ufunc_eval import ufunc_eval

def unordered_cols(mat):
    """
    O(nm) solution. we basically just look for columns that are not in the
    ascending lexicographic order that we desire. the semantics of the problem
    statement are a little confusing for some reason.
    """
    assert mat is not None
    # n rows, m columns
    nrow = len(mat)
    ncol = len(mat[0])
    # number of columns to remove
    rc = 0
    # for each column
    for col in range(ncol):
        # check each row entry to see if it is larger than the previous;
        # if this is not the case, increment rc and break the loop
        for row in range(1, nrow):
            if mat[row][col] < mat[row - 1][col]:
                rc = rc + 1
                break
    # return number of unordered columns to remove
    return rc

# main
if __name__ == "__main__":
    func = unordered_cols
    # problem input 1, answer is 1
    mat = [['c', 'b', 'a'], ['d', 'a', 'f'], ['g', 'h', 'i']]
    ufunc_eval(func, mat)
    # problem input 2, answer is 0
    mat = [['a', 'b', 'c', 'd', 'e', 'f']]
    ufunc_eval(func, mat)
    # problem input 3, answer is 3
    mat = [['z', 'y', 'x'], ['w', 'v', 'u'], ['t', 's', 'r']]
    ufunc_eval(func, mat)
