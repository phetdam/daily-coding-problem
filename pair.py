# daily coding problem #5 (medium, asked by the legendary jane street!)
#
# Changelog:
#
# 06-08-2019
#
# edited the previous changelog entry to add some relevant comments.
#
# 06-07-2019
#
# initial creation. easy solution, although one should note that this would not
# be valid in C: the pair pointer returned by cons would be garbage. but in
# python the interpreter remembers the location of pair in that scope.

__doc__ = """
This problem was asked by Jane Street.

cons(a, b) constructs a pair, and car(pair) and cdr(pair) returns the first and
last element of that pair. For example, car(cons(3, 4)) returns 3, and
cdr(cons(3, 4)) returns 4. Given this implementation of cons:

def cons(a, b):
    def pair(f):
        return f(a, b)
    return pair

Implement car and cdr.
"""

# import evaluation function
from ufunc_eval import ufunc_eval

# given implementation of cons
def cons(a, b):
    def pair(f):
        return f(a, b)
    return pair

def car(pair):
    """
    O(1) solution, as it should be. from what i can see, since we have a
    function declared in cons that is returning a functional operation on
    the two numbers a and b, we define our own function that returns just
    its first argument.
    """
    def get_first(x, y): return x
    return pair(get_first)

def cdr(pair):
    """
    O(1) solution, again as it should be. as we did with car, we define a
    funtion in cdr that returns just its first argument, and we feed that
    to pair as its argument, as pair is a function pointer.
    """
    def get_second(x, y): return y
    return pair(get_second)

# main
if __name__ == "__main__":
    # get function pointer from cons
    fp = cons(4, 5)
    # eval car and cdr
    ufunc_eval(car, fp)
    ufunc_eval(cdr, fp)
