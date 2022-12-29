# daily coding problem #151 (medium, pretty standard)
#
# Changelog:
#
# 10-31-2019
#
# initial creation. pretty standard breadth-first traversal type of problem.

__doc__ = """
Given a 2-D matrix representing an image, a location of a pixel in the screen
and a color C, replace the color of the given pixel and all adjacent same
colored pixels with C. For example, given the following matrix, and location
pixel of (2, 2), and 'G' for green:

B B W
W W W
W W W
B B B

Becomes

B B G
G G G
G G G
B B B
"""

# import evaluation function
from ufunc_eval import ufunc_eval

def flood_fill(img, loc, clr):
    """
    given a 2D matrix img, (x, y) point in the matrix loc, and a color clr, will
    fill loc and all adjacent points (assumed horizontally and vertically) with
    the given color clr. does this by breadth-first traversal of the grid.
    """
    assert (img is not None) and (clr is not None)
    # get dimensions of the image
    nrow = len(img)
    ncol = len(img[0])
    # if loc is invalid, do nothing and return pointer to img
    if (loc[0] < 0) or (loc[0] > nrow - 1) or (loc[1] < 0) or \
       (loc[1] > ncol - 1): return img
    # record color of loc
    loc_clr = img[loc[0]][loc[1]]
    # queue of (x, y) tuples; start with loc
    queue = [loc]
    # while queue is not empty
    while len(queue) > 0:
        # get the row, col coordinates of the loc_clr square
        r, c = queue.pop(0)
        # change the color from loc_clr to clr
        img[r][c] = clr
        # add any adjacent squares that are also loc_clr to the queue
        if (c > 0) and (img[r][c - 1] == loc_clr): queue.append((r, c - 1))
        if (r < nrow - 1) and (img[r + 1][c] == loc_clr):
            queue.append((r + 1, c))
        if (c < ncol - 1) and (img[r][c + 1] == loc_clr):
            queue.append((r, c + 1))
        if (r > 0) and (img[r - 1][c] == loc_clr): queue.append((r - 1, c))
    # when done, return the modified copy (even though it is in place)
    return img

# main
if __name__ == "__main__":
    func = flood_fill
    # problem input, answer is [['B', 'B', 'G'], ['G', 'G', 'G'], ['G', 'G',
    # 'G'], ['B', 'B', 'B']]
    img = [['B', 'B', 'W'],
           ['W', 'W', 'W'],
           ['W', 'W', 'W'],
           ['B', 'B', 'B']]
    loc, clr = (2, 2), 'G'
    ufunc_eval(func, img, loc, clr)
    # another input, answer is [['R', 'W', 'W', 'G'], ['R', 'B', 'B', 'B'],
    # ['G', 'B', B;, 'G']]
    img = [['R', 'W', 'W', 'G'],
           ['R', 'G', 'G', 'B'],
           ['G', 'B', 'B', 'G']]
    loc, clr = (1, 1), 'B'
    ufunc_eval(func, img, loc, clr)
