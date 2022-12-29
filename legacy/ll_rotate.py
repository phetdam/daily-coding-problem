# daily coding problem #177 (easy)
#
# Changelog:
#
# 12-23-2019
#
# initial creation. not a difficult problem, but i was stuck for a little bit
# since i forgot to increment a loop counter. this problem also inspired the
# creation of the simple_ds package, as being another linked list problem,
# something in me snapped and suddenly i just had enough of always redefining
# simple linked list and binary tree classes, so i wrote simple_ds.

__doc__ = """
This problem was asked by Airbnb.

Given a linked list and a positive integer k, rotate the list to the right by k
places. For example, given the linked list 7 -> 7 -> 3 -> 5 and k = 2, it should
become 3 -> 5 -> 7 -> 7. Given the linked list 1 -> 2 -> 3 -> 4 -> 5 and k = 3,
it should become 3 -> 4 -> 5 -> 1 -> 2.
"""

# import evaluation function
from ufunc_eval import ufunc_eval
# use ll_node and traverse function from simple_ds.linked_list
from simple_ds.linked_list import ll_node, traverse

def ll_right_rotate(head, k):
    """
    O(n) solution for rotating a linked list k places to the right. note that if
    we had a more sophisticated implementation of the linked list, for example a
    list that maintained a length property, we can save the initial iteration
    through the entire list to get the length.
    """
    assert k >= 0
    # trivial return
    if head is None: return head
    # length of the linked list, counter
    n, c = 0, 0
    # new linked list head, tail of the old list, tail of the new list
    head_new, tail_old, tail_new = None, None, None
    # get the length of the linked list
    cur = head
    while cur is not None:
        n = n + 1
        # if cur.next is None, then cur is the old tail; record that
        if cur.next is None: tail_old = cur
        cur = cur.next
    cur = head
    # if k >= n, subtract n from it
    if k > n - 1: k = k - n
    # move n - k places from head to get the "new" head and the new tail
    while c < n - k:
        tail_new = cur
        cur = cur.next
        c = c + 1
    head_new = cur
    # connect old head to old tail, set new tail next to None, return new head
    tail_old.next = head
    tail_new.next = None
    return head_new

# main
if __name__ == "__main__":
    func = ll_right_rotate
    # problem input, answer is "3 ~> 5 ~> 7 ~> 7 ~>"
    ll = ll_node(7,
                 next_node = ll_node(7,
                                     next_node = ll_node(3,
                                                         next_node = ll_node(5))
                 ))
    k = 2
    print("original: {0}".format(traverse(ll)))
    ll = ufunc_eval(func, ll, k)
    print("rotated:  {0}".format(traverse(ll)))
    # another problem input, answer is "3 ~> 4 ~> 5 ~> 1 ~> 2 ~>"
    ll = ll_node(1,
                 next_node = ll_node(2,
                                     next_node =
                                     ll_node(3, next_node =
                                             ll_node(4, next_node = ll_node(5)))
                 ))
    k = 3
    print("original: {0}".format(traverse(ll)))
    ll = ufunc_eval(func, ll, k)
    print("rotated:  {0}".format(traverse(ll)))
