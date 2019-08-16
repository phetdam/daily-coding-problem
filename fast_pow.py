# daily coding problem #61 (medium, but pretty straightforward)
#
# Changelog:
#
# 08-16-2019
#
# initial creation. used a nice property of repeated multiplications to reduce
# the order of multiplications from O(y) to O(logy).

__doc__ = """
This problem was asked by Google.

Implement integer exponentiation. That is, implement the pow(x, y) function,
where x and y are integers and returns x^y. Do this faster than the naive method
of repeated multiplication. For example, pow(2, 10) should return 1024.
"""

# import evaluation function
from ufunc_eval import ufunc_eval

def fast_pow(base, exp):
    """
    O(logy) implementation of integer exponentiation. the naive method is O(y)
    in terms of multiplications used. we can reduce multiplications by using
    the fact that setting k = fast_pow(base, exp // 2), we can write that
    fast_pow(base, exp) = k * k * fast_pow(base, exp % 2). by doing this
    recursively, we achieve our O(logy) number of multiplications.
    """
    # sanity check although technically base could be a float too
    assert isinstance(base, int) and isinstance(exp, int)
    # base case: if exp is 0 or 1
    if exp == 0: return 1
    elif exp == 1: return base
    # if exp is odd, we will need to also multiply by base
    extra = 1
    if exp % 2 > 0: extra = base
    # get the square root of base ^ exp
    prod_sqrt = fast_pow(base, exp // 2)
    # return final product
    return extra * prod_sqrt * prod_sqrt

# main
if __name__ == "__main__":
    func = fast_pow
    # problem input, answer is 1024
    base = 2
    exp = 10
    ufunc_eval(func, base, exp)
    # another input, answer is 48828125
    base = 5
    exp = 11
    ufunc_eval(func, base, exp)
    # another input, answer is 117649
    base = 7
    exp = 6
    ufunc_eval(func, base, exp)
