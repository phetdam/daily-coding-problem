# daily coding problem #194 (easy)
#
# Changelog:
#
# 12-20-2019
#
# initial creation. not too hard a problem, but be a little careful not to mix
# up the notation. i spent some extra time to get a nice plotting function
# going just to make it more fun.

__doc__ = """
This problem was asked by Facebook.

Suppose you are given two lists of n points, one list p1, p2, ..., pn on the
line y = 0 and the other list q1, q2, ..., qn on the line y = 1. Imagine a set
of n line segments connecting each point pi to qi. Write an algorithm to
determine how many pairs of the line segments intersect.
"""

# import evaluation function
from ufunc_eval import ufunc_eval
# for graphing the line segments
from matplotlib.pyplot import subplots
from matplotlib.collections import LineCollection
# function for retrieving color maps
from matplotlib.cm import get_cmap
# function for convertin to RGBA
from matplotlib.colors import to_rgba

def n_isects(a, b):
    """
    it is clear that two segments (a1, a2), (b1, b2) intersect if sgn(a1 - b1)
    != sgn(a2 - b2). put another way, the segments don't intersect if and only
    if a1 > b1 and a2 > b2 or if a1 < b1 and a2 < b2. if we index our segments
    to be s1, s2, etc., we only compare each si with sj, j > i, to prevent
    double-counting our intersections.
    """
    assert (a is not None) and (b is not None) and (len(a) == len(b))
    # number of segments
    n = len(a)
    # number of intersections
    ni = 0
    # for each segment (ai, bi), check if it intersects with all (aj, bj), j > i
    for i in range(n):
        for j in range(i + 1, n):
            # calculate differences in starting and ending position
            ds = a[i] - a[j]
            de = b[i] - b[j]
            # if signs don't match, intersection
            if (ds / abs(ds) != de / abs(de)): ni = ni + 1
    # return ni
    return ni

def show_isects(a, b, out_file = "isects.png", figsize = (8, 3), linewidths = 1,
                cmap = "plasma"):
    """
    function that creates a .png file using matplotlib functions to show the
    intersections of the line segments. uses matplotlib.collections to add
    lots of lines at once to the plot. change named parameters as desired;
    out_file controls output file, figsize changes the image size, linewidths
    changes the default width of the segments, and cmap sets the color map.

    note: don't names that are LinearSegmentedColormaps!
    """
    assert (a is not None) and (b is not None) and (len(a) == len(b))
    # number of segments
    n = len(a)
    # for each segment (a[i], b[i]), change to cartesian coordinates; i.e.
    # => ((a[i], 0), (b[i], 1)) in order to be plotted correctly
    segs = [[(a[i], 0), (b[i], 1)] for i in range(n)]
    # get n colors and convert them to rgba
    colors = [to_rgba(c) for c in get_cmap(cmap, n).colors]
    # create plot grid, add LineCollection to axis with colors, autoscale axis
    fig, ax = subplots(nrows = 1, ncols = 1, figsize = figsize)
    ax.add_collection(LineCollection(segs, linewidths = linewidths,
                                     colors = colors))
    ax.autoscale()
    # save figure
    fig.savefig(out_file)
    # return fig, ax
    return fig, ax

# main
if __name__ == "__main__":
    func = n_isects
    # input, answer is 10
    a, b = [6, 2, 4, 7, -1, 8], [1, 8, 7, -4, 2, 6]
    ufunc_eval(func, a, b)
    show_isects(a, b)
