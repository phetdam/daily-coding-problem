# daily coding problem #15 (medium, nice and quick however)
#
# Changelog:
#
# 06-15-2019
#
# initial creation. used the "random points" method to determine pi. note that
# this method has a higher estimation variance than the average value method.

__doc__ = """
This problem was asked by Google.

The area of a circle is defined as pi * r^2. Estimate pi to 3 decimal places
using a Monte Carlo method. Hint: The basic equation of a circle is x2 + y2 = r2.
"""

# import evaluation function
from ufunc_eval import ufunc_eval

# needed to generate uniform randoms
import random

def pi_mc(n = 1000000, sig = 2, r = 1):
    """
    generates a monte carlo estimate of pi by seeing what fraction of generated
    points fall within a circle or radius r out of all the points generated
    within a box of side length 2r, centered at (0, 0). n controls the number of
    points to generate for the estimate. sig indicates to how many decimal
    places the generated estimate should express.
    """
    # number of points that fall within the unit circle
    n_in = 0
    # for n iterations, generate (x, y) pairs where -r <= x <= r, -r <= y <= r
    for i in range(n):
        x = random.uniform(-r, r)
        y = random.uniform(-r, r)
        # if we have that x ^ 2 + y ^ 2 <= r ^ 2, then it is in the unit circle
        # so we increment n_in (number of points in the unit circle)
        if x ** 2 + y ** 2 <= r ** 2: n_in = n_in + 1
    # calculate area of the unit circle (percentage of box area)
    A = (n_in / n) * (4 * r ** 2)
    # return estimate for pi
    return round(A / (r ** 2), ndigits = sig)

# main
if __name__ == "__main__":
    # more or less returns ~3.14 as desired. note that having a smaller value of
    # r will lead to a more accurate estimation, holding n constant.
    ufunc_eval(pi_mc, sig = 3, r = 0.05)
