# daily coding problem #136 (medium, but simple as long as you are careful)
#
# Changelog:
#
# 10-16-2019
#
# initial creation. pretty straightforward problem; just need to make sure that
# every node added to the queue is marked as visited so you don't double add.

__doc__ = """
This question was asked by Google.

Given an N by M matrix consisting only of 1's and 0's, find the largest
rectangle containing only 1's and return its area.

For example, given the following matrix:

[[1, 0, 0, 0],
 [1, 0, 1, 1],
 [1, 0, 1, 1],
 [0, 1, 0, 0]]

Return 4.
"""

# import evaluation function
from ufunc_eval import ufunc_eval

def max_rect(mat):
    """
    given a matrix, search for starting points (1s) that have not been visited
    and then find the largest connected component of 1s. take the max for each
    of these areas (running max) and we have our answer.
    """
    if mat is None: return 0
    # get dimensions of mat
    nrow = len(mat)
    ncol = len(mat[0])
    # running max of area
    maxa = 0
    # visited matrix
    visited = [[False for _ in range(ncol)] for _ in range(nrow)]
    # for each (r, c) cell
    for r in range(nrow):
        for c in range(ncol):
            # if (r, c) is 1, and not visited, then we have a starting point
            if (mat[r][c] == 1) and (visited[r][c] == False):
                # queue for bfs
                queue = [(r, c)]
                # mark as visited
                visited[r][c] = True
                # number of 1s in this connected component
                area = 0
                # while queue is not empty
                while len(queue) > 0:
                    # get row and column
                    row, col = queue.pop(0)
                    # add 1 to area
                    area = area + 1
                    # add all neighbors that are within the matrix and 1s and
                    # have not been visited; also mark as visited
                    if (row < nrow - 1) and (mat[row + 1][col] == 1) and \
                       visited[row + 1][col] == False:
                        queue.append((row + 1, col))
                        visited[row + 1][col] = True
                    if (row > 0) and (mat[row - 1][col] == 1) and \
                       visited[row - 1][col] == False:
                        queue.append((row - 1, col))
                        visited[row - 1][col] = True
                    if (col < ncol - 1) and (mat[row][col + 1] == 1) and \
                       visited[row][col + 1] == False:
                        queue.append((row, col + 1))
                        visited[row][col + 1] = True
                    if (col > 0) and (mat[row][col - 1] == 1) and \
                       visited[row][col - 1] == False:
                        queue.append((row, col - 1))
                        visited[row][col - 1] = True
                # after the traversal, take max with maxa
                maxa = max(maxa, area)
    # return maxa
    return maxa

# main
if __name__ == "__main__":
    func = max_rect
    # problem input, answer is 4
    mat = [[1, 0, 0, 0],
           [1, 0, 1, 1],
           [1, 0, 1, 1],
           [0, 1, 0, 0]]
    ufunc_eval(func, mat)
    # another input, answer is 8
    mat = [[1, 1, 0, 1, 0],
           [1, 1, 1, 0, 1],
           [0, 0, 1, 0, 1],
           [0, 0, 1, 1, 0],
           [0, 1, 0, 0, 1],
           [0, 1, 1, 1, 1]]
    ufunc_eval(func, mat)
