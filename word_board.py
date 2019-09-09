# daily coding problem #98 (easy, but tedious)
#
# Changelog:
#
# 09-08-2019
#
# initial creation. a lot of best-first search.

__doc__ = """
This problem was asked by Coursera.

Given a 2D board of characters and a word, find if the word exists in the grid.

The word can be constructed from letters of sequentially adjacent cell, where
"adjacent" cells are those horizontally or vertically neighboring. The same
letter cell may not be used more than once.

For example, given the following board:

[
  ['A','B','C','E'],
  ['S','F','C','S'],
  ['A','D','E','E']
]

exists(board, "ABCCED") returns true,
exists(board, "SEE") returns true,
exists(board, "ABCB") returns false.
"""

# import evaluation function
from ufunc_eval import ufunc_eval

def word_exists(mat, s):
    """
    description of solution runtime and any other comments.
    """
    assert (mat is not None) and (s is not None)
    # get dimensions of the board
    nrow = len(mat)
    ncol = len(mat[0])
    # length of the word s
    n = len(s)
    # perform best-only search from each vertex on the board
    for i in range(nrow):
        for j in range(ncol):
            # if mat[i][j] matches s[0], we may have a potential match
            if mat[i][j] == s[0]:
                # to prevent running over previously searched incomplete paths,
                # we make a matrix to mark which cells have been visited and
                # which cells have not been visited
                visited = [[False for _ in range(ncol)] for _ in range(nrow)]
                # next character in s to look for
                nci = 1
                # stack
                stk = [(i, j)]
                # while the stack is not empty and while nci < n
                while (len(stk) > 0) and (nci < n):
                    row, col = stk[-1]
                    # mark the cell as visited
                    visited[row][col] = True
                    # best-only search: add the neighbor that is not visited
                    # and exactly matches s[nci] and increment nci
                    # use value in row - 1, col
                    if (row > 0) and (mat[row - 1][col] == s[nci]) and \
                       (visited[row - 1][col] == False):
                        stk.append((row - 1, col))
                        nci = nci + 1
                    # else try value in row, col - 1
                    elif (col > 0) and (mat[row][col - 1] == s[nci]) and \
                         (visited[row][col - 1] == False):
                        stk.append((row, col - 1))
                        nci = nci + 1
                    # else try value in row + 1, col
                    elif (row < nrow - 1) and (mat[row + 1][col] == s[nci]) and \
                         (visited[row + 1][col] == False):
                        stk.append((row + 1, col))
                        nci = nci + 1
                    # else try value in row, col + 1
                    elif (col < ncol - 1) and (mat[row][col + 1] == s[nci]) and \
                         (visited[row][col + 1] == False):
                        stk.append((row, col + 1))
                        nci = nci + 1
                    # else none of the choices work, so pop and decrement nci
                    else:
                        stk.pop()
                        nci = nci - 1
                # if nci == n, we found the word
                if nci == n: return True
                # else try again
    # after looping, if we still found nothing, it does not exist
    return False

# main
if __name__ == "__main__":
    func = word_exists
    # problem input
    mat = [['A','B','C','E'],
           ['S','F','C','S'],
           ['A','D','E','E']]
    # problem case 1, answer is True
    s = "ABCCED"
    ufunc_eval(func, mat, s)
    # problem case 2, answer is True
    s = "SEE"
    ufunc_eval(func, mat, s)
    # problem case 3, answer is False
    s = "ABCB"
    ufunc_eval(func, mat, s)
    # another input, answer is True
    s = "CCBF"
    ufunc_eval(func, mat, s)
