# daily coding problem #104 (easy, but not really)
#
# Changelog:
#
# 09-16-2019
#
# ended up finishing today. used property of doubly linked list to simply
# traverse in opposite directions, but for singly linked had to use extra
# memory to build up half of the list to be checked in reverse.
#
# 09-15-2019
#
# initial creation. may not finish today.

__doc__ = """
This problem was asked by Google.

Determine whether a doubly linked list is a palindrome. What if it's singly
linked? For example, 1 -> 4 -> 3 -> 4 -> 1 returns True while 1 -> 4 returns
False.
"""

# import evaluation function
from ufunc_eval import ufunc_eval

# doubly linked list node
class dbnode:

    def __init__(self, data, next = None, prev = None):
        self.data = data
        self.next = next
        self.prev = prev

# singly linked list node
class node:

    def __init__(self, data, next = None):
        self.data = data
        self.next = next

def ar2ll(ar):
    """
    supporting method for creating a singly liked list from an array of ints.
    """
    if (ar is None) or (len(ar) == 0): return None
    n = len(ar)
    # create root of list
    root = cur = node(ar[0])
    # for each element in array except for the first
    for i in range(1, n):
        cur.next = node(ar[i])
        cur = cur.next
    # return root
    return root

def ar2dbll(ar):
    """
    supporting method for creating a doubly linked list from an array of ints.
    """
    if (ar is None) or (len(ar) == 0): return None
    n = len(ar)
    # create root of list
    root = cur = dbnode(ar[0])
    # for each element in array except for the first
    for i in range(1, n):
        cur.next = dbnode(ar[i], prev = cur)
        cur = cur.next
    # return root
    return root

def traverse(root):
    """
    traverse (forwards) a single or doubly linked list. checks if root is a
    singly or doubly linked list. not really needed, used for error checks
    """
    if root is None: return "None"
    assert isinstance(root, node) or isinstance(root, dbnode)
    dbl = None
    if isinstance(root, node): dbl = False
    elif isinstance(root, dbnode): dbl = True
    cur = root
    outs = "<~ " if dbl is True else ""
    while cur is not None:
        if dbl == True:
            if cur.next is None: outs = outs + str(cur.data) + " ~>"
            else: outs = outs + str(cur.data) + " <=> "
        else: outs = outs + str(cur.data) + " ~> "
        cur = cur.next
    return outs

def is_palin_dbll(head):
    """
    O(n). exploit property of doubly linked list by traversing from each end
    in opposite directions, stopping after going past the midpoint.
    """
    assert isinstance(head, dbnode)
    if head is None: return True
    # find the midpoint + 1; when the forward index hits this node, we stop
    slow = fast = head
    while (fast.next is not None) and (fast.next.next is not None):
        slow = slow.next
        fast = fast.next.next
    stop = slow.next
    # forward pointer
    cur_fw = root
    # backwards pointer
    cur_bk = fast
    while cur_bk.next is not None: cur_bk = cur_bk.next
    # keep checing palindrome until cur_fw hits stop
    while cur_fw != stop:
        # if fw does not match bk, return False
        if cur_fw.data != cur_bk.data: return False
        # else advance fw, retract bk
        cur_fw = cur_fw.next
        cur_bk = cur_bk.prev
    # return True if we successfully checked everything
    return True

def is_palin_ll(head):
    """
    O(n). determine stop point (differs if length is even or odd), use result to
    determine position of forward node. collect nodes up to midpoint, and when
    past midpoint, check backwards against elements in array.
    """
    assert isinstance(head, node)
    if head is None: return True
    # find the stopping point and count number of nodes in the list
    slow = fast = head
    ndn = 0
    while (fast.next is not None) and (fast.next.next is not None):
        slow = slow.next
        fast = fast.next.next
        ndn = ndn + 1
    stop = slow
    while slow is not None:
        slow = slow.next
        ndn = ndn + 1
    # if ndn == 1, return True
    if ndn == 1: return True
    # if ndn is evenly divisible, need to advance stop
    if ndn % 2 == 0: stop = stop.next
    # build array for first half of the list
    lar = []
    # traverse forward until stopping point and build lar
    cur = head
    while cur != stop:
        lar.append(cur.data)
        cur = cur.next
    # if ndn is odd, skip the stop (odd palindrome can skip middle element)
    if ndn % 2 > 0: cur = cur.next
    # else go backwards through the array and forwards through the list to
    # check elements; any mismatch returns False
    for i in range(len(lar)):
        # if mismatch, return False
        if cur.data != lar[-(i + 1)]: return False
        cur = cur.next
    # finally made it
    return True

# main
if __name__ == "__main__":
    func_a = is_palin_dbll
    func_b = is_palin_ll
    # problem input 1, answer is True
    ar = [1, 4, 3, 4, 1]
    print(ar)
    root = ar2dbll(ar)
    ufunc_eval(func_a, root)
    root = ar2ll(ar)
    ufunc_eval(func_b, root)
    # problem input 2, answer is False
    ar = [1, 4]
    print(ar)
    root = ar2dbll(ar)
    ufunc_eval(func_a, root)
    root = ar2ll(ar)
    ufunc_eval(func_b, root)
    # problem input 3, answer is True
    ar = [1, 3, 5, 2, 2, 5, 3, 1]
    print(ar)
    root = ar2dbll(ar)
    ufunc_eval(func_a, root)
    root = ar2ll(ar)
    ufunc_eval(func_b, root)
