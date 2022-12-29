# daily coding problem #36 (medium, this is actually quite easy)
#
# Changelog:
#
# 07-08-20199
#
# initial creation. at first glance i knew it was a case of the "find the kth
# greatest/least element in a binary search tree" problem, so kth_largest was
# very easy to write. the part that took longer was actually writing the helper
# functions to allow me to test my solution. the solution tester builds a tree
# of random size with random values, with behavior parameterized by two degrees
# of freedom a and b, and choose a k where 1 <= k <= b, with tree size n where
# a <= n <= b, and 1 <= a < b.

__doc__ = """
This problem was asked by Dropbox.

Given the root to a binary search tree, find the second largest node in the tree.
"""

# import evaluation function
from ufunc_eval import ufunc_eval
# import random int function from random
from random import randint

# simple tree class, no extra members involved
class bst_node:
    def __init__(self, data):
        self.data = data
        self.left = self.right = None

def bt_size(root):
    """
    recursive function to get size of binary tree. could use an iterative method
    like breadth-first search instead, but this is just shorter.
    """
    if root is None: return 0
    return bt_size(root.left) + 1 + bt_size(root.right)

def kth_largest(root, k):
    """
    since the problem is asking me to find the 2nd largest node in the tree, we
    can easily see that this a case of the kth-largest-node problem. we just
    need to return the node whose right subtree has size k - 1, and we are done.
    """
    # sanity checks
    if (root is None) or (k < 1): return None
    # get size of right subtree
    rt_siz = bt_size(root.right)
    # if rt_siz equals k - 1, return root
    if rt_siz == k - 1: return root
    # else if rt_siz is greater than k - 1, recurse into right subtree
    elif rt_siz > k - 1: return kth_largest(root.right, k)
    # else rt_siz < k - 1, so we now recurse into left subtree, subtracting
    # rt_siz + 1 from k
    return kth_largest(root.left, k - rt_siz - 1)

### helper functions for testing ###

def bst_insert(root, k):
    """
    inserts nodes into a binary search tree. helper for tree building. for the
    sake of simplicity, will not insert duplicates. returns root node and status
    where status is True if k is inserted, False if not.
    """
    if root is None: return bst_node(k), True
    # current node to inspect
    cur = root
    # return status
    status = False
    # while True
    while True:
        # try to insert left; if successful, create node and break
        if k < cur.data:
            if cur.left is None:
                cur.left = bst_node(k)
                status = True
                break
            cur = cur.left
        # try to insert right; if successful, create node and break
        elif k > cur.data:
            if cur.right is None:
                cur.right = bst_node(k)
                status = True
                break
            cur = cur.right
        # case of duplicate insert
        elif k == cur.data: break
    # return
    return root, status

def bst_inorder(root, vals = None, orig = True):
    """
    recursively append values to list; returns list (only if calling function).
    do not change named arguments from defaults if you are calling function.
    """
    # if the function is the calling function and root is None, return empty,
    # else just return None since there is nothing meaningful to insert
    if root is None:
        # redundant test
        if (vals is None) or (orig == True): return []
        return None
    # else root is not None; if vals is None, initialize it
    if vals is None: vals = []
    # inorder append
    bst_inorder(root.left, vals = vals, orig = False)
    vals.append(root.data)
    bst_inorder(root.right, vals = vals, orig = False)
    # return vals only if calling function; i.e. orig is True; else return None
    if orig == True: return vals
    return None

# main
if __name__ == "__main__":
    # build a random tree; a and b are parameters where 1 <= a < b
    a = 5
    b = 15
    root = None
    status = False
    # add a <= n <= b nodes to the empty tree
    for i in range(randint(a, b)):
        # if we have a duplicate, try again
        while status == False:
            root, status = bst_insert(root, randint(-a * a, b * a))
        status = False
    # print inorder tree
    ufunc_eval(bst_inorder, root)
    # print kth largest, where 1 <= k <= b
    k = randint(1, b)
    kl = ufunc_eval(kth_largest, root, k)
    # print kl.data is kl is not None; may be none if k > n
    if kl is not None: print(kl.data)
