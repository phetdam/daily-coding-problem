# daily coding problem #85 (medium)
#
# Changelog:
#
# 08-29-2019
#
# initial creation. was initially stuck on this but suddenly got the solution.

__doc__ = """
This problem was asked by Facebook.

Given three 32-bit integers x, y, and b, return x if b is 1 and y if b is 0,
using only mathematical or bit operations. You can assume b can only be 1 or 0.
"""

# import evaluation function
from ufunc_eval import ufunc_eval

def return_one(x, y, b):
    """
    returns x if b == 1, y if b == 0.
    """
    assert (b == 0) or (b == 1)
    return x * b + y * (1 - b)

# main
if __name__ == "__main__":
    func = return_one
    # input 1, returns 10
    x, y, b = 10, 5, 1
    ufunc_eval(func, x, y, b)
    # input 2, returns 99
    x, y, b = 16, 99, 0
    ufunc_eval(func, x, y, b)
