# daily coding problem #84 (medium, just a little tedious)
#
# Changelog:
#
# 08-29-2019
#
# initial creation. saw immediately that i could just do multiple bread-first
# searches to solve this problem. initially ran into some difficulties because
# i had elifs in place of ifs, which totally changes the code meaning.

__doc__ = """
This problem was asked by Amazon.

Given a matrix of 1s and 0s, return the number of "islands" in the matrix. A 1
represents land and 0 represents water, so an island is a group of 1s that are
neighboring whose perimeter is surrounded by water.

For example, this matrix has 4 islands.

1 0 0 0 0
0 0 1 1 0
0 1 1 0 0
0 0 0 0 0
1 1 0 0 1
1 1 0 0 1

"""

# import evaluation function
from ufunc_eval import ufunc_eval

def n_islands(mat):
    """
    given the n x n matrix mat, where 1 indicates land and 0 represents water,
    returns in O(n^2) time the number of "islands", or areas of 1s containing no
    horizontal, vertical, or diagonal connections to other 1s; i.e. must be
    completely surrounded by cells of 0. simply check each non-visited cell,
    explore any connected area, and count connected components.
    """
    assert mat is not None
    # get matrix dimensions
    nrow = len(mat)
    ncol = len(mat[0])
    # visited matrix
    visited = [[False for _ in range(ncol)] for _ in range(nrow)]
    # number of islands
    iln = 0
    # first mark all 0s as visited
    for i in range(nrow):
        for j in range(ncol):
            if mat[i][j] == 0: visited[i][j] = True
    # for each unvisited piece of land
    for i in range(nrow):
        for j in range(ncol):
            # if cell is 1 and unvisited
            if (mat[i][j] == 1) and (visited[i][j] == False):
                # perform bfs and mark every unvisited 1 as visited
                queue = [(i, j)]
                visited[i][j] = True
                # while queue is not empty
                while len(queue) > 0:
                    # dequeue (already marked as visited)
                    r, c = queue.pop(0)
                    # add any neighbor that is within matrix bounds and is not 0
                    # nor a visited 1 into the queue
                    # stuff in previous row
                    if r > 0:
                        # prev diagonal left, prev above, prev diagonal right
                        if (c > 0) and (visited[r - 1][c - 1] == False):
                            queue.append((r - 1, c - 1))
                            visited[r - 1][c - 1] = True
                        if visited[r - 1][c] == False:
                            queue.append((r - 1, c))
                            visited[r - 1][c] = True
                        if (c < ncol - 1) and (visited[r - 1][c + 1] == False):
                            queue.append((r - 1, c + 1))
                            visited[r - 1][c + 1] = True
                    # stuff in current row; cur left and cur right
                    if (c > 0) and (visited[r][c - 1] == False):
                        queue.append((r, c - 1))
                        visited[r][c - 1] = True
                    if (c < ncol - 1) and (visited[r][c + 1] == False):
                        queue.append((r, c + 1))
                        visited[r][c + 1] = True
                    # stuff in next row
                    if r < nrow - 1:
                        # next diagonal left, next below, next diagonal right
                        if (c > 0) and (visited[r + 1][c - 1] == False):
                            queue.append((r + 1, c - 1))
                            visited[r + 1][c - 1] = True
                        if visited[r + 1][c] == False:
                            queue.append((r + 1, c))
                            visited[r + 1][c] = True
                        if (c < ncol - 1) and (visited[r + 1][c + 1] == False):
                            queue.append((r + 1, c + 1))
                            visited[r + 1][c + 1] = True
                # after completion, increment iln (one more island)
                iln = iln + 1
            # else do nothing; cell is either visited 1 or 0 (visited)
    # return number of islands
    return iln

# main
if __name__ == "__main__":
    func = n_islands
    # problem input, answer is 4
    mat = [[1, 0, 0, 0, 0],
           [0, 0, 1, 1, 0],
           [0, 1, 1, 0, 0],
           [0, 0, 0, 0, 0],
           [1, 1, 0, 0, 1],
           [1, 1, 0, 0, 1]]
    ufunc_eval(func, mat)
    # another input, answer is 4 again
    mat = [[1, 0, 1, 0, 0, 0],
           [1, 1, 1, 0, 0, 1],
           [0, 0, 0, 0, 1, 0],
           [1, 0, 0, 1, 0, 1],
           [1, 0, 0, 0, 0, 0],
           [0, 1, 0, 0, 1, 1]]
    ufunc_eval(func, mat)
