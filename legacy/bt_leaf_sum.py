# daily coding problem #135 (easy, just a graph traversal)
#
# Changelog:
#
# 10-16-2019
#
# initial creation. a breadth-first traversal and recording of path values
# during traversal is sufficient to give us the answer.

__doc__ = """
This question was asked by Apple.

Given a binary tree, find a minimum path sum from root to a leaf.

For example, the minimum path in this tree is [10, 5, 1, -1], which has sum 15.

   10
  /  \
 5    5
  \    \
   2    1
       /
     -1
"""

# import evaluation function
from ufunc_eval import ufunc_eval
from math import inf

# standard binary tree node class
class bt_node:

    def __init__(self, data, left = None, right = None):
        self.data = data
        self.left = left
        self.right = right

def min_path_sum(root):
    """
    perform a breadth first search, and define (node, cur_path_sum) pairs in the
    queue to remember the path length. for each leaf node (both left and right
    nodes are None), compare min path length.
    """
    # min path length is 0
    if root is None: return 0
    # queue; format is (next node, path sum without this node's data)
    queue = [(root, 0)]
    # minimum path length
    mpl = inf
    # while queue is not empty
    while len(queue) > 0:
        # get node and current path value without the node's value
        u, psum = queue.pop(0)
        # if u is a leaf, then compare psum + u.data to mpl
        if (u.left is None) and (u.right is None):
            mpl = min(mpl, psum + u.data)
        # else add next level of nodes
        if u.left is not None: queue.append((u.left, psum + u.data))
        if u.right is not None: queue.append((u.right, psum + u.data))
    # return mpl
    return mpl

# main
if __name__ == "__main__":
    func = min_path_sum
    # problem input, answer is 15
    root = bt_node(10,
                   left = bt_node(5,
                                  right = bt_node(2)),
                   right = bt_node(5,
                                   right = bt_node(1,
                                                   left = bt_node(-1))))
    ufunc_eval(func, root)
    # another input, answer is -2
    root = bt_node(6,
                   left = bt_node(-8,
                                  left = bt_node(2),
                                  right = bt_node(4,
                                                  left = bt_node(-1))),
                   right = bt_node(10,
                                   left = bt_node(2),
                                   right = bt_node(-20,
                                                   left = bt_node(2))))
    ufunc_eval(func, root)
