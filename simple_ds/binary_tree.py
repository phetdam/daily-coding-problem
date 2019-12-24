# contains simple binary tree implementation and related functions.
#
# Changelog:
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

def bt2ar(root, order = "inorder"):
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

# main
if __name__ == "__main__":
    print("{0}: do not run module as a script.".format(_MODULE_NAME),
          file = stderr)
