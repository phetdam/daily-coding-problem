__doc__ = """
module containing a single function designed to evaluate and print the output of
a function, taking variable arguments. will print the function name, args, and
ultimately the function output, and returns the results from the function eval
if no errors have occurred. will attempt to keep the output to 80 characters
in width per line, and preserve the newlines.
"""
# Changelog:
#
# 06-05-2019
#
# added ability to return results of function evaluation.
#
# 06-04-2019
#
# initial creation. took _strwrap from pyalgos_test.py, modified to accept a
# constant, initial, or subsequent level of indentation for non-empty lines,
# and completed ufunc_eval. so _strwrap actually got improved.

import sys
import textwrap

# string formatter, literally directly copied from pyalgos_test.py, where it was
# originally implemented, but modified to also allow a constant level of
# indentation for any non-empty lines (indent). if a non-"" value of indent is
# provided, it will override both initial_indent and subsequent_indent. returns
# arg as string block (with \n) width characters width; default width 80.
# wrapper around textwrap.
def _strwrap(s, width = 80, indent = "", initial_indent = "",
             subsequent_indent = ""):
    # if indent is not empty string, set initial_indent and subsequent_indent
    # to indent's value
    if indent != "": initial_indent = subsequent_indent = indent
    # split by newlines, proxy for paragraph breaks
    paras = s.split("\n")
    # for each paragraph in paras
    for i in range(len(paras)):
        # if the paragraph is an empty string, ignore
        if paras[i] == "": pass
        # else wrap lines without replacing whitespace
        else:
            # break into list of strings, then join by newline; add indentation
            # as desired. initial_indent will apply to the first line of the
            # first paragraph only; then subsequent_indent will be used solely
            if i == 0:
                hlines = textwrap.wrap(paras[i], width = width,
                                       replace_whitespace = False,
                                       initial_indent = initial_indent,
                                       subsequent_indent = subsequent_indent)
            else:
                hlines = textwrap.wrap(paras[i], width = width,
                                       replace_whitespace = False,
                                       initial_indent = subsequent_indent,
                                       subsequent_indent = subsequent_indent)
            paras[i] = "\n".join(hlines)
    # return all strings in paras joined by newlines
    return "\n".join(paras)

def ufunc_eval(func, *args, **kwargs):
    """
    main user-called function in ufunc_eval. takes a function pointer and a
    variable number of arguments, with optional keyword arguments after the
    required arguments.
    """
    # print error is func is None
    if func == None:
        print(_strwrap("{0}: error: function pointer expected, None received"
                       "".format(ufunc_eval.__name__)), file = sys.stderr)
        return None
    # else print function name, inputs, and evaluate
    # change args format from (arg1, arg2, ...) -> arg1, arg2, ...
    str_args = str(args).strip(",()")
    str_kwargs = None
    # if length of kwargs is 0, set str_kwargs to ""
    if len(kwargs) == 0: str_kwargs = ""
    # else change dict format from {'key': val, ... } -> key = val, ...
    else:
        str_kwargs = ", " + str(kwargs).strip(
            "{}").replace("'", "").replace(":", " =")
    # print with indents
    print(_strwrap("{0} -> {1}({2})"
                   "".format(ufunc_eval.__name__, func.__name__,
                             str_args + str_kwargs)))
    # get result of function, for later return
    res = func(*args, **kwargs)
    print(_strwrap("{0}".format(res), initial_indent = " -> ",
                   subsequent_indent = "    "))
    # return results of function evaluation
    return res
