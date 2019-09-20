# daily coding problem #110 (medium)
#
# Changelog:
#
# 09-20-2019
#
# initial creation. pretty straightforward application of a modified depth first
# search. got a little stuck regarding how to pop correctly. can make this more
# efficient by simply adding a "visited" field to each node.

__doc__ = """
This problem was asked by Facebook.

Given a binary tree, return all paths from the root to leaves.

For example, given the tree:

   1
  / \
 2   3
    / \
   4   5

Return [[1, 2], [1, 3, 4], [1, 3, 5]].
"""

# import evaluation function
from ufunc_eval import ufunc_eval

# idk how many times i keep remaking this class...
class bt_node:

    def __init__(self, data, left = None, right = None):
        self.data = data
        self.left = left
        self.right = right

def leaf_paths(root):
    """
    use depth-first search to find all paths that end at a leaf, and then add
    each path to a list to be returned from the function.
    """
    if root is None: return None
    # list of paths to leaves
    paths = []
    # create stack for paths
    stk = [root]
    # collection of visited nodes
    visited = []
    # depth first search for each path to a leaf node
    while len(stk) > 0:
        u = stk[-1]
        # append to visited if not in visited
        if u not in visited: visited.append(u)
        # if the left node is not None and not visited, push on stack
        if (u.left is not None) and (u.left not in visited):
            stk.append(u.left)
        # else if the right node is not None and not visited, push on stack
        elif (u.right is not None) and (u.right not in visited):
            stk.append(u.right)
        # else we cannot proceed further
        else:
            # if u is a leaf node, then copy all values in stk to a list then
            # append this list to path
            if (u.left is None) and (u.right is None):
                lp = [e.data for e in stk]
                paths.append(lp)
            # then just pop from the stack
            stk.pop()
    # at the conclusion of the depth first search, return all paths
    return paths

# main
if __name__ == "__main__":
    func = leaf_paths
    # problem input, answer is [[1, 2], [1, 3, 4], [1, 3, 5]]
    root = bt_node(1,
                   left = bt_node(2),
                   right = bt_node(3,
                                   left = bt_node(4),
                                   right = bt_node(5)))
    ufunc_eval(func, root)
    # another input, answer is [[6, 7, 11], [6, 7, 9], [6, 8, 10, 12]]
    root = bt_node(6,
                   left = bt_node(7,
                                  left = bt_node(11),
                                  right = bt_node(9)),
                   right = bt_node(8,
                                   right = bt_node(10,
                                                   left = bt_node(12))))
    ufunc_eval(func, root)
