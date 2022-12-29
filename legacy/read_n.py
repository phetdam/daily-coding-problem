# daily coding problem #82 (easy, but takes some thought)
#
# Changelog:
#
# 08-26-2019
#
# initial creation. was confused about how to maintain the file pointer with
# the read7() calls until i realized that having a global buffer to hold the
# unused characters would be a good way to solve this issue.

__doc__ = """
This problem was asked Microsoft.

Using a read7() method that returns 7 characters from a file, implement readN(n)
which reads n characters. For example, given a file with the content
"Hello world", three read7() returns "Hello w", "orld" and then "".
"""

# import evaluation function
from ufunc_eval import ufunc_eval

# overflow buffer; keep any characters we do not return immediately
# here so we do not make another read7() call until necessary
_overbuf = ""

def read7(f):
    """
    returns up to 7 characters from an already opened file.
    """
    assert (f is not None) and (hasattr(f, "read") == True)
    return f.read(7)


def readn(f, n):
    """
    using read7(), returns n characters from the file f. stores unused
    characters in global variable _overbuf so we can still use the
    f.read() call in read7() to track the file pointer position.
    """
    assert (f is not None) and (hasattr(f, "read") == True) and n > 0
    # declare _overbuf as global
    global _overbuf
    # get number of characters we need to read that are not in _overbuf
    ndelta = n - len(_overbuf)
    # number of read7() calls to have sufficient characters to read
    ncalls = (ndelta // 7) if (ndelta % 7 == 0) else (ndelta // 7 + 1)
    # string from the read7() calls
    buf = ""
    for i in range(ncalls): buf = buf + read7(f)
    # output string; comprised of _overbuf + ndelta chars from buf
    outs = _overbuf + buf[:ndelta]
    # set _overbuf to the remaining len(buf) - ndelta chars remaining
    _overbuf = buf[ndelta:]
    # return outs
    return outs

# main
if __name__ == "__main__":
    func = readn
    # use itself as input
    f = "./read_n.py"
    f = open(f, "r")
    # returns "# daily coding pr"
    n = 17
    ufunc_eval(func, f, n)
    # returns "oblem #82 (ea"
    n = 13
    ufunc_eval(func, f, n)
    # returns "sy, but ... maintain the f"
    n = 120
    ufunc_eval(func, f, n)
    f.close()
