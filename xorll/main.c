/**
 * @file xorll/main.c
 * @author Derek Huang
 * @brief C XOR linked list main wrapper for running XOR linked list demo
 *
 * Main wrapper for running the xor linked list program. Uses atoi() to take
 * arguments from the command line as a space-separated list.
 *
 * The program will simply insert the n int arguments into an XOR linked list
 * and then return the value of all the elements inserted in the linked list in
 * reverse order, since in the program the head and tail pointers of the linked
 * list will be swapped. The size of the linked list will also be printed.
 *
 * Changelog:
 *
 * 2022-12-27
 *  Modernization. I have become a much better C/C++ programmer since. Includes
 *  stylistic changes to the code and comments and better error handling.
 *
 * 2019-06-09
 *  Added changelog and edited header comments; atoi() returns 0 anyways if it
 *  can't parse the string fed to it.
 *
 * 2019-06-08
 *  Initial creation and completion date.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pddcp/xorll/xorll.h"

// program name, help flags, help string
#define PROGNAME "xorll_driver"
#define HELP_FLAG_SHORT "-h"
#define HELP_FLAG_LONG "--help"
#define HELP_STR \
  "Usage: " PROGNAME " [" HELP_FLAG_SHORT "] " " arg1 arg2 ... argn" \
  "\n\n" \
  "Takes n integers from the command line. Will initialize an XOR linked\n" \
  "list, print its size, and then print out all the inserted elements in\n" \
  "reverse order (head and tail pointers in the list struct are swapped)." \
  "\n\n" \
  "Note that this program uses atoi(), so don't feed it anything exceeding\n" \
  "INT_MAX. If any of the arguments are not int, atoi() will return 0."

int
main(int argc, char **argv)
{
  // print message if no arguments
  if (argc == 1) {
    fprintf(
      stderr,
      "%s: no arguments. type \'%s %s\' for usage\n",
      PROGNAME, PROGNAME, HELP_FLAG_LONG
    );
    return EXIT_SUCCESS;
  }
  // check if help flag(s) were passed
  for (int i = 1; i < argc; i++) {
    if (
      !strcmp(argv[1], HELP_FLAG_SHORT) ||
      !strcmp(argv[1], HELP_FLAG_LONG)
    ) {
      printf("%s\n", HELP_STR);
      return EXIT_SUCCESS;
    }
  }
  // get n, the number of arguments, from argc. i is counter
  size_t n = argc - 1;
  // create new xorll, and append all the elements to it
  pddcp_xorll *xll = pddcp_xorll_alloc();
  for (size_t i = 0; i < n; i++) {
    // note that any invalid (non-int) args will become 0
    if (pddcp_xorll_append(xll, atoi(argv[i + 1])))
      return EXIT_FAILURE;
  }
  // print the size of the linked list
  printf("size of XOR linked list at %p: %zu\n", xll, xll->n_nodes);
  // temp xor_node
  pddcp_xor_node *temp;
  // swap head and tail pointers
  temp = xll->head;
  xll->head = xll->tail;
  xll->tail = temp;
  // print elements in reverse order as a space-separated list
  printf("reversed list: ");
  for (size_t i = 0; i < n; i++) {
    if (pddcp_xorll_get(xll, i, &temp))
      return EXIT_FAILURE;
    printf("%.2g", temp->data);
    if (i < n - 1)
      printf(" ");
  }
  // print finishing newline and exit
  printf("\n");
  return EXIT_SUCCESS;
}
