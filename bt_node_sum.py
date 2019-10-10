# daily coding problem #125 (easy, but not the easiest?)
#
# Changelog:
#
# 10-10-2019
#
# initial creation. created two solutions: one requires no extra memory (except
# for a queue for breadth-first search), but is O(nlogn), and consists of binary
# search for each node looked at by the traversal. the other solution is O(n),
# by means of hash table, but requires extra O(n) space for the table.

__doc__ = """
This problem was asked by Google.

Given the root of a binary search tree, and a target K, return two nodes in the
tree whose sum equals K. For example, given the following tree and K of 20

   10
  /  \
 5    15
     /  \
    11  15

Return the nodes 5 and 15.
"""

# import evaluation function
from ufunc_eval import ufunc_eval

class bt_node:

    def __init__(self, data, left = None, right = None):
        self.data = data
        self.left = left
        self.right = right

def two_sum(root, k):
    """
    performs a breadth first search to choose the first node, and then does a
    modified binary search to find the next node that can sum to target k. if
    none of the combinations of two nodes work, then (None, None) is returned.
    note: as assumed in the problem, duplicate nodes are allowed, and they will
    always be stored in consecutive right subtrees. no extra memory, but it is
    O(nlogn), where n is the number of nodes in the tree.
    """
    if root is None: return (None, None)
    # start with the root on the queue
    queue = [root]
    # breadth first traversal for the first node
    while len(queue) > 0:
        u = queue.pop(0)
        # perform iterative binary search for the node of value k - u.data
        # target value to find that has value k - u.data
        j = k - u.data
        # binary search from root
        cur = root
        # while we don't hit None
        while cur is not None:
            # if j < cur.data, go left
            if j < cur.data: cur = cur.left
            # else if j equals cur.data
            elif j == cur.data:
                # if cur == u, these are the same node, are duplicates will be
                # in the right subtree, so go right
                if cur == u: cur = cur.right
                # else just break
                else: break
            # else j > cur.data, so go right
            else: cur = cur.right
        # if cur is not None, return straightaway
        if cur is not None: return (u, cur)
        # else cur is None, so continue the breadth first search
        if u.left is not None: queue.append(u.left)
        if u.right is not None: queue.append(u.right)
    # if all fails, looks like we're returning (None, None) again
    return (None, None)

def two_sum_fast(root, k):
    """
    O(n) solution, using O(n) extra memory. breadth-first traversal to insert
    all the nodes into a hash table; format val: [nodes] for each entry.
    """
    if root is None: return (None, None)
    queue = [root]
    # lookup hash table
    look = {}
    # add nodes into look
    while len(queue) > 0:
        u = queue.pop(0)
        # if not in look, add new key-value pair (value, node pointer)
        if u.data not in look: look[u.data] = [u]
        # else add new node to same value list
        else: look[u.data].append(u)
        # append children is not None
        if u.left is not None: queue.append(u.left)
        if u.right is not None: queue.append(u.right)
    # look through look to try to find the two nodes
    for val, nodes in look.items():
        # if k - val in look
        if (k - val) in look:
            # if k - val == val, if there are two nodes in the list, then we can
            # return the two nodes with the same value
            if (k - val == val) and (len(look[val]) > 1):
                return (look[val][0], look[val][1])
            # else if k - val != val, return first item from each entry
            elif (k - val != val):
                return (look[val][0], look[k - val][0])
    # else return (None, None) if their are no suitable pairs
    return (None, None)

def bt_list_str(ar):
    """
    utility function that returns string values of the nodes; if any are None,
    None will be the value of the node. takes a tuple/list of nodes.
    """
    assert ar is not None
    # get length of ar
    n = len(ar)
    # if length is zero, just print the empty list
    if n == 0: return str(ar)
    # else create output string and concatenate values to it. starting brackets
    # are based on the type that the list-type ar is
    outs = "(" if isinstance(ar, tuple) else "["
    # for each element, append to outs
    for i in range(n):
        # if None, add None as a value
        if ar[i] is None: outs = outs + "None"
        # else just add the value
        else: outs = outs + str(ar[i].data)
        # if the index is not the last index, at comma and space
        if i < n - 1: outs = outs + ", "
    # add closing bracket based on instance type
    outs = outs + (")" if isinstance(ar, tuple) else "]")
    # return outs
    return outs

# main
if __name__ == "__main__":
    func_a = two_sum
    func_b = two_sum_fast
    # problem input, answer is 5, 15
    root = bt_node(10,
                   left = bt_node(5),
                   right = bt_node(15,
                                   left = bt_node(11),
                                   right = bt_node(15)))
    k = 20
    print(bt_list_str(ufunc_eval(func_a, root, k)))
    print(bt_list_str(ufunc_eval(func_b, root, k)))
    # another input, answer is 15, 15
    root = bt_node(17,
                   left = bt_node(9,
                                  left = bt_node(8),
                                  right = bt_node(15,
                                                  right = bt_node(15))),
                   right = bt_node(20,
                                   right = bt_node(24)))
    k = 30
    print(bt_list_str(ufunc_eval(func_a, root, k)))
    print(bt_list_str(ufunc_eval(func_b, root, k)))
