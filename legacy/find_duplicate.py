# daily coding problem #164 (medium)
#
# Changelog:
#
# 11-15-2019
#
# initial creation. if one recalls gauss's formula for summing 1, ... 100, and
# makes note of the fact that all the elements are unique except for 1, we can
# find the value of the duplicate by summing all the elements and subtracting
# out the sum of the unique elements, which is given by n(n + 1) / 2.

__doc__ = """
This problem was asked by Google.

You are given an array of length n + 1 whose elements belong to the set {1, 2,
..., n}. By the pigeonhole principle, there must be a duplicate. Find it in
linear time and space.
"""

# import evaluation function
from ufunc_eval import ufunc_eval

def find_duplicate(ar):
    """
    it is known that the sum of 1, ... n is n(n + 1) / 2. we can simply sum all
    the elements in the set and then subtract n(n + 1) / 2 from the sum.
    """
    assert (ar is not None) and (len(ar) > 0)
    # get number of unique elements in the array
    n = len(ar) - 1
    # return difference between sum and sum of 1, ... n
    return sum(ar) - n * (n + 1) // 2

# main
if __name__ == "__main__":
    func = find_duplicate
    # input, answer is 4
    ar = [1, 2, 3, 4, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14]
    ufunc_eval(func, ar)
    # another input, answer is 9
    ar = [1, 2, 3, 4, 5, 6, 7, 8, 9, 9, 10, 11]
    ufunc_eval(func, ar)
