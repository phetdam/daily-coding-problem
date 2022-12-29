# daily coding problem #26 (medium, straightforward once you get the trick)
#
# Changelog:
#
# 06-28-2019
#
# initial creation. very straightforward problem.

__doc__ = """
This problem was asked by Google.

Given a singly linked list and an integer k, remove the kth last element from
the list. k is guaranteed to be smaller than the length of the list. The list is
very long, so making more than one pass is prohibitively expensive. Do this in
constant space and in one pass.
"""

# import evaluation function
from ufunc_eval import ufunc_eval

# simple linked list class
class node:
    def __init__(self, data, next = None):
        self.data = data
        self.next = next

    def to_string(self):
        """
        returns string with this node and all other nodes until None is reached
        """
        cur = self
        out_str = ""
        while cur is not None:
            out_str = out_str + str(cur.data) + " -> "
            cur = cur.next
        return out_str + "None"

def kthlast(root, k):
    """
    O(n) removal of kth last node in a linked list. advance one pointer k places
    and when that pointer is null remove the back pointer.
    """
    # sanity checks
    if (root is None) or (k < 1): return None
    # front and back nodes start at root
    front = back = root
    # move front k places
    while (front is not None) and (k > 0):
        front = front.next
        k = k - 1
    # if front is None, then k > length of list, so return None
    if front is None: return None
    # else continue traversing until front is None and return back
    while front is not None:
        front = front.next
        back = back.next
    return back

# main
if __name__ == "__main__":
    func = kthlast
    # make some linked list (how convenient!)
    root = node(13, next =
                node(4, next =
                     node(9, next =
                          node(-1, next =
                               node(-91, next =
                                    node(19, next =
                                         node(21)))))))
    # valid k, returns node with -1
    k = 4
    print("{0} ~> {1}".format(root.to_string(), ufunc_eval(func, root, k).data))
    # invalid k, returns None
    k = 10
    print("{0} ~> {1}".format(root.to_string(), ufunc_eval(func, root, k)))
