# daily coding problem #50 (easy)
#
# Changelog:
#
# 07-22-2019
#
# initial creation. since the expression is given to you, no need to try and
# deconstruct an infix expression yourself. just do inorder traversal. i have
# really not been keeping up with these problems.

__doc__ = """
This problem was asked by Microsoft.

Suppose an arithmetic expression is given as a binary tree. Each leaf is an
integer and each internal node is one of '+', '-', '*', or '/'. Given the root
to such a tree, write a function to evaluate it.

For example, given the following tree:

     *
   /   \
  +     +
 / \   / \
3   2 4   5

You should return 45, as it is (3 + 2) * (4 + 5).
"""

# import evaluation function
from ufunc_eval import ufunc_eval
# inf
from math import inf

# simple tree clas; for convenience, we can specify left and right children
class bt_node:

    def __init__(self, data, left = None, right = None):
        self.data = data
        self.left = left
        self.right = right

def bst_arith(root):
    """
    use inorder traversal to evaluate the elements at the leaves using the
    operators in the non-leaf nodes. only supports the 4 arithmetic operations.
    the design of this problem is that all parent nodes will have two children.
    any null trees evaluate to -inf.
    """
    # if the root is null, return -inf
    if root is None: return -inf
    # if both children are null and the root data is a number, return the data
    if (root.left is None) and (root.right is None):
        if (isinstance(root.data, int) == False) and \
           (isinstance(root.data, float) == False):
            raise TypeError("leaves must be either int or float values")
        return root.data
    # get left and right tree values
    lval = bst_arith(root.left)
    rval = bst_arith(root.right)
    # test for each of the arithmetic operators and perform appropriate function
    if root.data == "+": return lval + rval
    elif root.data == "-": return lval - rval
    elif root.data == "*": return lval * rval
    elif root.data == "/": return lval / rval
    # else just return -inf
    return -inf

# main
if __name__ == "__main__":
    func = bst_arith
    # use problem input, answer should be 45
    root = bt_node("*",
                   left = bt_node("+", left = bt_node(3), right = bt_node(2)),
                   right = bt_node("+", left = bt_node(4), right = bt_node(5)))
    ufunc_eval(func, root)
    # use the expression 7 * (9 + 6) / 5, evaluates to 21
    root = bt_node("*",
                   left = bt_node(7),
                   right = bt_node("/",
                                   left = bt_node("+",
                                                  left = bt_node(9),
                                                  right = bt_node(6)),
                                   right = bt_node(5)))
    ufunc_eval(func, root)
