# daily coding problem #73 (easy)
#
# Changelog:
#
# 08-14-2019
#
# initial creation. an oldie but goodie.

__doc__ = """
This problem was asked by Google.

Given the head of a singly linked list, reverse it in-place.
"""

# import evaluation function
from ufunc_eval import ufunc_eval

class node:

    def __init__(self, data, next_node = None):
        self.data = data
        self.next = next_node

def reverse(head):
    """
    returns the head of a now reversed linked list.
    """
    # conditions for just returning the head
    if (head is None) or (head.next is None): return head
    prev = None
    cur = head
    while cur is not None:
        old_prev = prev
        prev = cur
        cur = cur.next
        prev.next = old_prev
    return prev

def traverse(head):
    """
    returns the singly linked list as a string
    """
    if head is None: return "None"
    outs = ""
    cur = head
    while cur is not None:
        outs = outs + str(cur.data) + " -> "
        cur = cur.next
    return outs

# main
if __name__ == "__main__":
    # random input
    head = node(2, next_node =
                node(6, next_node =
                     node(-1, next_node =
                          node(90, next_node =
                               node(-9, next_node =
                                    node(11, next_node = node(-9.8)))))))
    old_list = traverse(head)
    head = ufunc_eval(reverse, head)
    new_list = traverse(head)
    print("{0}\n{1}".format(old_list, new_list))
