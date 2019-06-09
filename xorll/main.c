/**
 * main.c
 *
 * main wrapper for running the xor linked list program. lazily will use atoi()
 * take arguments from the command line in this format:
 *
 * arg1 arg2 arg3 ... argn
 *
 * the program will simply insert the n int arguments into an xor linked list
 * and then return the value of all the elements inserted in the linked list in
 * reverse order, since in the program the head and tail pointers of the linked
 * list will be swapped. the size of the linked list will also be printed.
 *
 * Changelog:
 *
 * 06-09-2019
 *
 * added changelog and edited header comments; atoi() returns 0 anyways if it
 * can't parse the string fed to it.
 *
 * 06-08-2019
 *
 * initial creation and completion date.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "xorll.h"

// program name
#define PROGNAME "xorll_test"

// help flag
#define HELP_FLAG "--help"

// help string
#define HELP_STR "Usage: " PROGNAME " arg1 arg2 ... argn\n" \
    "       " PROGNAME " [ " HELP_FLAG " ]\n" \
    "takes n integers from the command line. will initialize an xor linked\n" \
    "list, print its size, and then print out all the inserted elements in\n" \
    "reverse order (head and tail pointers in the list struct are swapped).\n" \
    "this program may be called from the command line, or (most likely) be\n" \
    "run from a python script using subprocess.run().\n" \
    "\nnote that this program uses atoi(), so don't pass in too big ints! if\n" \
    "any of the arguments are not int, atoi() will return 0."

int main(int argc, char **argv) {
    // if there are no arguments
    if (argc == 1) {
	fprintf(stderr, "%s: no arguments. type \'%s %s\' for usage\n", \
		PROGNAME, PROGNAME, HELP_FLAG);
	exit(0);
    }
    // if there are two arguments
    else if (argc == 2) {
	// if one is the help flag, then print the usage information
	if (strcmp(argv[1], HELP_FLAG) == 0) {
	    printf("%s\n", HELP_STR);
	    exit(0);
	}
	// else pass
    }
    // else get n, the number of arguments, from argc. i is counter
    int n, i;
    n = argc - 1;
    // create new xorll, and append all the elements to it
    xorll *xll;
    xll = xorll__new();
    for (i = 0; i < n; i++) {
	// note that any invalid (non-int) args will become 0
	xorll__append(xll, atoi(argv[i + 1]));
    }
    // print the size of the linked list
    printf("size of xor linked list at %p: %d\n", xll, xll->siz);
    // temp xor_node
    xor_node *temp;
    // swap head and tail pointers
    temp = xll->head;
    xll->head = xll->tail;
    xll->tail = temp;
    // print elements in reverse order
    printf("reversed list: ");
    for (i = 0; i < n; i++) {
	// get the node using xorll__getnode
	temp = xorll__getnode(xll, i);
	// print its value
	printf("%d", temp->data);
	// if i is not the last index, print a separating space
	if (i < n - 1) {
	    printf(" ");
	}
    }
    // print finishing newline and exit
    printf("\n");
    return 0;
}
