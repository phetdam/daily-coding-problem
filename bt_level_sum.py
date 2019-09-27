# daily coding problem #117 (easy)
#
# Changelog:
#
# 09-27-2019
#
# initial creation. not the easiest problem, but straightforward modification
# of a breadth-first search traversal.

__doc__ = """
This problem was asked by Facebook.

Given a binary tree, return the level of the tree with minimum sum.
"""

# import evaluation function
from ufunc_eval import ufunc_eval
from math import inf

class bt_node:

    def __init__(self, data, left = None, right = None):
        self.data = data
        self.left = left
        self.right = right

def min_level_sum(root):
    """
    uses breadth-first traversal to get the level sums for each level, and then
    returns the level with the lowest sum. uses two queues; one for the current
    level, and one for the subsequent level. root is level 1. if there are
    levels that have the same level sums, the earliest level will be returned.
    """
    if root is None: return 0
    # make cur, next queue
    qcur, qnext = [root], []
    # dict keyed as level: sum
    look = {1: 0}
    # current tree level
    lvl = 1
    # while the current queue is not None
    while len(qcur) > 0:
        # pop current node
        u = qcur.pop(0)
        # add value to current level sum
        look[lvl] += u.data
        # add any non-None children to qnext
        if u.left is not None: qnext.append(u.left)
        if u.right is not None: qnext.append(u.right)
        # if the length of the current queue is 0, make it point to qnext, and
        # set qnext to an empty list. then increase level number by 1 and make
        # a corresponding entry in look.
        if len(qcur) == 0:
            qcur = qnext
            qnext = []
            lvl = lvl + 1
            look[lvl] = 0
    # return level with lowest sum; in the case of levels having the same sum,
    # the shallowest level will be preferred.
    min_l, min_s = 0, inf
    for k, v in look.items():
        if v < min_s: min_l, min_s = k, v
    return min_l

# main
if __name__ == "__main__":
    func = min_level_sum
    # input 1, answer is 3
    root = bt_node(-5,
                   left = bt_node(8,
                                  left = bt_node(-1),
                                  right = bt_node(-4)),
                   right = bt_node(-10,
                                   left = bt_node(10),
                                   right = bt_node(-100)))
    ufunc_eval(func, root)
    # input 2, answer is 4
    root = bt_node(40,
                   left = bt_node(-8,
                                  left = bt_node(-4),
                                  right = bt_node(14,
                                                  left = bt_node(-8))),
                   right = bt_node(10,
                                   left = bt_node(-5),
                                   right = bt_node(8,
                                                   right = bt_node(-16))))
    ufunc_eval(func, root)
