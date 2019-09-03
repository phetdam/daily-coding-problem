# daily coding problem #89 (medium)
#
# Changelog:
#
# 09-03-2019
#
# initial creation. this problem is an extension of the standard problem, as it
# allows tree with duplicates nodes, as long as they are descendants of each
# other. howver, the problem's definition of a binary search tree is kind of
# problematic, as it is not strictly enforcing the search tree property: i.e.
# the definition given in the problem is loose enough that the following tree
#
#      5
#    /   \
#   3     9
#  / \   / \
# 1   4 2   11
#
# is technically valid under the problem definition. however, if you search for
# 2, you will never be able to find it using a standard binary search. we will
# also add in the condition that the node must be greater than or equal to all
# nodes in its left subtree, and must be less than or equal to all nodes in its
# right subtree. this makes the tree
#
#      7
#    /   \
#   3     7
#  / \   / \
# 1   4 7   11
#
# a valid tree that a standard binary search can work on, even though this is
# not a preferred way of handling duplicates. to spice things up, we provide two
# implementations: one recursive, and one iterative using bfs. both are good.

__doc__ = """
This problem was asked by LinkedIn.

Determine whether a tree is a valid binary search tree.

A binary search tree is a tree with two children, left and right, and satisfies
the constraint that the key in the left child must be less than or equal to the
root and the key in the right child must be greater than or equal to the root.
"""

# import evaluation function
from ufunc_eval import ufunc_eval
from math import inf

# simple tree class (i keep remaking this, so maybe i should make a generic
# class in this directory, even the task is trivial)
class bt_node:

    def __init__(self, data, left = None, right = None):
        self.data = data
        self.left = left
        self.right = right

def is_bst_rec(root, lo = -inf, hi = inf):
    """
    recursive implementation of a method to determine if a tree is a binary
    search tree or not, allowing duplicate nodes.
    """
    if root is None: return True
    # if root exceeds limits set by lo and hi, bst property is violated
    if (root.data < lo) or (root.data > hi): return False
    # check subtrees
    return is_bst_rec(root.left, lo = lo, hi = root.data) and \
        is_bst_rec(root.right, lo = root.data, hi = hi)

def is_bst_iter(root):
    """
    iterative implementation of a method to determine if a tree is a binary
    search tree or not, allowing duplicates nodes. uses breadth-first search
    to traverse the nodes in the tree.
    """
    if root is None: return True
    # each tuple in queue contains node, and lower, upper limits
    queue = [(root, -inf, inf)]
    # while we have not searched the whole tree
    while len(queue) > 0:
        # pop last node
        u, lo, hi = queue.pop(0)
        # check if the data in u violates its limits
        if (u.data < lo) or (u.data > hi): return False
        # if not, append its left and right child to queue if not None
        if u.left is not None: queue.append((u.left, lo, u.data))
        if u.right is not None: queue.append((u.right, u.data, hi))
    # successful completion indicates that the tree is a bst
    return True

# main
if __name__ == "__main__":
    func_a = is_bst_rec
    func_b = is_bst_iter
    # input 1, answer should be False
    root = bt_node(5,
                   left = bt_node(3,
                                  left = bt_node(1),
                                  right = bt_node(4)),
                   right = bt_node(9,
                                   left = bt_node(2),
                                   right = bt_node(11)))
    ufunc_eval(func_a, root)
    ufunc_eval(func_b, root)
    # input 2, answer should be True
    root = bt_node(7,
                   left = bt_node(3,
                                  left = bt_node(1),
                                  right = bt_node(4)),
                   right = bt_node(7,
                                   left = bt_node(7),
                                   right = bt_node(11)))
    ufunc_eval(func_a, root)
    ufunc_eval(func_b, root)
