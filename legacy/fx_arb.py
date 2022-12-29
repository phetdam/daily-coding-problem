# daily coding problem #32 (hard, but not really, unless i was supposed to come
# up with a solution that is much faster than O(n^3) search)
#
# Changelog:
#
# 07-05-2019
#
# initial creation. O(n^3) solution that tries all currency pairs and returns
# all possible triangular currency arbitrages. note that rates are of the form
# r(i, j), i.e. the rate quoted for i/j. so Ci * r(i, j) = Cj, which means that
# Ck * r(j, i) = Ci. weird way of quoting rates but easier for this problem.

__doc__ = """
This problem was asked by Jane Street.

Suppose you are given a table of currency exchange rates, represented as a 2D
array. Determine whether there is a possible arbitrage: that is, whether there
is some sequence of trades you can make, starting with some amount A of any
currency, so that you can end up with some amount greater than A of that
currency. There are no transaction costs and you can trade fractional
quantities.
"""

# import evaluation function
from ufunc_eval import ufunc_eval

def fx_arbs(rates, tol = 0.01):
    """
    the idea behind currency arbitrage is as follows: denote exchange rate
    r(i, j) as the rate such the rate is expressed as currency i / currency j.
    that is, j is the base currency, and i is the foreign currency, so some
    quantity of currency i Ci * r(i, j) will equal some quantity Cj of currency
    j, and Cj * r(j, i) will equal Ci. a currency arbitrage exists if for some
    starting capital X, we find that for three currencies i, j, and k, that

    X' = r(k, i)r(j, k)r(i, j)X >= X + tol, where X and X' are of currency i.

    the tol parameter sets a threshold for what counts as an "arbitage", as
    subpenny arbitrages are often theoretically possible but practially not
    possible/profitable. returns a list of all possible currency arbitrages,
    returning a list of coordinates (i, j) that indicate which rates to use.
    O(n^3) complete search for all possible triangular currency arbitrages.
    """
    # assume that the rates matrix is not None, and is at least 1 x 1 & square.
    nrc = len(rates)
    # list of possible arbitrages
    arbs = []
    # for simplicity, we assume starting capital is unit capital (1)
    # for each starting currency i
    for i in range(nrc):
        # for each first intermediate currency j that is not i
        for j in range(nrc):
            # exclude itself, as r(i, i) is always 1 for any currency i if j = i
            if j == i: continue
            # first each second intermediate currency k that is not i or j
            for k in range(nrc):
                # if k equals i or j, continue
                if (k == i) or (k == j): continue
                # else check if there is arbitrage; if so add to arbs. we make
                # the arbitrage condition looser, so that the profit margin to
                # become arbitrage is 0.01 of the starting currency.
                elif rates[i][j] * rates[j][k] * rates[k][i] >= 1 + tol:
                    arbs.append([(i, j), (j, k), (k, i)])
    # return list of arbitrages
    return arbs

# main
if __name__ == "__main__":
    # some matrix of rates: 0 (euro), 1 (dollar), 2(pound). remember that
    # rates[i][j] is same as quoting i/j. so foreign currency first.
    # the only arbitrages here should be the 3 permutations of the rates
    # r(0, 1), r(1, 2), and r(2, 0), i.e. eur/usd, use/gbp, gbp/eur. arbitrage
    # profit is 0.012 units of the starting currency.
    rates = [[1, 0.89, 1.12],
             [1 / 0.89, 1, 1.25],
             [0.91, 1 / 1.25, 1]]
    ufunc_eval(fx_arbs, rates)
