# daily coding problem #x (easy/hard/medium + any other comments)
#
# Changelog:
#
# 06-07-2019
#
# changed the template to include the company that asked the question; it can
# be interesting to see who is asking what.
#
# 06-05-2019
#
# noticed that i used f instead of func in ufunc_eval. corrected.
#
# 06-04-2019
#
# initial creation. change the date above to whatever the file creation date is
# and write some comments about the runtime of your solutions and how you
# solved/failed to solve and approached the problem.

__doc__ = """
copy the problem starting with the sentence 'This problem was asked by xxx
company', and then insert a blank line between that sentence and the body of the
question. get rid of all other extra line breaks. the question statement should
be in proper sentence case unlike this docstring. this template should be reused
for all problems from daily coding problem. note that the help utility will only
wrap lines that are too long, hence the newline before the doc text starts.
"""

# import evaluation function
from ufunc_eval import ufunc_eval

def f(x):
    """
    description of solution runtime and any other comments.
    """
    return True

def g(x):
    """
    description of solution runtime and any other comments. usually necessary
    because follow up question are often asked.
    """
    return False

# main
if __name__ == "__main__":
    # remove all comments in main body later
    l = 10 # and any other inputs
    func = f # set function you want to test
    ufunc_eval(func, l) # print output
