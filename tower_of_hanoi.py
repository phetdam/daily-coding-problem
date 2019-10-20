# daily coding problem #128 (medium)
#
# Changelog:
#
# 10-19-2019
#
# initial creation. i knew a priori that the "easy" solution would be recursive,
# but i had to draw out a couple cases before i figured out the pattern. the way
# to break it down in the case of n disks is to think about moving n - 1 disks
# to the spare peg, moving the last disk to the last peg, and then moving the
# n - 1 from the spare to the last peg to complete the job.

__doc__ = """
The Tower of Hanoi is a puzzle game with three rods and n disks, each a
different size. All the disks start off on the first rod in a stack. They are
ordered by size, with the largest disk on the bottom and the smallest one at the
top. The goal of this puzzle is to move all the disks from the first rod to the
last rod while following these rules:

You can only move one disk at a time.
A move consists of taking the uppermost disk from one of the stacks and placing
it on top of another stack.
You cannot place a larger disk on top of a smaller disk.

Write a function that prints out all the steps necessary to complete the Tower
of Hanoi. You should assume that the rods are numbered, with the first rod being
1, the second (auxiliary) rod being 2, and the last (goal) rod being 3.

For example, with n = 3, we can do this in 7 moves:

Move 1 to 3
Move 1 to 2
Move 3 to 2
Move 1 to 3
Move 2 to 1
Move 2 to 3
Move 1 to 3
"""

# import evaluation function
from ufunc_eval import ufunc_eval

def hanoi(n, st = 1, spt = 2, et = 3, moves = None):
    """
    to prevent stdout from being filled by line-by-line printouts, i opt to
    return a list of all the moves. the optional arguments st, spt,and  et are
    start, spare, and ending tower respectively; by default they are 1, 2, and
    3, as that is the goal of the game. the moves list holds all the moves; the
    parent call will set it to the empty list. classic recursive solution.
    """
    # if parent call, instantiate moves
    if moves is None: moves = []
    # base case: no disks
    if n == 0: return moves
    # recursive case: output from moving n - 1 to spare tower
    moves = hanoi(n - 1, st = st, spt = et, et = spt, moves = moves)
    # move last disk from start to target tower
    moves.append("moves " + str(st) + " to " + str(et))
    # second recursive case: out from n - 1 to target tower
    moves = hanoi(n - 1, st = spt, spt = st, et = et, moves = moves)
    # return list of moves
    return moves

# main
if __name__ == "__main__":
    func = hanoi
    # problem input
    n = 3
    ufunc_eval(func, n)
    # another input
    n = 7
    ufunc_eval(func, n)
