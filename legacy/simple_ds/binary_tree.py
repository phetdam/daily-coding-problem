# contains simple binary tree implementation and related functions.
#
# Changelog:
#
# 12-24-2019
#
# added function make_bt that allows one to make a binary tree of arbitrary size
# with elements filled left to right. allows one to pass in a function to use
# the number (k - 1) of created elements to determine the value of the kth node.
# changed the default ordering for bt2ar to "levelorder".
#
# 12-23-2019
#
# initial creation. added simple binary tree class and bt2ar, a function for
# taking a tree and return elements of the tree in a list based on a specified
# traversal order; default inorder element ordering.

__doc__ = """
simple_ds.binary_tree: module containing simple implementations of binary trees
for software engineering interviews, and related helpful functions.
"""
from sys import stderr
from math import log2

_MODULE_NAME = "simple_ds.binary_tree"
# possible orderings for bt2ar
_orders = ["inorder", "preorder", "postorder", "levelorder"]

class bt_node:
    """
    bare-bones implementation of a binary tree. makes it easy to create a whole
    binary tree in one call by using the left and right named arguments in the
    constructor. for example, one can create a tree by

    from simple_ds.binary_tree import bt_node
    my_tree = bt_node(5,
                      left = bt_node(-1,
                                     left = bt_node(7),
                                     right = bt_node(23)),
                      right = bt_node(19,
                                      left = bt_node(8,
                                                     right = bt_node(-9))))
    """
    def __init__(self, data, left = None, right = None):
        self.data = data
        self.left = left
        self.right = right

def bt2ar(root, order = "levelorder"):
    """
    returns the elements of a binary tree as a list, default with the inorder
    element ordering. if None is passed, then an empty list will be returned.
    possible orderings are "inorder", "preorder", "postorder", and "levelorder".
    the "levelorder" ordering gives the output of a breadth-first traversal.
    """
    fname = bt2ar.__name__
    # if None, return empty list
    if root is None: return []
    # check type
    if not isinstance(root, bt_node):
        raise TypeError("{0}: root must be of type bt_node".format(fname))
    # check if order is valid
    if order not in _orders:
        raise ValueError("{0}: valid orderings are {1}".format(fname, _orders))
    # traverse in specified order, appending elements to outl
    outl = []
    # if order is "inorder"
    if order == "inorder":
        def inorder(u):
            if u is not None:
                inorder(u.left)
                outl.append(u.data)
                inorder(u.right)
        inorder(root)
    # else if order is "preorder"
    elif order == "preorder":
        def preorder(u):
            if u is not None:
                outl.append(u.data)
                preorder(u.left)
                preorder(u.right)
        preorder(root)
    # else if order is "postorder"
    elif order == "postorder":
        def postorder(u):
            if u is not None:
                postorder(u.left)
                postorder(u.right)
                outl.append(u.data)
        postorder(root)
    # else if order is "levelorder"
    elif order == "levelorder":
        queue = [root]
        while len(queue) > 0:
            u = queue.pop(0)
            outl.append(u.data)
            if u.left is not None: queue.append(u.left)
            if u.right is not None: queue.append(u.right)
    # return outl
    return outl

def make_bt(n = 15, func = lambda x: x):
    """
    make a binary tree with n elements, where elements are filled left to right;
    i.e. the function will attempt to create a complete binary tree. the func
    named argument specifies how to generate values for the tree, operating
    individually on each node based on the number of nodes that have been
    created before the node, so the first node has value 0, the kth value k - 1,
    etc. the default is an identity function. some examples of usage are

    from random import uniform, gauss
    make_bt(100) # identity
    make_bt(23, func = lambda x: 2 ** x) # powers of 2, starting from 1
    make_bt(160, func = lambda x: uniform(4, 7)) # uniform(4, 7) draw
    make_bt(24, func = lambda x: gauss(-3 + x / 10, x + 1)) # x-dependent normal
    make_bt(55, func = lambda x: x + 1) # natural number ordering
    """
    fname = make_bt.__name__
    # check type of n and enforce nonnegativity
    if not isinstance(n, int):
        raise TypeError("{0}: n must be a nonnegative integer".format(fname))
    if n < 0:
        raise ValueError("{0}: n must be a nonnegative integer".format(fname))
    # if n is 0, return None
    if n == 0: return None
    # else create a tree with n nodes. take d := int(log2(n)) to get the depth
    # of the tree, and build the tree in level order, i.e. breadth-first. after
    # building the root, build an even number of nodes, i.e. ((n - 1) // 2) * 2,
    # and then add 1 if n was initially even.
    # root of tree we will create
    root = bt_node(func(0))
    # number of created nodes, remaining nodes to create as an even number
    ncn, nre = 1, ((n - 1) // 2) * 2
    # build nre nodes
    queue = [root]
    while ncn < nre + 1:
        u = queue.pop(0)
        # build a left and right child for u, add them to the queue, and
        # increment ncn by 2 since we are creating two nodes at a time
        u.left = bt_node(func(ncn))
        u.right = bt_node(func(ncn + 1))
        queue.append(u.left)
        queue.append(u.right)
        ncn = ncn + 2
    # if nre = n - 1, then n is odd, and we are done, so return root
    if nre == n - 1: return root
    # else take the first remaining node in queue and give it a left child
    queue[0].left = bt_node(func(n - 1))
    return root

# main
if __name__ == "__main__":
    print("{0}: do not run module as a script.".format(_MODULE_NAME),
          file = stderr)
