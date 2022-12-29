# daily coding problem #175 (easy)
#
# Changelog:
#
# 11-25-2019
#
# initial creation. not really hard, but was a pretty fun problem to do. i added
# an extra check that terminates the simulation whenever an absorbing state is
# reached. wouldn't exactly say this is easy, but it is straightforward.

__doc__ = """
This problem was asked by Google.

You are given a starting state start, a list of transition probabilities for a
Markov chain, and a number of steps num_steps. Run the Markov chain starting
from start for num_steps and compute the number of times we visited each state.

For example, given the starting state a, number of steps 5000, and the following
transition probabilities:

[
  ('a', 'a', 0.9),
  ('a', 'b', 0.075),
  ('a', 'c', 0.025),
  ('b', 'a', 0.15),
  ('b', 'b', 0.8),
  ('b', 'c', 0.05),
  ('c', 'a', 0.25),
  ('c', 'b', 0.25),
  ('c', 'c', 0.5)
]

One instance of running this Markov chain might produce

{ 'a': 3012, 'b': 1656, 'c': 332 }.
"""

# import evaluation function
from ufunc_eval import ufunc_eval
from random import random
from sys import stderr

def run_markov_chain(start, tps, n_steps = 1000):
    """
    we assume all states are a single char and transition probabilities are
    given in the format specified in the problem. the format of the transition
    probabilities will be changed to that of a dictionary with form

    {"a": {"a": 0.9, "b": 0.975, "c": 1}, "b": ... }

    for example, using the problem input. this allows us to easily figure out
    which state to transition into given a uniform [0, 1) draw using random().
    then, up to n_steps, we simply just count the number of times that we are
    each state. the starting state automatically has frequency 1 to start. if
    the starting state is not one of the states given, then all the entries of
    the dictionary of state counts will be 0. we assume that all the transition
    probabilities conditional on a starting state s add up to 1 as well.
    """
    # save name of function
    fname_ = run_markov_chain.__name__
    # bad asserts honestly; we are also assuming that the list of transition
    # probabilities is in the correct format.
    assert start is not None
    assert (tps is not None) and (len(tps) > 0)
    assert n_steps > 0
    # from transition probabilities, build dictionary
    tp_dict = {}
    # again we assume the format of the element is valid
    for (st, ed, tp) in tps:
        # if st is in the dictionary, we just add ed as an entry with tp + the
        # sum of all the other transition probabilities for tp_dict[st]; i.e.
        # plus the last cumulative transition probability. this aids our binning
        # process for when we simulate. here we also implicitly assume that each
        # ed associated with an st is unique. note: possible only because in
        # python the values are in insertion order.
        if st in tp_dict: tp_dict[st][ed] = list(tp_dict[st].values())[-1] + tp
        # else st is not in the dictionary, so we have to add an entry for st,
        # along with the inner dictionary containing the transition state
        else: tp_dict[st] = {ed: tp}
    # make a dictionary to track number of times each state has been visited
    st_dict = {}
    for k in tp_dict.keys(): st_dict[k] = 0
    # check if start is in st_dict; if not, then return st_dict
    if start not in st_dict: return st_dict
    # else it is in start dict, so begin markov chain simulation up to n_steps.
    # note that if we get stuck in with an entry like z: {z: 1}, then the
    # simulation will terminate as this is an absorbing state. however, there
    # will be a warning printed to stderr to inform the user of this.
    # current state
    cur = start
    for i in range(n_steps):
        # draw uniform [0, 1)
        r = random()
        # check tp_dict[cur] for potential cumulative transition probabilities;
        # transition to state k where tp_dict[cur][k] first strictly exceeds r
        for k, tp in tp_dict[cur].items():
            # test for absorbing state; if len(tp_dict[cur]) == 1 and tp == 1,
            # we are in an absorbing state, so warn and terminate early.
            if (len(tp_dict[cur]) == 1) and (tp == 1):
                print("{0}: absorbing state {1} reached; terminating early"
                      "".format(fname_, list(tp_dict[cur].keys())[0]),
                      file = stderr)
                return st_dict
            # if the cumulative transition probability is greater than r, mark
            # an additional visit to state k, and set cur to k before break
            if tp > r:
                st_dict[k] = st_dict[k] + 1
                cur = k
                break
        # simulate next transition
    # return st_dict
    return st_dict

# main
if __name__ == "__main__":
    func = run_markov_chain
    # problem input
    start, n_steps = 'a', 5000
    tps = [('a', 'a', 0.9),
           ('a', 'b', 0.075),
           ('a', 'c', 0.025),
           ('b', 'a', 0.15),
           ('b', 'b', 0.8),
           ('b', 'c', 0.05),
           ('c', 'a', 0.25),
           ('c', 'b', 0.25),
           ('c', 'c', 0.5)]
    ufunc_eval(func, start, tps, n_steps = n_steps)
    # markov chain with absorbing state a
    start, n_steps = 'b', 7000
    tps = [('a', 'a', 1),
           ('b', 'a', 0.1),
           ('b', 'b', 0.5),
           ('b', 'c', 0.1),
           ('b', 'd', 0.4),
           ('c', 'a', 0.2),
           ('c', 'b', 0.1),
           ('c', 'c', 0.25),
           ('c', 'd', 0.45),
           ('d', 'a', 0.1),
           ('d', 'b', 0.4),
           ('d', 'c', 0.2),
           ('d', 'd', 0.3)]
    ufunc_eval(func, start, tps, n_steps = n_steps)
