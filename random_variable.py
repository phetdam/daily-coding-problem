# daily coding problem #152 (medium, but it is pretty simple)
#
# Changelog:
#
# 11-02-2019
#
# initial creation. got a little fancy with this one; beyond answering the
# question i also created a toy class to implement a random variable and have
# some driver code that procues two pretty plots that verify the answers. had
# to change the figure size a couple of times since my original 16 x 8 in.
# figure was not fully displaying on github correctly.

__doc__ = """
This problem was asked by Triplebyte.

You are given n numbers as well as n probabilities that sum up to 1. Write a
function to generate one of the numbers with its corresponding probability.

For example, given the numbers [1, 2, 3, 4] and probabilities [0.1, 0.5, 0.2,
0.2], your function should return 1 10% of the time, 2 50% of the time, and 3
and 4 20% of the time.

You can generate random numbers between 0 and 1 uniformly.
"""

# import evaluation function
from ufunc_eval import ufunc_eval
# import random
from random import random as rand_unif
# import matplotlib and seaborn to make density plots
import matplotlib.pyplot as plt
import seaborn as sns

# to make implementation more interesting, i will make a class for a discrete
# random variable. the corresponding function for this problem is the realize
# function that is in the class. default is bernoulli coin flip rv
class discrete_rv:

    def __init__(self, vals = [0, 1], probs = [0.5, 0.5]):
        # need to check that vals and probs are the same length, and that the
        # values in probs add up to 1
        assert len(vals) == len(probs)
        assert sum(probs) == 1
        # number of values/probabilities, values, and probabilities
        self.nvals = len(vals)
        self.vals = vals
        self.probs = probs
        # change probabilities to cutoffs. in the realize() function, when a
        # uniform [0, 1) variable is generated, vals[i] will be returned if
        # u < cuts[i] and u >= max(cuts[i - 1], 0). each cuts[i] is the
        # cumulative sum of probabilities 0 to i inclusive.
        self.cuts = [0 for _ in range(self.nvals)]
        for i in range(self.nvals):
            self.cuts[i] = sum(probs[:(i + 1)])

    def realize(self):
        """
        returns a realization of the discrete random variable. each vals[i] is
        returned at a corresponding frequency in probs[i].
        """
        # generate uniform rv
        u = rand_unif()
        # find first value in cuts[i] that is strictly larger than u, and then
        # return vals[i]
        i = 0
        while i < self.nvals:
            if u < self.cuts[i]: break
            i = i + 1
        # if there is a mistake return None
        return self.vals[i]

def n_realize(drv, n):
    """return n realizations of a discrete random variable object drv"""
    assert n > 0
    vals = [None for _ in range(n)]
    for i in range(n): vals[i] = drv.realize()
    return vals

# main
if __name__ == "__main__":
    # for each of the random variables, we collect n different realizations
    # and then generate a corresponding histogram/density plot. takes a few
    # seconds to run due to the importing of seaborn and matplotlib
    n = 60000
    # make a figure with multiple subplots
    fig, axs = plt.subplots(nrows = 1, ncols = 2, figsize = (10, 4.5))
    # problem input
    vals, probs = [1, 2, 3, 4], [0.1, 0.5, 0.2, 0.2]
    drv = discrete_rv(vals = vals, probs = probs)
    # plot histogram on corresponding axis with axis label
    sns.distplot(n_realize(drv, n), hist = True, kde = False, bins = 4,
                 hist_kws = {"edgecolor": "black"}, ax = axs[0],
                 axlabel = "problem input. probs = {0}".format(probs))
    # another random variable; biased dice roll
    vals, probs = [1, 2, 3, 4, 5, 6], [0.35, 0.3, 0.15, 0.1, 0.05, 0.05]
    drv = discrete_rv(vals = vals, probs = probs)
    # plot histogram on corresponding axis with axis label
    sns.distplot(n_realize(drv, n), hist = True, kde = False, bins = 6,
                 hist_kws = {"edgecolor": "black"}, ax = axs[1],
                 axlabel = "biased die. probs = {0}".format(probs))
    # save figure
    fig.savefig("drv_plots.png")
