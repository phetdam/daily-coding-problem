# daily coding problem #187 (easy)
#
# Changelog:
#
# 12-24-2019
#
# initial creation. pretty standard problem; just be careful not to mix up the
# two rectangles that you are comparing (which i did).

__doc__ = """
This problem was asked by Google.

You are given given a list of rectangles represented by min and max x- and
y-coordinates. Compute whether or not a pair of rectangles overlap each other.
If one rectangle completely covers another, it is considered overlapping.

For example, given the following rectangles:

{
    "top_left": (1, 4),
    "dimensions": (3, 3) # width, height
},
{
    "top_left": (-1, 3),
    "dimensions": (2, 1)
},
{
    "top_left": (0, 5),
    "dimensions": (4, 3)
}

return true as the first and third rectangle overlap each other.
"""

# import evaluation function
from ufunc_eval import ufunc_eval

# so i don't have to type "top_left" and "dimensions" over and over again
_tl = "top_left"
_dim = "dimensions"

def has_overlap(rects):
    """
    given a list of rectangles as dicts, returns True if at least one pair is
    overlapping. O(n^2) worst case, due to pairwise comparisons.
    """
    assert rects is not None
    # number of rectangles
    n = len(rects)
    # check pairwise for overlaps
    for i in range(n):
        # compute coordinate ranges for width and height of rectangle i
        wl_i, wu_i = rects[i][_tl][0], rects[i][_tl][0] + rects[i][_dim][0]
        hl_i, hu_i = rects[i][_tl][1] - rects[i][_dim][1], rects[i][_tl][1]
        for j in range(i + 1, n):
            # compute coordinate ranges for width and height of rectangle j
            wl_j, wu_j = rects[j][_tl][0], rects[j][_tl][0] + rects[j][_dim][0]
            hl_j, hu_j = rects[j][_tl][1] - rects[j][_dim][1], rects[j][_tl][1]
            # if both the height and width ranges overlap, then there is overlap
            if (((wl_j >= wl_i) and (wl_j <= wu_i)) or
                ((wu_j >= wl_i) and (wu_j <= wu_i))) and \
                (((hl_j >= hl_i) and (hl_j <= hu_i)) or
                 ((hu_j >= hl_i) and (hu_j <= hu_i))):
                return True
    # else return False
    return False

# main
if __name__ == "__main__":
    func = has_overlap
    # problem input, answer is True
    rects = [{_tl: (1, 4), _dim: (3, 3)}, {_tl: (-1, 3), _dim: (2, 1)},
             {_tl: (0, 5), _dim: (4, 3)}]
    ufunc_eval(func, rects)
    # another input, answer is False
    rects = [{_tl: (3, 0), _dim: (7, 2)}, {_tl: (8, 7), _dim: (6, 4)},
             {_tl: (-3, -4), _dim: (6, 4)}, {_tl: (-7, 3), _dim: (7, 5)}]
    ufunc_eval(func, rects)
