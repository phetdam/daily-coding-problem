# daily coding problem #74 (medium)
#
# Changelog:
#
# 08-16-2019
#
# initial creation. use properties of the multiplication table to make a
# natively O(n^2) task into an O(n) task.

__doc__ = """
This problem was asked by Apple.

Suppose you have a multiplication table that is N by N. That is, a 2D array
where the value at the i-th row and j-th column is (i + 1) * (j + 1) (if
0-indexed) or i * j (if 1-indexed). Given integers N and X, write a function
that returns the number of times X appears as a value in an N by N
multiplication table. For example, given N = 6 and X = 12, you should return 4,
since the multiplication table looks like this:

| 1 | 2 | 3 | 4 | 5 | 6 |

| 2 | 4 | 6 | 8 | 10 | 12 |

| 3 | 6 | 9 | 12 | 15 | 18 |

| 4 | 8 | 12 | 16 | 20 | 24 |

| 5 | 10 | 15 | 20 | 25 | 30 |

| 6 | 12 | 18 | 24 | 30 | 36 |

And there are 4 12's in the table.
"""

# import evaluation function
from ufunc_eval import ufunc_eval

def mt_freq(n, x):
    """
    O(n) solution, whereas the naive solution is O(n^2) in time, and potentially
    O(n^2) in space if you actually stored the entire multiplication table. we
    simply use properties of the multiplication table, which is that the rows
    increase one by one, so we can test for x's presence in a row. first check
    that x % row_num == 0, where we are 1-indexing, to make sure that x has the
    possibility of being in the row, and then get k = x // row_num, and if k is
    in the range of multuplication table, then x is in the row.
    """
    # sanity checks
    assert n > 0 and x > 0
    # occurrences of x in the table
    freq = 0
    # for each row of the multiplication table
    for i in range(n):
        # check if x is divisible by the row number
        if x % (i + 1) == 0:
            # if so, get the quotient
            k = x // (i + 1)
            # if the quotient is a column in the table, x is in row i
            if (k > 0) and (k < n + 1): freq = freq + 1
    # return freq
    return freq

# main
if __name__ == "__main__":
    func = mt_freq
    # problem input, answer is 4
    n = 6
    x = 12
    ufunc_eval(func, n, x)
    # another input, answer is 0 (since 19 is prime!)
    n = 7
    x = 19
    ufunc_eval(func, n, x)
    # another input, answer is 4
    n = 8
    x = 24
    ufunc_eval(func, n, x)
