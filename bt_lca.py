# daily coding problem #112 (hard, but pretty straightforward)
#
# Changelog:
#
# 09-22-2019
#
# initial creation. had some difficulties initially due to an error in ar2bst,
# but after that was fixed, everything went well. straightfoward tree problem
# that i don't think should be considered a hard problem.

__doc__ = """
This problem was asked by Twitter.

Given a binary tree, find the lowest common ancestor (LCA) of two given nodes in
the tree. Assume that each node in the tree also has a pointer to its parent.

According to the definition of LCA on Wikipedia: "The lowest common ancestor is
defined between two nodes v and w as the lowest node in T that has both v and w
as descendants (where we allow a node to be a descendant of itself)."
"""

# import evaluation function
from ufunc_eval import ufunc_eval

# binary tree with parent node. we make bst methods to make the insertion and
# search process easy for the sake of problem exposition.
class bt_node:

    def __init__(self, data, parent = None):
        self.data = data
        self.left = None
        self.right = None
        self.parent = parent

def ar2bst(ar):
    """
    creates a binary search tree, with each node also having a pointer to its
    parent. the first node of the tree is assumed to be the root. unique
    values only, for a non-None root to be returned.
    """
    assert ar is not None
    # get length of ar
    n = len(ar)
    if n == 0: return None
    # choose first node as root
    root = bt_node(ar[0])
    # for the rest of the nodes, insert into the tree
    for i in range(1, n):
        # start at root
        cur = root
        # while we have not reached leaf node
        while True:
            # if ar[i] < cur.data
            if ar[i] < cur.data:
                # if left is None, insert
                if cur.left is None:
                    cur.left = bt_node(ar[i], parent = cur)
                    break
                # else go left
                cur = cur.left
            # duplicate warning
            elif ar[i] == cur.data: raise ValueError("no duplicates allowed")
            # if ar[i] > cur.data
            elif ar[i] > cur.data:
                # if right is None, insert
                if cur.right is None:
                    cur.right = bt_node(ar[i], parent = cur)
                    break
                # else go right
                cur = cur.right
    # return root
    return root

def find(root, k):
    """
    return a reference to the node with value k, else return None
    """
    if root is None: return None
    cur = root
    # keep searching until we eat a None node
    while cur is not None:
        # standard binary search tree methods
        if k < cur.data: cur = cur.left
        elif k == cur.data: break
        elif k > cur.data: cur = cur.right
    # return cur
    return cur

def bt_lca(root, u, v):
    """
    returns least common ancestor of nodes u and v. if they are identical, u
    will be returned. if one is None, None will be returned.
    """
    if (root is None) or (u is None) or (v is None): return None
    if u == v: return u
    # reverse ancestor paths and u and v
    u_ap, v_ap = [], []
    # get reverse ancestor paths for u and v. note that a node is allowed to be
    # a descendant of itself.
    cur = u
    while cur is not None:
        u_ap.append(cur)
        cur = cur.parent
    cur = v
    while cur is not None:
        v_ap.append(cur)
        cur = cur.parent
    # get min length of the two paths
    mpl = min(len(u_ap), len(v_ap))
    # going backwards, stop after the first uncommon node
    i = -1
    while i > (-1 - mpl):
        # if there is a difference, break
        if u_ap[i] != v_ap[i]: break
        # else decrement i
        i = i - 1
    # if i == -1 - mpl, we have no common ancestor, so return None
    if i == (-1 - mpl): return None
    # else return the i + 1 index (last commonly shared
    return u_ap[i + 1]

# main
if __name__ == "__main__":
    func = bt_lca
    # binary search tree (for simplicity of exposition)
    root = ar2bst([5, 3, 4, 2, 7, 9])
    # array of all the answers
    lca_ar = []
    # input 1, answer should be 5 (root)
    u, v = find(root, 3), find(root, 9)
    lca = ufunc_eval(func, root, u, v)
    lca_ar.append(lca if lca is None else lca.data)
    # input 2, answer should be 7
    u, v = find(root, 7), find(root, 9)
    lca = ufunc_eval(func, root, u, v)
    lca_ar.append(lca if lca is None else lca.data)
    # input 3, answer should be 3
    u, v = find(root, 2), find(root, 4)
    lca = ufunc_eval(func, root, u, v)
    lca_ar.append(lca if lca is None else lca.data)
    print(lca_ar)
