# daily coding problem #188 (medium, teaches you about a python quirk)
#
# Changelog:
#
# 12-23-2019
#
# initial creation. there is a similar problem that was asked by dropbox, but i
# only just figured out this solution. the problem is that the i in the for loop
# is being referred to by all of the functions and just having its value updated
# in the loop. my solution is to map a function f(x) that returns a function
# that prints out the input x; the map means that each element in [1, 2, 3] is
# pointed to by a separate function, so we get what we want. the reason the
# original code doesn't work is because in python, everything is an object.

__doc__ = """
This problem was asked by Google.

What will this code print out?

    def make_functions():
        flist = []

        for i in [1, 2, 3]:
            def print_i():
                print(i)
            flist.append(print_i)

        return flist

    functions = make_functions()
    for f in functions:
        f()

How can we make it print out what we apparently want?
"""

# import evaluation function
from ufunc_eval import ufunc_eval

def make_functions():
    """
    the original function given in the problem. it does print "1\n2\n3\n" as
    expected, but rather "3\n3\n3\n" since i is not garbage collected.
    """
    flist = []
    for i in [1, 2, 3]:
        def print_i(): print(i)
        flist.append(print_i)
    return flist

def make_functions_fixed():
    """
    fixes the problem by applying a map to [1, 2, 3], where the mapped function
    returns a function that returns the element it is mapped to.
    """
    # define a function make_func that returns a function that prints its input
    def make_func(x):
        def print_x(): print(x)
        return print_x
    # apply make_func using map() to the list [1, 2, 3]. this solves the problem
    # we had before, because each function is pointing to a different object.
    return list(map(make_func, [1, 2, 3]))

# main
if __name__ == "__main__":
    # a. the code will print out "3\n3\n3\n", as by referencing the i through a
    #    function, all the functions are pointing to the same i, which just has
    #    its value updated in the for loop. here's a demonstration.
    print("wrong code:")
    funcs = make_functions()
    for f in funcs: f()
    # b. use a map() method instead. each mapped function points to a different
    # element, unlike in the original make_functions, where they all pointed to
    # the same object and simply inherited the latest value of the object.
    print("right code:")
    funcs = make_functions_fixed()
    for f in funcs: f()
