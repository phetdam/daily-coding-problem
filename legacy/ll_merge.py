# daily coding problem #78 (medium)
#
# Changelog:
#
# 08-19-2019
#
# initial creation. pretty straightforward problem; just take the merge sort
# merge approach to merge all the lists. for some reason i decided to use
# varargs instead of just passing in an array.

__doc__ = """
This problem was asked by Google.

Given k sorted singly linked lists, write a function to merge all the lists into
one sorted singly linked list.
"""

# import evaluation function
from ufunc_eval import ufunc_eval

# linked list node class
class node:

    def __init__(self, data, next_node = None):
        self.data = data
        self.next = next_node

def ll_merge(*args):
    """
    takes a variable number of sorted linked lists and returns a final linked
    list from all its constituents. uses the merge sort idea to merge the lists.
    note that the merge is in-place, so that the original lists are all merged
    into a new final linked list. no extra memory used.
    """
    assert len(args) > 0
    # get number of linked lists
    n = len(args)
    # if n is 1, return the first linked list as is
    if n == 1: return args[0]
    # else call recursively on the arguments
    # get midpoint of array
    m = (n - 1) // 2
    # sorted list from 0 to m and from m + 1 to n - 1
    l_cur = ll_merge(*args[:(m + 1)])
    r_cur = ll_merge(*args[(m + 1):])
    # choose smaller head one as the head for the merged list
    head = None
    if l_cur.data <= r_cur.data:
        head = l_cur
        l_cur = l_cur.next
    else:
        head = r_cur
        r_cur = r_cur.next
    # current node in the final list we are pointing at
    cur = head
    # while neither currents are None
    while (l_cur is not None) and (r_cur is not None):
        # if l_cur.data <= r_cur.data, make cur.next point to l_cur and advance
        if l_cur.data <= r_cur.data:
            cur.next = l_cur
            l_cur = l_cur.next
        # else set cur.next to r_cur and advance
        else:
            cur.next = r_cur
            r_cur = r_cur.next
        # advance cur
        cur = cur.next
    # append any remaining elements from l_cur or r_cur
    while l_cur is not None:
        cur.next = l_cur
        l_cur = l_cur.next
        cur = cur.next
    while r_cur is not None:
        cur.next = r_cur
        r_cur = r_cur.next
        cur = cur.next
    # return head of merged list
    return head

def traverse(head):
    """
    prints out a linked list. literally copied from ll_reverse.py except i
    changed the arrow format to use a tilde for extra fanciness
    """
    if head is None: return "None"
    outs = ""
    cur = head
    while cur is not None:
        outs = outs + str(cur.data) + " ~> "
        cur = cur.next
    return outs

def ll_to_ar(head):
    """
    returns an array from a linked list
    """
    outl = []
    if head is None: return outl
    cur = head
    while cur is not None:
        outl.append(cur.data)
        cur = cur.next
    return outl

# main
if __name__ == "__main__":
    func = ll_merge
    # array of several small sorted linked lists
    lar = [node(-9, next_node =
                node(1, next_node =
                     node(3, next_node =
                          node(19)))),
           node(2, next_node =
                node(6, next_node =
                     node(21))),
           node(1, next_node =
                node(17, next_node =
                     node(31, next_node =
                          node(82, next_node =
                               node(100))))),
           node(-7, next_node =
                node(9)),
           node(5, next_node =
                node(101, next_node =
                     node(190))),
           node(10),
           node(6, next_node =
                node(11, next_node =
                     node(13, next_node =
                          node(22))))]
    # print what the input looks like
    print([ll_to_ar(lar[i]) for i in range(len(lar))])
    # merge and print output
    res = ufunc_eval(func, *lar)
    print(traverse(res))
