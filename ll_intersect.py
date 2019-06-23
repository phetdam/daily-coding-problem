# daily coding problem #20 (easy, given the conditions of the question.)
#
# Changelog:
#
# 06-23-2019
#
# initial creation. note that this problem makes several simplifying
# assumptions to the problem. one of these assumptions that if two nodes
# a, b have same data k, then a == b. this assumption is significant as
# it allows us to say to lists "intersect" even if they actually do not.
# a more robust test would be that of actual intersection, i.e. if the
# two lists share the same pointer at some point.

__doc__ = """
This problem was asked by Google.

Given two singly linked lists that intersect at some point, find the
intersecting node. The lists are non-cyclical. For example, given A = 3 -> 7 ->
8 -> 10 and B = 99 -> 1 -> 8 -> 10, return the node with value 8. In this
example, assume nodes with the same value are the exact same node objects.

Do this in O(M + N) time (where M and N are the lengths of the lists) and
constant space.
"""

# import evaluation function
from ufunc_eval import ufunc_eval

# simple singly linked node class
class node:
    def __init__(self, data):
        self.data = data
        self.next = None

def arrtoll(ar):
    """
    converts an integer array into a linked list, returning its root
    """
    if (ar is None) or (len(ar) == 0): return None
    # make root node with first element
    root = node(ar[0])
    # pointer to current node
    cur = root
    for i in range(1, len(ar)):
        cur.next = node(ar[i])
        cur = cur.next
    return root

def ll_poi(lroot, rroot):
    """
    O(m + n) solution, where m and n are the lengths of the respective linked
    lists. the trick is to first traverse both lists to get their lengths. if
    the linked lists intersect at one point, then from the node of intersection,
    will have the same values. we have the conditions that if nodes a, b have
    the same value, then a == b, which makes this easy (not actually testing if
    the pointers are the same, although that would be more correct). ll_poi
    stands for linked list point of intersect.
    """
    if (lroot is None) or (rroot is None): return None
    # lengths of lroot list and rroot lists
    llen = rlen = 0
    # current node pointer
    cur = lroot
    # get length of both lists
    while cur is not None:
        cur = cur.next
        llen = llen + 1
    cur = rroot
    while cur is not None:
        cur = cur.next
        rlen = rlen + 1
    # for the longer list, traverse |llen - rlen| nodes. make lcur and rcur to
    # prevent confusion between the two
    lcur = lroot
    rcur = rroot
    if llen > rlen:
        for i in range(abs(llen - rlen)): lcur = lcur.next
    elif llen < rlen:
        for i in range(abs(llen - rlen)): rcur = rcur.next
    # if they are equal, no need to do anything
    # return data where both lcur and rcur have same data
    while (lcur is not None) and (rcur is not None):
        if lcur.data == rcur.data: return lcur.data
        lcur = lcur.next
        rcur = rcur.next
    # if they don't intersect, return None
    return None

# main
if __name__ == "__main__":
    # input example given by problem
    lla = arrtoll([3, 7, 8, 10])
    llb = arrtoll([99, 1, 8, 10])
    ufunc_eval(ll_poi, lla, llb)
    # random example (slightly violates problem input restrictions)
    lla = arrtoll([5, 3, 6, 2, 7, 1, 8, 9])
    llb = arrtoll([3, 4, 2, 7, 6, 1, 8, 9])
    ufunc_eval(ll_poi, lla, llb)
