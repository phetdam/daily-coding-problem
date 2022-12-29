# daily coding problem #83 (medium, but this should be an easy)
#
# Changelog:
#
# 08-26-2019
#
# initial creation. writing input cases took longer than solving the problem...

__doc__ = """
This problem was asked by Google.

Invert a binary tree. For example, given the following tree:

     a
   /   \
  b     c
 / \   /
d   e f

should become:

    a
  /   \
 c     b
  \   / \
   f e   d
"""

# import evaluation function
from ufunc_eval import ufunc_eval

# simply binary tree node
class bt_node:

    def __init__(self, data, left = None, right = None):
        self.data = data
        self.left = left
        self.right = right

def inorder(root):
    """
    utility function that returns the nodes of a binary tree as a list by
    means of inorder traversal.
    """
    if root is None: return []
    return inorder(root.left) + [root.data] + inorder(root.right)

def invert(root):
    """
    for each node, switches the left and right children and their subtrees
    """
    if root is None: return None
    root.left, root.right = invert(root.right), invert(root.left)
    return root

# main
if __name__ == "__main__":
    func = invert
    # problem input, answer should be [c, f, a, e, b, d]
    root = bt_node("a",
                   left = bt_node("b",
                                  left = bt_node("d"),
                                  right = bt_node("e")),
                   right = bt_node("c",
                                   left = bt_node("f")))
    print(inorder(root))
    root = ufunc_eval(func, root)
    print(inorder(root))
    # a much longer input, answer should be [20, 22, 81, 43, -99, 15, 16, 10, 6,
    # 11, -3, 7, 9, -1, -5]
    # we use subroot to shorten our line lengths
    subroot = bt_node(-99,
                      right = bt_node(81,
                                      left = bt_node(43),
                                      right = bt_node(22)))
    root = bt_node(6,
                   left = bt_node(-1,
                                  left = bt_node(-5),
                                  right = bt_node(7,
                                                  left = bt_node(9),
                                                  right = bt_node(11,
                                                                  left =
                                                                  bt_node(-3)))),
                   right = bt_node(10,
                                   right = bt_node(15,
                                                   left = bt_node(16),
                                                   right = bt_node(20,
                                                                   left =
                                                                   subroot))))
    print(inorder(root))
    root = ufunc_eval(func, root)
    print(inorder(root))
