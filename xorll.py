# daily coding problem #6 (hard, but i did it in one shot which is good)
#
# Changelog:
#
# 06-08-2019
#
# initial creation. does not actually contain the necessary implementation
# because this must be done in a language with pointers; python has none. this
# file insteads evaluates a function that calls an executable that tests the
# implementation of the xor linked list, which was done in C. the C program
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

# necessary to externally call the C program
import subprocess
import sys
# import evaluation function
from ufunc_eval import ufunc_eval

def xorll_test(exe_name, A):
    """
    wrapper for the implementation; runtime not relevant here. takes a string,
    which should be the name of the external executable that test the C
    implementation of an xor linked list, and a list of integers. note that the
    C function atoi(), which is used to parse the string arguments into ints,
    returns 0 if it cannot parse a string into an int, and also has a fixed size
    on how large the int can be. so please keep ints under 32-bit signed.
    """
    if (exe_name is None) or (exe_name == ""):
        print("{0}: error: empty executable name".format(xorll_test.__name__),
              file = sys.stderr)
        quit(1)
    # convert all elements in A to strings (for sanity purposes)
    for i in range(len(A)): A[i] = str(A[i])
    # run and return stdout; strip extra newline from output
    cp = subprocess.run([exe_name] + A, capture_output = True, text = True)
    return cp.stdout.rstrip()

# main
if __name__ == "__main__":
    A = [3, 4, 2, 34, -2, 43, -98, 11]
    ufunc_eval(xorll_test, "./xorll/main.exe", A) # print output
