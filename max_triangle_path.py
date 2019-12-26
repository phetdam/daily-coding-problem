# daily coding problem #201 (easy, but takes a bit of thought and care)
#
# Changelog:
#
# 12-26-2019
#
# initial creation. i had the right idea in first figuring out the possible ways
# each ith path could be updated from the previous iteration of paths, but got
# the wrong answers initially since i forgot that by only using one path array,
# i was overwriting the previous solutions elementwise. making a new array for
# the results of the current iteration solved this problem. not exactly sure
# this is an easy problem per se honestly.

__doc__ = """
This problem was asked by Google.

You are given an array of arrays of integers, where each array corresponds to a
row in a triangle of numbers. For example, [[1], [2, 3], [1, 5, 1]] represents
the triangle:

  1
 2 3
1 5 1

We define a path in the triangle to start at the top and go down one row at a
time to an adjacent value, eventually ending with an entry on the bottom row.
For example, 1 -> 3 -> 5. The weight of the path is the sum of the entries.

Write a program that returns the weight of the maximum weight path.
"""

# import evaluation function
from ufunc_eval import ufunc_eval

def max_triangle_path(ar, trace = False):
    """
    O(N) solution, where N is the number of elements in our triangle. since when
    we advance rows, we can only advance to adjacent elements, if we were to
    rearrange the equilateral triangle as a left-justified right triangle, each
    kth row has k elements, and for each ith element in each row we can go to
    the next row's ith element or (i + 1)th element. it is clear to see that for
    each kth row, the first and last elements of the kth row can only be reached
    in one way from the (k - 1)th row, while the other k - 2 elements can be
    reached in two different ways; i.e. one can get to the ith element in the
    kth row, i > 0, i < k, from the (i - 1)th or ith element from the (k - 1)th
    row, so we will have to take the max of paths (i - 1) and i when adding to
    path weight of path i when looking at row k. if trace is True, then the path
    weights will be printed out at every iteration alongside the row elements,
    which is good for simple debugging when there aren't too many rows.
    """
    assert ar is not None
    # length of ar
    n = len(ar)
    # if n == 0, trivial return
    if n == 0: return 0
    # path weights; initialize the first element
    paths = [0 for _ in range(n)]
    paths[0] = ar[0][0]
    # debug for trace == True
    if trace == True:
        print("{0:{2}} | {1}".format(str(ar[0]), paths, len(str(ar[-1]))))
    # for each subsequent array in ar, iteratively construct max paths
    for i in range(1, n):
        # create array of new paths to prevent overwriting
        new_paths = [0 for _ in range(n)]
        # array i has i + 1 elements, while the (i - 1)th array has i elements.
        # there is only one way to the 0th and ith elements in array i, from
        # the 0th and (i - 1)th paths, where we add the respective weights from
        # the ith array to the 0th and ith elements.
        new_paths[0] = paths[0] + ar[i][0]
        new_paths[i] = paths[i - 1] + ar[i][i]
        # for the i - 2 elements in the middle, there are two choices; for each
        # jth element, we take max of (j - 1)th and jth path from the previous
        # path weights and add the value of the current step.
        for j in range(1, i):
            new_paths[j] = max(paths[j - 1], paths[j]) + ar[i][j]
        # replace paths with new_paths
        paths = new_paths
        # debug for trace == True
        if trace == True:
            print("{0:{2}} | {1}".format(str(ar[i]), paths, len(str(ar[-1]))))
    # take final max of paths to return
    return max(paths)

# main
if __name__ == "__main__":
    func = max_triangle_path
    # problem input, answer is 9
    ar = [[1], [2, 3], [1, 5, 1]]
    ufunc_eval(func, ar)
    # another input, answer is 25
    ar = [[1], [2, 4], [1, 3, 5], [6, 2, 3, 7], [4, 9, 3, 8, 1]]
    ufunc_eval(func, ar, trace = True)
    # another input, answer is 49
    ar = [[9], [8, 7], [1, 2, 4], [-1, 3, 8, 10], [11, 2, 5, 9, 8],
          [3, 1, -7, 2, -5, 11]]
    ufunc_eval(func, ar)
