# daily coding problem #146 (medium, standard binary tree problem)
#
# Changelog:
#
# 10-26-2019
#
# initial creation. pretty standard, straightforward binary tree question.

__doc__ = """
This question was asked by BufferBox.

Given a binary tree where all nodes are either 0 or 1, prune the tree so that
subtrees containing all 0s are removed.

For example, given the following tree:

   0
  / \
 1   0
    / \
   1   0
  / \
 0   0

should be pruned to:

   0
  / \
 1   0
    /
   1
We do not remove the tree at the root or its left child because it still has a 1
as a descendant.
"""

# import evaluation function
from ufunc_eval import ufunc_eval

# your basic standard binary tree note
class bt_node:

    def __init__(self, data, left = None, right = None):
        self.data = data
        self.left = left
        self.right = right

def zero_prune(root):
    """
    runs recursively to remove all subtrees that contain only 0. base case is
    for a single node; if it is 0, then None (prune) is returned. for the
    recursive case, any subtree that returns None will be pruned, and if both
    return None and the root node is also a 0, then prune the whole tree (None).
    """
    # if node is None just return None
    if root is None: return None
    # assign left node to whatever the return value should be
    root.left = zero_prune(root.left)
    # do the same to the right node
    root.right = zero_prune(root.right)
    # if both nodes are None and root is 0, return None (prune whole tree)
    if (root.left is None) and (root.right is None) and (root.data == 0):
        return None
    # else return root
    return root

def bt_inorder(root, outl = None):
    """
    returns all the tree values in an array inorder. outl is the output array.
    """
    # if outl is None, this is the parent, so set to empty list
    if outl is None: outl = []
    # if root is None, return outl
    if root is None: return outl
    # else traverse inorder, appending to outl
    bt_inorder(root.left, outl = outl)
    outl.append(root.data)
    bt_inorder(root.right, outl = outl)
    # return outl
    return outl

# main
if __name__ == "__main__":
    func = zero_prune
    # problem input, answer (inorder) is [1, 0, 1, 0]
    root = bt_node(0,
                   left = bt_node(1),
                   right = bt_node(0,
                                   left = bt_node(1,
                                                  left = bt_node(0),
                                                  right = bt_node(0)),
                                   right = bt_node(0)))
    root = ufunc_eval(func, root)
    print(bt_inorder(root))
    # another input, answer is [1, 0, 1, 1]
    root = bt_node(1,
                   left = bt_node(0),
                   right = bt_node(0,
                                   left = bt_node(0,
                                                  left = bt_node(0),
                                                  right = bt_node(0)),
                                   right = bt_node(1,
                                                   left = bt_node(0),
                                                   right = bt_node(1))))
    root = ufunc_eval(func, root)
    print(bt_inorder(root))
