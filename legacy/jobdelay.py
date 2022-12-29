# daily coding problem #10 (medium, not really sure what they want to do)
#
# Changelog:
#
# 06-12-2019
#
# initial creation. not really sure if this was what i was supposed to do.
# jobdelay delays a specified function for w milliseconds, and takes whatever
# unnamed and named arguments that the specified function func takes. not sure
# if the problem was supposed to be trivial or not.

__doc__ = """
This problem was asked by Apple.

Implement a job scheduler which takes in a function f and an integer n, and
calls f after n milliseconds.
"""

# import evaluation function
from ufunc_eval import ufunc_eval

# need time to sleep
import time

def jobdelay(func, w, *args, **kwargs):
    """
    trivial implementation of what was asked. not sure if this is what was
    supposed to be the answer to the problem. takes function func, will delay
    it by w milliseconds, before executing func. takes any other unnamed and
    named arguments that func takes, passing them to func
    """
    # sanity checks
    if func is None: return None
    if w < 0: raise ValueError("{0}: positive delay required"
                               "".format(jobdelay.__name__))
    # sleep w milliseconds
    time.sleep(w / 1000)
    return func(*args, **kwargs)

# random test function
def _f(x, y, a = -1, b = -2):
    return pow(x * y, a ^ b)

# main
if __name__ == "__main__":
    args = [0.7, 0.9]
    kwargs = {"a": 17, "b": 8}
    w = 190 # in milliseconds
    ufunc_eval(jobdelay, _f, w, *args, **kwargs)
