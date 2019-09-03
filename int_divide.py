# daily coding problem #88 (medium, but simple)
#
# Changelog:
#
# 09-03-2019
#
# initial creation. completed this in pseudocode a couple days ago but never had
# the time to actually code it up.

__doc__ = """
This question was asked by ContextLogic.

Implement division of two positive integers without using the division,
multiplication, or modulus operators. Return the quotient as an integer,
ignoring the remainder.
"""

# import evaluation function
from ufunc_eval import ufunc_eval

def int_divide(a, b):
    """
    implements integer division without using the *, /, or % operators. because
    of this restriction, we just choose to use addition and subtraction.
    """
    assert (a > 0) and (b > 0)
    # integer quotient
    iq = 0
    # subtract b from a, incrementing iq, until a < b
    while b <= a:
        a = a - b
        iq = iq + 1
    # return integer quotient
    return iq

# main
if __name__ == "__main__":
    func = int_divide
    # input 1, should return 6
    a, b = 19, 3
    ufunc_eval(func, a, b)
    # input 2, should return 12
    a, b = 38, 3
    ufunc_eval(func, a, b)
    # input 3, should return 7
    a, b = 21, 3
    ufunc_eval(func, a, b)
