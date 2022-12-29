# daily coding problem #133 (medium, but pretty straightforward)
#
# Changelog:
#
# 10-13-2019
#
# initial creation. seemed pretty straightforward, but there were some edge
# cases that took me a little bit to find. for example, if the node you select
# is the largest in the tree (return None), if the node is the root but the tree
# consists of only the root, or if the inorder successor needs traversal to be
# reached. this problem, however, is pretty straightforward.

__doc__ = """
This problem was asked by Amazon.

Given a node in a binary search tree, return the next bigger element, also known
as the inorder successor. For example, the inorder successor of 22 is 30.

    10
   /  \
  5   30
     /  \
    22  35

You can assume each node has a parent pointer.
"""

# import evaluation function
from ufunc_eval import ufunc_eval

# binary tree node with a parent pointer
class bt_node:

    def __init__(self, data, left = None, right = None, parent = None):
        self.data = data
        self.left = left
        self.right = right
        self.parent = parent

def get_next_inorder(root, k):
    """
    performs O(h) search, where h is height of the tree, to find the node with
    value k. if k has no right child, then its parent is returned, else if k
    does have a right child, then its right child is returned. this is not
    exactly the same as the problem statement, which says "given a node", but it
    is easier to perform the implementation this way. either way, the actually
    returning of the inorder successor is O(1) as it should be.
    """
    if root is None: return None
    # look for value k
    cur = root
    while cur is not None:
        # left
        if k < cur.data: cur = cur.left
        # found
        elif k == cur.data: break
        # right
        else: cur = cur.right
    # if node containing k is not found, return None
    if cur is None: return None
    # else it has been found. if the right child is None, then check the parent.
    # if the parent is None (root), return parent, else return parent only if
    # parent is greater than current node. also return None if parent < cur.
    if cur.right is None:
        # if parent is None or parent has smaller value, return None
        if (cur.parent is None) or (cur.parent.data < cur.data): return None
        return cur.parent
    # else we need to search the right subtree for the leftmost node.
    cur = cur.right
    while cur.left is not None: cur = cur.left
    return cur

def make_bst(ar):
    """
    creates a binary search tree from the elements in the array ar. note that as
    usual, the order of elements in ar matters. no duplicates.
    """
    # cases for None root
    if (ar is None) or (len(ar) == 0): return None
    # length of ar
    n = len(ar)
    # create root of the tree
    root = bt_node(ar[0])
    # for each subsequent val, insert as binary search tree
    for i in range(1, n):
        # make new node
        new_node = bt_node(ar[i])
        # insert into binary tree
        cur = root
        while True:
            # ar[i] < cur.data
            if ar[i] < cur.data:
                # if cur.left is None, set parent of new_node and insert
                if cur.left is None:
                    new_node.parent = cur
                    cur.left = new_node
                    break
                # else just go left
                cur = cur.left
            # ar[i] > cur.data
            else:
                # if cur.right is None, set parent of new_node and insert
                if cur.right is None:
                    new_node.parent = cur
                    cur.right = new_node
                    break
                # else just go right
                cur = cur.right
    # return root
    return root

# main
if __name__ == "__main__":
    func = get_next_inorder
    # problem input, answer is 30
    root = make_bst([10, 5, 30, 22, 35])
    k = 22
    nin = ufunc_eval(func, root, k)
    print(nin.data if nin is not None else "None")
    # same tree different k, answer is 22
    k = 10
    nin = ufunc_eval(func, root, k)
    print(nin.data if nin is not None else "None")
    # another input, answer is 13
    root = make_bst([8, 10, 9, 7, 4, 13, 12, 20])
    k = 10
    nin = ufunc_eval(func, root, k)
    print(nin.data if nin is not None else "None")
    # same tree different k, answer is None
    k = 20
    nin = ufunc_eval(func, root, k)
    print(nin.data if nin is not None else "None")
