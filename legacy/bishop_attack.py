# daily coding problem #68 (medium, but pretty simple)
#
# Changelog:
#
# 08-16-2019
#
# initial creation. took advantage of the fact that attacks go through other
# pieces so that we only need to check positions and not lines of sight.

__doc__ = """
This problem was asked by Google.

On our special chessboard, two bishops attack each other if they share the same
diagonal. This includes bishops that have another bishop located between them,
i.e. bishops can attack through pieces. You are given N bishops, represented as
(row, column) tuples on a M by M chessboard. Write a function to count the
number of pairs of bishops that attack each other. The ordering of the pair
doesn't matter: (1, 2) is considered the same as (2, 1).

For example, given M = 5 and the list of bishops:

(0, 0)
(1, 2)
(2, 2)
(4, 0)

The board would look like this:

[b 0 0 0 0]
[0 0 b 0 0]
[0 0 b 0 0]
[0 0 0 0 0]
[b 0 0 0 0]

You should return 2, since bishops 1 and 3 attack each other, as well as bishops
3 and 4.
"""

# import evaluation function
from ufunc_eval import ufunc_eval

def bishop_attacks(bl, n):
    """
    O(k^2) solution, where k is the number of pairs of bishops on the board. we
    exploit the fact that the bishops can still attack each other through other
    pieces, so that we only need to check that two pieces share a diagonal.
    """
    # note that n is the chessboard dimension, and bl is the list of bishops
    assert (bl is not None) and (n > 0)
    # get length of the bishop list
    k = len(bl)
    # number of pairs attacking each other (through other pieces as well)
    pairs = 0
    # check each combination of bishops
    for i in range(k):
        ba = bl[i]
        for j in range(i + 1, k):
            bb = bl[j]
            # if x and y displacements are equal in magnitude, the two bishops
            # in question are definitely on the same diagonal
            if abs(ba[0] - bb[0]) == abs(ba[1] - bb[1]): pairs = pairs + 1
    return pairs

# main
if __name__ == "__main__":
    func = bishop_attacks
    # problem input, answer is 2
    bl = [(0, 0), (1, 2), (2, 2), (4, 0)]
    n = 5
    ufunc_eval(func, bl, n)
    # another input, answer is 4
    bl = [(0, 0), (0, 5), (1, 0), (2, 3), (2, 5), (4, 1), (5, 3), (6, 0), (6, 5)]
    n = 7
    ufunc_eval(func, bl, n)
