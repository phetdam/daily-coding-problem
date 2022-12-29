# daily coding problem #143 (medium, takes a little thought)
#
# Changelog:
#
# 10-23-2019
#
# prima facie i could see that this is a similar problem to partitioning in
# quick sort. however, if there are multiple elements that equal the pivot,
# quick sort will arbitrarily arrange them to the left of the pivot. therefore,
# once the pivot's hypothetical position is determined, one has to partition
# the elements to the left of the pivot to ensure the order. i didn't do this
# problem in a single sitting; i had to take a break to think about it after
# i figured out the first half, which is setting all elements <= pivot to
# one side, like in quick sort.

__doc__ = """
This problem was asked by Amazon.

Given a pivot x, and a list lst, partition the list into three parts.

The first part contains all elements in lst that are less than x
The second part contains all elements in lst that are equal to x
The third part contains all elements in lst that are larger than x

Ordering within a part can be arbitrary.

For example, given x = 10 and lst = [9, 12, 3, 5, 14, 10, 10], one partition may
be [9, 3, 5, 10, 10, 12, 14].
"""

# import evaluation function
from ufunc_eval import ufunc_eval

def partition(ar, pv):
    """
    simple partitioning algorithm based on the quicksort partition. O(n), as it
    does the "standard" quicksort parititon before partioning just the elements
    on the left of the pivot group them with all tems equal to the pivot.
    """
    assert ar is not None
    # get length of the array
    n = len(ar)
    # return if array is empty or only has 1 element
    if n < 2: return ar
    # left and right indices starting at opposite ends
    li, ri = 0, n - 1
    # while the two indices are not overlapping, continue to swap when there is
    # an inversion (when ar[li] > pv and ar[ri] <= pv)
    while li < ri:
        # if there is an inversion; do <= only for right because we want all the
        # elements equal to the pivot on the left side of where the pivot would
        # hypothetically go in a normal quick sort partition, swap. the if
        # statements afterwards will move the pointer for you
        if (ar[li] > pv) and (ar[ri] <= pv): ar[li], ar[ri] = ar[ri], ar[li]
        # if ar[li] <= pv, continue to move li to the right. if ar[ri] > pv,
        # continue to move ri to the right.
        if ar[li] <= pv: li = li + 1
        if ar[ri] > pv: ri = ri - 1
    # since ri marks the last element that is <= pivot, we start from there
    # reset li
    li = 0
    # while the two indices are not overlapping, continue to swap while there is
    # an inversion (when ar[li] == pv and ar[ri] < pv)
    while li < ri:
        # if there is an inversion, swap
        if (ar[li] == pv) and (ar[ri] < pv): ar[li], ar[ri] = ar[ri], ar[li]
        # continue to advance pointers if ordering is ok
        if ar[li] < pv: li = li + 1
        if ar[ri] == pv: ri = ri - 1
    # return the rearranged array. note that there isn't a need to do this as we
    # partitioned in place, but for the sake of showing the results from the
    # evaluation function, we will keep it in here.
    return ar

# main
if __name__ == "__main__":
    func = partition
    # problem input, my answer is [9, 5, 3, 10, 10, 14, 12] (still valid as the
    # ordering in each partition can be arbitrary)
    ar = [9, 12, 3, 5, 14, 10, 10]
    pv = 10
    ufunc_eval(func, ar, pv)
    # same ar, different pv; my answer is [3, 5, 9, 10, 10, 14, 12]. note that
    # this time, pv is not even in the array!
    pv = 4
    ufunc_eval(func, ar, pv)
    # different ar, different pv; my answer is [4, 9, 6, 7, 5, 11, 11, 11, 34]
    ar = [4, 34, 6, 11, 5, 11, 11, 7, 9]
    pv = 11
    ufunc_eval(func, ar, pv)
