# daily coding problem #97 (medium, but straightforward)
#
# Changelog:
#
# 09-08-2019
#
# initial creation. basically make a dictionary with two levels of keys.

__doc__ = """
This problem was asked by Stripe.

Write a map implementation with a get function that lets you retrieve the value
of a key at a particular time. It should contain the following methods:

set(key, value, time): sets key to value for t = time.
get(key, time): gets the key at t = time.

The map should work like this. If we set a key at a particular time, it will
maintain that value forever or until it gets set at a later time. In other
words, when we get a key at a time, it should return the value that was set for
that key set at the most recent time.

Consider the following examples:

d.set(1, 1, 0) # set key 1 to value 1 at time 0
d.set(1, 2, 2) # set key 1 to value 2 at time 2
d.get(1, 1) # get key 1 at time 1 should be 1
d.get(1, 3) # get key 1 at time 3 should be 2

d.set(1, 1, 5) # set key 1 to value 1 at time 5
d.get(1, 0) # get key 1 at time 0 should be null
d.get(1, 10) # get key 1 at time 10 should be 1

d.set(1, 1, 0) # set key 1 to value 1 at time 0
d.set(1, 2, 0) # set key 1 to value 2 at time 0
d.get(1, 0) # get key 1 at time 0 should be 2
"""

# import evaluation function
from ufunc_eval import ufunc_eval
from math import inf

# simple map class with time history
class timed_map:

    # data must be of the format {key1: {t1: v1, ...}, ...}
    def __init__(self, data = None):
        if data is None: self.map = {}
        else: self.map = {k: v for k, v in data.items()}

    # set key/value association at a certain time
    def set(self, key, val, t):
        # check if there is a dict there; if so, directly index
        try:
            self.map[key]
        except KeyError: self.map[key] = {}
        self.map[key][t] = val

    # get value associated with key at certain time of retrieval
    def get(self, key, rt):
        # dictionary of time/value pairs
        look = self.map[key]
        # greatest time before retrieval time rt
        gtb = None
        # difference between gtb and rt
        gtd = inf
        # first try to find directly to avoid looping
        try:
            val = look[rt]
            # if successful, just return
            return val
        # if not found, we need to find greatest gtb < rt
        except KeyError:
            for k, v in look.items():
                # if k < rt, potential candidate for gtb, so take rt - k. if
                # rt - k is smaller than gtd, update gtd and gtb
                if k < rt:
                    if rt - k < gtd:
                        gtd = rt - k
                        gtb = k
        # if gtb is None, there is no value set at a time before rt, so None
        if gtb is None: return None
        # else return look[gtb], gtb <= rt
        return look[gtb]

# main
if __name__ == "__main__":
    # problem example 1, answers are 1 and 2
    tmap = timed_map()
    tmap.set(1, 1, 0)
    tmap.set(1, 2, 2)
    ufunc_eval(tmap.get, 1, 1)
    ufunc_eval(tmap.get, 1, 3)
    # problem example 2, answers are None and 1
    tmap = timed_map()
    tmap.set(1, 1, 5)
    ufunc_eval(tmap.get, 1, 1)
    ufunc_eval(tmap.get, 1, 10)
    # problem example 3, answer is 2
    tmap = timed_map()
    tmap.set(1, 1, 0)
    tmap.set(1, 2, 0)
    ufunc_eval(tmap.get, 1, 0)
