# daily coding problem #3 (medium, but this is going to take me more time and
# more thought that the first two, since i want to do it efficiently.)
#
# Changelog:
#
# 06-08-2019
#
# corrected some silly typos.
#
# 06-07-2019
#
# updated to include sentence indicating which company asked this question, as
# noted in the changes made to sol_template.py.
#
# 06-06-2019
#
# solved it, but with a not so clean solution (although technically the concept
# was correct). initially confused breadth-first traversal with preorder
# traversal, which made the deserialize function not work. admittedly my
# solution is janky because i wanted to avoid using a wrapper for the recursive
# functions, so i used an optional named keyword argument for both. deserialize
# therefore returns two arguments; if i used a wrapper it would just return one
# (had to return two since strings are immutable objects, and i needed a string
# to be modified in the current scope).
#
# 06-05-2019
#
# initial creation. was initially unable to solve it and eventually looked at
# the geeksforgeeks answer to the problem.

__doc__ = """
This problem was asked by Google.

Given the root to a binary tree, implement serialize(root), which serializes the
tree into a string, and deserialize(s), which deserializes the string back into
the tree. For example, given the following Node class

class Node:
    def __init__(self, val, left=None, right=None):
        self.val = val
        self.left = left
        self.right = right

The following test should pass:

node = Node('root', Node('left', Node('left.left')), Node('right'))
assert deserialize(serialize(node)).left.left.val == 'left.left'
"""

# import evaluation function
from ufunc_eval import ufunc_eval

# sample node class (literally straight from the problem)
class Node:
    def __init__(self, val, left = None, right = None):
        self.val = val
        self.left = left
        self.right = right

# helper: null marker
_none = "_none"
# serialization delimiter
_delim = "|"

def serialize(root, bt_str = None):
    """
    O(n), where n is the number of nodes in the tree. i needed geeksforgeeks
    for help on this one. using preorder traversal, with null markers, we can
    preserve the tree order sequentially. note that values cannot contain the
    character used by _delim (currently {0}), since that is how we are
    distinguishing different values! we are also assuming a string type for the
    tree as well, for simplicity.
    """.format(_delim)
    # if bt_str is None and root is None, return None, ""
    if (bt_str is None) and (root is None): return None, ""
    # get value of root; if None, set to _none and return bt_str with _delim
    # and _none appended to it
    val = None
    if root is None:
        val = _none
        return bt_str + _delim + val
    # else set val to root.val
    else: val = root.val
    # if bt_str is None (root not None), set bt_str to val
    if bt_str is None: bt_str = val
    # else append to bt_str and recur for left and right subtrees
    else: bt_str = bt_str + _delim + val
    # note: we have to return bt_str because python treats strings as immutable
    bt_str = serialize(root.left, bt_str = bt_str)
    bt_str = serialize(root.right, bt_str = bt_str)
    return bt_str

def deserialize(bt_str, root = None):
    """
    O(n), where n is the number of nodes in the tree (including nulls). again i
    needed geeksforgeeks; realized that this was correct and only seemed wrong
    because breadth-first search is not the same as preorder traversal. it is
    depth-first search that is; i got them mixed up.
    """
    # if bt_str is empty, return None and empty string
    if bt_str == "": return None, ""
    # else convert bt_str to a list of strings, pop the first element, and
    # rejoin by _delim into a smaller string.
    bt_str = bt_str.split(_delim)
    val = bt_str.pop(0)
    bt_str = _delim.join(bt_str)
    # if val is _none, return None
    if val == _none: return None, bt_str
    # else set root to new node with value val
    root = Node(val)
    # run recursively to get left and right subtrees
    root.left, bt_str = deserialize(bt_str)
    root.right, bt_str = deserialize(bt_str)
    # return tree and string; again we have to return since string is immutable
    return root, bt_str

# main
if __name__ == "__main__":
    # simple tree from input example; arg names added for clarity
    node = Node("root", left = Node("left", left = Node("left.left")),
                right = Node("right"))
    # showing that serialization and deserialization are lossless
    bt_str = ufunc_eval(serialize, node)
    node, _ = ufunc_eval(deserialize, bt_str)
    ufunc_eval(serialize, node)
    # passes assert test (we replace with boolean here)
    print("node.left.left.val == 'left.left' is {0}".format(
        node.left.left.val == "left.left"))
