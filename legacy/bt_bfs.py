# daily coding problem #107 (easy)
#
# Changelog:
#
# 09-17-2019
#
# initial creation. straightforward application of breadth-first traversal.

__doc__ = """
This problem was asked by Microsoft.

Print the nodes in a binary tree level-wise. For example, the following should
print 1, 2, 3, 4, 5.

   1
  / \
 2   3
    / \
   4   5
"""

# import evaluation function
from ufunc_eval import ufunc_eval

# simple binary tree
class bt_node:

    def __init__(self, data, left = None, right = None):
        self.data = data
        self.left = left
        self.right = right

def bt2ar_bfs(root):
    """
    returns an array from a binary tree rooted at root, but in level order. this
    is accomplished by using breadth-first traversal.
    """
    if root is None: return []
    # output list
    outl = []
    # do breadth-first traversal
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
    func = bt2ar_bfs
    # problem input, answer is [1, 2, 3, 4, 5]
    root = bt_node(1,
                   left = bt_node(2),
                   right = bt_node(3,
                                   left = bt_node(4),
                                   right = bt_node(5)))
    ufunc_eval(func, root)
    # another input, answer is [3, 7, 2, 5, 9, 6, 10, 12]
    root = bt_node(3,
                   left = bt_node(7),
                   right = bt_node(2,
                                   left = bt_node(5,
                                                  left = bt_node(6)),
                                   right = bt_node(9,
                                                   left = bt_node(10),
                                                   right = bt_node(12))))
    ufunc_eval(func, root)
