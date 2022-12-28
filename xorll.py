# daily coding problem #6 (hard, but i did it in one shot which is good)
#
# Changelog:
#
# 2022-12-28
#
# Modernization. Also have become a better Python programmer since then. Note
# that the only changes that have been made are stylistic or idiomatic.
#
# 2019-06-08
#
# Initial creation. Does not actually contain the necessary implementation
# because this must be done in a language with pointers; Python has none. This
# file insteads evaluates a function that calls an executable that tests the
# implementation of the XOR linked list, which was done in C. The C program
# that tests the xor list implementation is located in ./xorll.

__doc__ = """
This problem was asked by Google.

An XOR linked list is a more memory efficient doubly linked list. Instead of
each node holding next and prev fields, it holds a field named both, which is an
XOR of the next node and the previous node. Implement an XOR linked list; it has
an add(element) which adds the element to the end, and a get(index) which
returns the node at index. If using a language that has no pointers (such as
Python), you can assume you have access to get_pointer and dereference_pointer
functions that converts between nodes and memory addresses.
"""

import os.path
import subprocess
from typing import List

from ufunc_eval import ufunc_eval


def xorll_test(exe_name: str, values: List[int]) -> str:
    """Wrapper for the XOR linked list driver program.

    Note that the C function atoi(), which is used to parse the string
    arguments into ints, returns 0 if it cannot parse a string into an int.

    Parameters
    ----------
    exe_name : str
        Name of the driver program to call
    values : List[int]
        Array of integers to send to the driver program

    Raises
    ------
    RuntimeError
        When executable name is empty
    CalledProcessError
        If driver program has non-zero exit
    """
    if not exe_name:
        raise RuntimeError(
            f"{xorll_test.__name__}: error: empty executable name"
        )
    # create new string array of elements in values). these will be passed as
    # command-line arguments to the driver program.
    str_values = [str(value) for value in values]
    # run driver with args, throws on non-zero exit. we strip final newline
    res = subprocess.run(
        [exe_name] + str_values, capture_output=True, text=True, check=False
    )
    return res.stdout.rstrip()


if __name__ == "__main__":
    arr = [3, 4, 2, 34, -2, 43, -98, 11]
    ufunc_eval(xorll_test, os.path.join(".", "xorll", "xorll_driver"), arr)
