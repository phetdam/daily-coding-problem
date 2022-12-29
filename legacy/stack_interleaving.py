# daily coding problem #180 (medium, a little tricky)
#
# Changelog:
#
# 12-23-2019
#
# initial creation. a good medium question; although a little tricky. the trick
# is to find a systematic way to interleave the elements from the second half
# of the stack with the elements of the first half; in my case, i separated the
# elements into halves using the stack and queue, and then for each popped
# element from the stack (from the second half of the original stack) i would
# dequeue an element and enqueue it after the enqeueued stack element. for odd
# values of n, an additional dequeue/enqueue needs to be performed beforehand.

__doc__ = """
This problem was asked by Google.

Given a stack of N elements, interleave the first half of the stack with the
second half reversed using only one other queue. This should be done in-place.

Recall that you can only push or pop from a stack, and enqueue or dequeue from a
queue. For example, if the stack is [1, 2, 3, 4, 5], it should become [1, 5, 2,
4, 3]. If the stack is [1, 2, 3, 4], it should become [1, 4, 2, 3].

Hint: Try working backwards from the end state.
"""

# import evaluation function
from ufunc_eval import ufunc_eval

def stack_itl(stk):
    """
    given a list treated as a stack, takes the second half of the elements and
    interleaves them in reversed order with the first half of the elements. see
    the problem examples above, using an auxiliary queue to help. in-place.
    """
    assert stk is not None
    # get number of total elements
    n = len(stk)
    # trivial return
    if n == 0: return stk
    # check how many elements are in the second half of the stack
    n2 = n // 2
    # queue
    queue = []
    # enqueue all elements onto the queue by popping them from the stack
    for i in range(n): queue.append(stk.pop())
    # dequeue the first n2 elements from queue, which are the last n2 elements
    # of the stack but in reversed insertion order
    for i in range(n2): stk.append(queue.pop(0))
    # if n - n2 > n2, then n is odd, so in preparation we first dequeue one of
    # elements from the queue onto the stack and then pop the stack to get it
    # enqueued again. if n is even, then we do not do this.
    if n - n2 > n2:
        # dequeue and then enqueue by using the stack
        stk.append(queue.pop(0))
        queue.append(stk.pop())
    # for each of the n2 elements, pop and enqueue it. then dequeue from the
    # queue, push onto stack, pop from stack and enqueue back onto queue (this
    # essentially removes the element and puts it on rear of queue). this will
    # give the desired order, but reversed, and in the queue.
    for i in range(n2):
        # interleave: pop element from stack and enqueue it
        queue.append(stk.pop())
        # dequeue and then enqueue by using the stack
        stk.append(queue.pop(0))
        queue.append(stk.pop())
        # if there are still elements on the stack, pop element from the stack
        # and then enqueue it (this is the interleaving part)
        #if len(stk) > 0: queue.append(stk.pop())
    # we reverse our result on the queue by pushing onto the stack and then
    # popping all elements from the stack back onto the queue. then to make the
    # change in-place, we dequeue all elements from the queue onto the stack.
    # a. dequeue all elements from queue onto stack
    for i in range(n): stk.append(queue.pop(0))
    # b. pop all elements from stack and enqueue (reverse order)
    for i in range(n): queue.append(stk.pop())
    # c. dequeue all elements from queue onto stack
    for i in range(n): stk.append(queue.pop(0))
    # return stack (although the change is in-place)
    return stk

# main
if __name__ == "__main__":
    func = stack_itl
    # problem input, answer is [1, 5, 2, 4, 3]
    stk = [1, 2, 3, 4, 5]
    ufunc_eval(func, stk)
    # another problem input, answer is [1, 4, 2, 3]
    stk = [1, 2, 3, 4]
    ufunc_eval(func, stk)
    # anoter input, answer is [6, 4, 3, 8, 7, -1, 1, 12, 9]
    stk = [6, 3, 7, 1, 9, 12, -1, 8, 4]
    ufunc_eval(func, stk)
