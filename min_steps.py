# daily coding problem #100 (easy)
#
# Changelog:
#
# 09-21-2019
#
# initial creation. the ability to move diagonally leads to a very easy way
# to make the optimal moves towards the next point.

__doc__ = """
This problem was asked by Google.

You are in an infinite 2D grid where you can move in any of the 8 directions:

 (x,y) to
    (x+1, y),
    (x - 1, y),
    (x, y+1),
    (x, y-1),
    (x-1, y-1),
    (x+1,y+1),
    (x-1,y+1),
    (x+1,y-1)

You are given a sequence of points and the order in which you need to cover the
points. Give the minimum number of steps in which you can achieve it. You start
from the first point.

Example:

Input: [(0, 0), (1, 1), (1, 2)]
Output: 2

It takes 1 step to move from (0, 0) to (1, 1). It takes one more step to move
from (1, 1) to (1, 2).
"""

# import evaluation function
from ufunc_eval import ufunc_eval
from math import inf

def min_steps(pts):
    """
    returns the minimum steps to traverse a series of points, given that we can
    move vertically, horizontally, and diagonally. if x or y coordinate of two
    points is the same, the number of steps is simply the difference between the
    differing coordinate. if both are the same, clearly we need 0 steps. if both
    are different, the number of steps is simply the max of the two, as we move
    diagonally to maximize distance covered by the smaller difference, and then
    move linearly with the remainder of the larger.
    """
    assert pts is not None
    # number of points to traverse
    n = len(pts)
    # if no points, return -inf
    if n == 0: return -inf
    # current position is initial point
    x, y = pts[0]
    # number of steps taken
    ns = 0
    # for each of the other points
    for i in range(1, n):
        # coords of ith point
        xi, yi = pts[i]
        # get abs differences in x, y coordinates
        dx, dy = abs(xi - x), abs(yi - y)
        # if dx is 0, add dy to ns
        if dx == 0: ns = ns + dy
        # else if y is 0, add dx to ns
        elif dy == 0: ns = ns + dx
        # else neither are 0; add min(dx, dy) to ns, and for whichever is chosen
        # subtract that from the other d*, then add the remaining to ns. this
        # whole process is the same as adding max(dx, dy) to ns
        else: ns = ns + max(dx, dy)
        # update position
        x, y = xi, yi
    # return ns
    return ns

# main
if __name__ == "__main__":
    func = min_steps
    # problem input, answer is 2
    pts = [(0, 0), (1, 1), (1, 2)]
    ufunc_eval(func, pts)
    # another input, answer is 24
    pts = [(3, 9), (-1, 3), (8, 7), (-1, 2)]
    ufunc_eval(func, pts)
