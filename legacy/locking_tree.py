# daily coding problem #24 (medium, but not exactly trivial)
#
# Changelog:
#
# 06-29-2019
#
# actually got around to finishing the locking implementation.
#
# 06-28-2019
#
# redid the tree to not allow optional arguments for left and right nodes since
# every node that is inserted into the tree needs to have a parent pointer
# properly initialized as well.
#
# 06-26-2019
#
# initial creation. changed my naive implementation of locking into a more
# efficient version. the naive version used a combination of traversal up to
# the ancestor and breadth-first search for the children. will write small test
# program later since this is quite an interesting topic.

__doc__ = """
This problem was asked by Google.

Implement locking in a binary tree. A binary tree node can be locked or unlocked
only if all of its descendants or ancestors are not locked.

Design a binary tree node class with the following methods:

is_locked, which returns whether the node is locked
lock, which attempts to lock the node. If it cannot be locked, then it should
return false. Otherwise, it should lock it and return true.
unlock, which unlocks the node. If it cannot be unlocked, then it should return
false. Otherwise, it should unlock it and return true.
You may augment the node to add parent pointers or any other property you would
like. You may assume the class is used in a single-threaded program, so there is
no need for actual locks or mutexes. Each method should run in O(h), where h is
the height of the tree.
"""

# import evaluation function
from ufunc_eval import ufunc_eval

# class for a lockable binary tree node
class lbt_node:
    def __init__(self, data, parent = None):
        self.data = data
        self.left = self.right = None
        self.parent = parent
        self.locked_children = 0
        self.locked = False

    def bst_insert(self, data):
        """
        binary search tree insert (just to make this easier). for convenience,
        also returns a pointer to the inserted node. cannot handle duplicates.
        """
        if data < self.data:
            if self.left is None:
                self.left = lbt_node(data, parent = self)
                return self.left
            return self.left.bst_insert(data)
        if self.right is None:
            self.right = lbt_node(data, parent = self)
            return self.right
        return self.right.bst_insert(data)

    ### implemention of locking ###

    def is_locked(self): return self.locked

    def is_clear(self):
        """
        method for determining whether the node is lockable or unlockable, i.e.
        that none of its ancestors or descendants are locked. the naive approach
        would be to traverse in O(h) to check ancestors, and then check with
        breadth-first search to see if any descendants are locked. however, in
        the worst case, that is O(n) (if we checked the root of the tree). the
        extra field that keeps track of locked descendants makes this much
        faster; we only need to traverse O(h) every time now.
        """
        # if locked children > 0, return False
        if self.locked_children > 0: return False
        # traverse to check ancestors
        parent = self.parent
        while parent is not None:
            if parent.locked == True: return False
            parent = parent.parent
        return True

    def lock(self):
        """
        locks the node if possible. uses the locked_children attribute to check
        if the node has any locked children, while traversing in O(h) to check
        parents and increment the number of locked descendants that they have.
        """
        # check that descendants and ancestors are all unlocked
        if self.is_clear() == False: return False
        # increment the locked_children attribute of the ancestors
        parent = self.parent
        while parent is not None:
            parent.locked_children = parent.locked_children + 1
            parent = parent.parent
        # set self to locked
        self.locked = True
        return True

    def unlock(self):
        """
        unlocks the node if possible. basically performs the opposite of lock.
        note that this will still return True if the node is already unlocked,
        hence why we also do the max() in the while loop.
        """
        # check that descendants and ancestors are all unlocked
        if self.is_clear() == False: return False
        # decrement the locked_children attribute of the ancestors
        parent = self.parent
        while parent is not None:
            parent.locked_children = max(parent.locked_children - 1, 0)
            parent = parent.parent
        # self self to unlocked
        self.unlocked = True
        return True

# main
if __name__ == "__main__":
    # input list
    ar = [6, 4, 0, 23, -9, -18, 17]
    # make a small tree and insert stuff into it and lock the middle node
    root = lbt_node(ar[0])
    target = None
    n = len(ar)
    for i in range(1, n):
        cur = root.bst_insert(ar[i])
        if i == n // 2: target = cur
    # lock the target node (succeeds)
    ufunc_eval(target.lock)
    # try to lock its parent (fails)
    ufunc_eval(target.parent.lock)
    # unlock the target node (succeeds)
    ufunc_eval(target.unlock)
    # lock the parent (succeeds)
    ufunc_eval(target.parent.lock)
