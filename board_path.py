# daily coding problem #23 (easy, once you realize that this is just a single
# source shortest path problem with an unweighted graph)
#
# Changelog:
#
# 06-26-2019
#
# initial creation. i have seen these kinds of problems before, but today marks
# the first day where i realized that this type of unweighted path-finding
# problem can simply be reframed as a graph problem and solved in that manner.

__doc__ = """
This problem was asked by Google.

You are given an M by N matrix consisting of booleans that represents a board.
Each True boolean represents a wall. Each False boolean represents a tile you
can walk on.

Given this matrix, a start coordinate, and an end coordinate, return the minimum
number of steps required to reach the end coordinate from the start. If there is
no possible path, then return null. You can move up, left, down, and right. You
cannot move through walls. You cannot wrap around the edges of the board.

For example, given the following board:

[[f, f, f, f],
 [t, t, f, t],
 [f, f, f, f],
 [f, f, f, f]]

and start = (3, 0) (bottom left) and end = (0, 0) (top left), the minimum number
of steps required to reach the end is 7, since we would need to go through
(1, 2) because there is a wall everywhere else on the second row.
"""

# import evaluation function
from ufunc_eval import ufunc_eval

# get inf
from math import inf

def shortest_path(board, st, ed):
    """
    because we see that the board is a graph, with edges between neighboring
    nodes, we can use breadth-first search on this graph. we create a m x n
    matrix to hold the path lengths, and mark any visited/unreachable nodes
    with an integer value less than inf. specifically, we mark unreachable ones
    with -inf (since we will return the absolute value of the path, this makes
    for consistent output if we make this an ending point). since we use bfs,
    our complexity is O(mn) roughly.
    """
    # sanity checks for the board and the st, ed (row, col) tuples
    if (board is None) or (st is None) or (ed is None): return inf
    # get dimensions of board
    nrow = len(board)
    ncol = len(board[0])
    # if st or ed are outside of board dimensions, return inf
    if (st[0] < 0) or (st[0] > nrow - 1) or (st[1] < 0) or (st[1] > ncol - 1):
        return inf
    # create matrix of paths
    paths = [[inf for _ in range(ncol)] for _ in range(nrow)]
    # set any unreachable nodes to -inf
    for i in range(nrow):
        for j in range(ncol):
            if board[i][j] == True: paths[i][j] = -inf
    # put st in queue and begin bfs
    queue = [st]
    paths[st[0]][st[1]] = 0
    while len(queue) > 0:
        row, col = queue.pop(0)
        # path length at that cell
        cur_path = paths[row][col]
        # no need to mark as visited since the cell has a value < inf
        # for all non-visited valid neighbors, set their distances to
        # paths[row][col] + 1
        if (row - 1 >= 0) and (paths[row - 1][col] == inf):
            paths[row - 1][col] = cur_path + 1
            queue.append((row - 1, col))
        if (row + 1 < nrow) and (paths[row + 1][col] == inf):
            paths[row + 1][col] = cur_path + 1
            queue.append((row + 1, col))
        if (col - 1 >= 0) and (paths[row][col - 1] == inf):
            paths[row][col - 1] = cur_path + 1
            queue.append((row, col - 1))
        if (col + 1 < ncol) and (paths[row][col + 1] == inf):
            paths[row][col + 1] = cur_path + 1
            queue.append((row, col + 1))
    # return absolute value of path ending in et (in case et is unreachable)
    return abs(paths[ed[0]][ed[1]])

# main
if __name__ == "__main__":
    # function pointer
    func = shortest_path
    # given problem input, answer is 7
    board = [[False, False, False, False],
             [True, True, False, True],
             [False, False, False, False],
             [False, False, False, False]]
    st = (3, 0)
    ed = (0, 0)
    ufunc_eval(func, board, st, ed)
    # arbitrary "harder" inputs
    board = [[False, False, False, False],
             [True, True, False, True],
             [False, False, False, True],
             [False, True, False, False]]
    # answer should be 7 as well
    st = (0, 0)
    ed = (3, 0)
    ufunc_eval(func, board, st, ed)
    # answer should be 5
    st = (0, 3)
    ed = (3, 3)
    ufunc_eval(func, board, st, ed)
