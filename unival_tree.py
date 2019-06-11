# daily coding problem #8 (easy, but not super easy)
#
# Changelog:
#
# 06-10-2019
#
# initial creation. completed in one shot using a recursive solution. it is
# important to get the special cases for a unival tree correct, and to realize
# that whether or not a tree rooted at root v is a unival tree itself depends
# on the history of whether or not the subtrees themselves are unival trees.

__doc__ = """
This problem was asked by Google.

A unival tree (which stands for "universal value") is a tree where all nodes
under it have the same value. Given the root to a binary tree, count the number
of unival subtrees. For example, the following tree has 5 unival subtrees:

   0
  / \
 1   0
    / \
   1   0
  / \
 1   1

"""

# import evaluation function
from ufunc_eval import ufunc_eval

# simple binary tree implementation
class tnode:
    def __init__(self, val, left = None, right = None):
        self.val = val
        self.left = left
        self.right = right

def num_uvt(root):
    """
    recursive O(n) solution, where n is the number of nodes in the tree. since
    we know that all leaves are unival trees, we can easily define the recursive
    solution: for a binary tree with root v, number of unival trees N satisfies

    N(v) = N(v.left) + N(v.right) + 1(special cases)

    here 1() is the indicator function, 1 if the immediate tree rooted by v
    is a unival tree, 0 if not. however, it is not sufficient to simply check if
    the left and right node's values are NULL or the same as the root's--this
    condition may be fulfilled, but the underlying subtree for each may NOT be a
    unival tree! so we need to also track not only the number of unival trees in
    the subtree, but whether or not that entire subtree itself is a unival tree.
    """
    # base case: if None, return 0, True
    if root is None: return 0, True
    # if both left and right are None, return 1, True
    if (root.left is None) and (root.right is None): return 1, True
    # get number of unival trees in left and right subtrees rooted by root.left
    # and by root.right respectively, and whether the entire subtrees rooted by
    # root.left and root.right are themselves unival trees
    nl, left_uvt = num_uvt(root.left)
    nr, right_uvt = num_uvt(root.right)
    # set left and right to ease notation
    left = root.left
    right = root.right
    # special, mutually exclusive cases
    # if left is None, right.val == root.val, and right_uvt is True, then the
    # whole subtree rooted by root is a unival tree too, so add 1
    if ((left is None) and (right is not None) and (right.val == root.val) and
        (right_uvt == True)): return nl + nr + 1, True
    # else if right is None, left.val == root.val, and left_uvt is True, then
    # the whole subtree rooted by root is a unival tree too, so add 1
    elif ((right is None) and (left is not None) and (left.val == root.val) and
          (left_uvt == True)): return nl + nr + 1, True
    # else if right.val == left.val == root.val, left_uvt == right_uvt == True,
    # the whole subtree rooted by root is a unival tree too, so add 1
    elif ((left is not None) and (right is not None) and (left.val == root.val)
          and (right.val == root.val) and (left_uvt == True) and
          (right_uvt == True)): return nl + nr + 1, True
    # else the tree rooted at root itself is not a unival tree, so don't add 1
    # and report the tree as not a unival tree
    return nl + nr, False

# main
if __name__ == "__main__":
    # given example (does not require that we have memory of the subtrees)
    # answer is 5
    root = tnode(0, left = tnode(1),
                 right = tnode(0, left = tnode(1,
                                               left = tnode(1),
                                               right = tnode(1)),
                               right = tnode(0)))
    func = num_uvt
    ufunc_eval(func, root)
    # make a simple tree that requires that we have memory of the subtrees
    # answer is 4
    root = tnode(1, left = tnode(2),
                 right = tnode(3, left = tnode(3),
                               right = tnode(3, left = tnode(4),
                                             right = tnode(1))))
    ufunc_eval(func, root)
