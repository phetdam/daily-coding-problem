# daily coding problem #204 (easy, but quite tricky if you do it this way)
#
# Changelog:
#
# 12-25-2019
#
# finalized the solution. logic was sound for the most part except for the fact
# that i forgot to count the leftmost leaf of the tree and the rightmost leaf;
# after appropriately accounting for them, the solution was completed.
#
# 12-24-2019
#
# initial creation. finished first draft of the solution, but got sidetracked
# and ended up adding the function make_bt to ./simple_ds/binary_tree.py since
# to test my solution, i needed to generate an arbitrarily sized binary tree.
# honestly the easiest solution would just be to make a binary tree that updates
# a size property every time a node is inserted, but this is the solution with
# a "vanilla" binary tree that does not have any other extra properties.

__doc__ = """
This problem was asked by Amazon.

Given a complete binary tree, count the number of nodes in faster than O(n)
time. Recall that a complete binary tree has every level filled except the last,
and the nodes in the last level are filled starting from the left.
"""

# import evaluation function
from ufunc_eval import ufunc_eval
# import binary tree class, tree to array function, and function to make tree
from simple_ds.binary_tree import bt_node, bt2ar, make_bt

def count_complete(root):
    """
    sub-O(n) solution for counting the number of nodes in a complete binary tree
    by exploiting the fact that for a complete binary tree of depth k, there are
    at least 2^k - 1 nodes in the tree. we just need to determine the last level
    of nodes, which we do so by memorizing the leftmost and rightmost outer
    nodes of the tree (if we went straight down the left branch and right
    branch, respectively). for each ith leftmost/rightmost branch we check the
    right/left child (respectively), and since there are 2^(k - i - 1) - 1
    possible nodes in a subtree formed from this child that reaches to the k - 1
    level, we subtract 2^(k - i - 1) from the number of actual nodes to get the
    extra number of nodes to add to 2^k - 1. to count the number of nodes in the
    subtrees sub-O(n), we call this function recursively.
    """
    # base cases
    if root is None: return 0
    # list for the leftmost nodes, rightmost nodes
    lnodes, rnodes = [], []
    # go down all the leftmost nodes and collect them
    cur = root.left
    while cur is not None:
        lnodes.append(cur)
        cur = cur.left
    # go down all the rightmost nodes and collect them
    cur = root.right
    while cur is not None:
        rnodes.append(cur)
        cur = cur.right
    # if len(lnodes) = len(rnodes) =: k, then there are 2^(k + 1) - 1 nodes in
    # the tree, and we can simply just return this number.
    nl = len(lnodes)
    nr = len(rnodes)
    if nl == nr: return (2 ** (nl + 1)) - 1
    # else, len(rnodes) = len(lnodes) - 1. start counting the subtrees using the
    # lnodes nodes' right child. for each ith lnode (1-indexed), the subtree
    # formed by the right child of the ith lnode to the (k - 1)th level will
    # have 2^(k - i - 1) - 1 nodes, so we subtract this from the number of
    # actual nodes to get the number of nodes in the kth level to add to our
    # original sum 2^(nl) - 1, the total nodes before the kth level.
    # number of nodes on the kth level
    nkn = 0
    # note we go "backwards", starting from the deepest lnode (excluding the
    # lnode on level k, i.e. the (nl - 1)th element in lnodes)
    for i in range(nl - 2, -1, -1):
        # note: need to add 1 to the i to correct the zero indexing, so what we
        # get is that 2^(k - i - 1) - 1 = 2^(nl - i - 1) - 1 turns into
        # 2^(nl - i - 2) - 1
        xn = count_complete(lnodes[i].right) - 2 ** (nl - i - 2) + 1
        # if xn is 0, then there are no nodes on the kth level for any other
        # right child of an lnode except for the leftmost lnode, so return nkn +
        # 2^(nl), as we need to add that to 2^(nl) - 1
        if xn == 0: return nkn + (2 ** nl)
        # else add xn to nkn
        else: nkn = nkn + xn
    # if there were nodes on the kth level for every lnode, look at the rnodes,
    # again ignoring the last one on the k - 1 level, i.e. the (nr - 1)th value
    # which is also the (nl - 2)th element in rnodes
    for i in range(nr):
        xn = count_complete(rnodes[i].left) - 2 ** (nl - i - 2) + 1
        # if xn is 0, then there are no nodes on the kth level for any other
        # left child of an rnode, so break the loop
        if xn == 0: break
        # else add xn to nkn
        else: nkn = nkn + xn
    # return nkn + (2 ** nl), since we need to add the one leftmost node
    return nkn + (2 ** nl)

# main
if __name__ == "__main__":
    func = count_complete
    # input, answer is 25
    root = make_bt(25)
    ufunc_eval(func, root)
    # another input, answer is 15 (full tree)
    root = make_bt(15)
    ufunc_eval(func, root)
    # another input, answer is 43425
    root = make_bt(43425)
    ufunc_eval(func, root)
