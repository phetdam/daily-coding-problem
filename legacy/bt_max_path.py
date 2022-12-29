# daily coding problem #94 (easy, but takes some thought)
#
# Changelog:
#
# 09-08-2019
#
# initial creation. took me a bit to figure out, but eventually broke it down
# into a recursive problem involving for each subtree with root u computing
# the longest path starting from u and the longest overall path in the subtree
# rooted by u. doing this recursively and maintaining these two quantities
# allows one to build up the longest path piece by piece.

__doc__ = """
This problem was asked by Google.

Given a binary tree of integers, find the maximum path sum between two nodes.
The path must go through at least one node, and does not need to go through the
root.
"""

# import evaluation function
from ufunc_eval import ufunc_eval

class bt_node:

    def __init__(self, data, left = None, right = None):
        self.data = data
        self.left = left
        self.right = right

def max_path(root, orig = True):
    """
    returns the maximum path between two nodes in a binary tree with root root.
    """
    # if root is None, return (0, 0) if not original call, else return 0
    if root is None:
        if orig == True: return 0
        return 0, 0
    # for left subtree, get max single path from left subtree root and max
    # circular path within left subtree
    lsp, lcp = max_path(root.left, orig = False)
    # for right subtree, get max single path from right subtree root and max
    # circular path within right subtree
    rsp, rcp = max_path(root.right, orig = False)
    # get max single path from root and max circular path
    mrp, mcp = root.data + max(lsp, rsp), \
        max(root.data + lsp + rsp, lcp, rcp)
    # if original, return max of mrp and mcp
    if orig == True: return max(mrp, mcp)
    return mrp, mcp

# main
if __name__ == "__main__":
    func = max_path
    # example input, answer should be 16
    root = bt_node(1,
                   left = bt_node(-5,
                                  left = bt_node(9),
                                  right = bt_node(4)),
                   right = bt_node(7,
                                   right = bt_node(-2,
                                                   left = bt_node(6),
                                                   right = bt_node(4))))
    ufunc_eval(func, root)
