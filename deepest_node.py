# daily coding problem #80 (easy)
#
# Changelog:
#
# 08-21-2019
#
# initial creation. i was going to attempt a recursive solution, but the breadth
# first search iterative solution tempted me because it was easier.

__doc__ = """
Good morning! Here's your coding interview problem for today.

This problem was asked by Google.

Given the root of a binary tree, return a deepest node. For example, in the
following tree, return d.

    a
   / \
  b   c
 /
d
"""

# import evaluation function
from ufunc_eval import ufunc_eval

# simple binary tree class
class bt_node:

    def __init__(self, data, left = None, right = None):
        self.data = data
        self.left = left
        self.right = right

def get_deepest(root):
    """
    solution using breadth first search to traverse the tree. if there are two
    nodes that have the same depth, the leftmost one will be returned.
    """
    # return None if root is None
    if root is None: return None
    # deepest node and its depth; set as root initially
    deepest, ddepth = root, 0
    # queue; entries are (node, depth). root has depth 0
    queue = [(deepest, ddepth)]
    # while queue is not empty
    while len(queue) > 0:
        # get last node and its depth
        u, ud = queue.pop(0)
        # if its depth is greater than that of the deepest, update deepest
        if ud > ddepth: deepest, ddepth = u, ud
        # else do nothing; add all non-null nodes to queue
        if u.left is not None: queue.append((u.left, ud + 1))
        if u.right is not None: queue.append((u.right, ud + 1))
    # return deepest node
    return deepest

# main
if __name__ == "__main__":
    func = get_deepest
    # problem input, answer is d
    root = bt_node('a',
                   left = bt_node('b',
                                  left = bt_node('d')),
                   right = bt_node('c'))
    deepest = ufunc_eval(func, root)
    print(deepest.data)
    # another tree, answer is -10
    root = bt_node(4,
                   left = bt_node(-4,
                                  left = bt_node(31),
                                  right = bt_node(14,
                                                  left = bt_node(19))),
                   right = bt_node(11,
                                   left = bt_node(-11,
                                                  right = bt_node(-9,
                                                                  left =
                                                                  bt_node(-10))),
                                   right = bt_node(19,
                                                   left = bt_node(14))))
    deepest = ufunc_eval(func, root)
    print(deepest.data)
