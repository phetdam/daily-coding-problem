# daily coding problem #106 (medium)
#
# Changelog:
#
# 09-17-2019
#
# initial creation. i think i might have run into a similar problem before, but
# this answer i am sure works globally than any greedy method.

__doc__ = """
This problem was asked by Pinterest.

Given an integer list where each number represents the number of hops you can
make, determine whether you can reach to the last index starting at index 0.
For example, [2, 0, 1, 0] returns True while [1, 1, 0, 1] returns False.
"""

# import evaluation function
from ufunc_eval import ufunc_eval

def crossable(ar):
    """
    we first create an adjacency list representation of a graph of ar, where
    each ith element has edges (i, i + 1), ... (i + ar[i]). since each edge
    points to elements at higher indices only, our graph is guaranteed to be
    cycle free, so we can then use BFS to traverse the graph and attempt to
    reach the final node. returns True or False. O(nm) due to BFS and graph.
    """
    assert (ar is not None) and (len(ar) > 0)
    # get length of ar
    n = len(ar)
    # make adjacency list graph
    G = [[] for _ in range(n)]
    # we stop at n - 1 as there are no edges that the last node can point to
    for i in range(n - 1):
        # need min statement in case number of hops would allow us to hop "out"
        # of the range of the indices in the array
        for j in range(min(ar[i], n - i - 1)):
            G[i].append(i + j + 1)
    # now that we have the graph, perform BFS on the graph
    queue = [0]
    while len(queue) > 0:
        u = queue.pop(0)
        # if this is the last node, return True
        if u == n - 1: return True
        # else add all its edges to the queue
        for i in range(len(G[u])):
            queue.append(G[u][i])
    # if we never reached the end, return False
    return False

# main
if __name__ == "__main__":
    func = crossable
    # problem input 1, answer is True
    ar = [2, 0, 1, 0]
    ufunc_eval(func, ar)
    # problem input 2, answer is False
    ar = [1, 1, 0, 1]
    ufunc_eval(func, ar)
    # input 3, answer is False
    ar = [3, 0, 0, 0, 3, 0, 0, 1, 2, 1, 0, 0]
    ufunc_eval(func, ar)
    # input 4, answer is True
    ar = [2, 0, 3, 0, 2, 0, 1, 2, 0, 1, 0]
    ufunc_eval(func, ar)
