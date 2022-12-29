/**
 * @file xorll/main.c
 * @author Derek Huang
 * @brief C XOR linked list main wrapper for running XOR linked list demo
 *
 * Main wrapper for running the xor linked list program. Uses atof() to take
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
 *  stylistic changes to the code and comments and better error handling. Also,
 *  note that atof() is used instead of atoi() now.
 *
 * 2019-06-09
 *  Added changelog and edited header comments; atoi() returns 0 anyways if it
 *  can't parse the string fed to it.
 *
 * 2019-06-08
 *  Initial creation and completion date.
 */

#include <stdbool.h>
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
  "This program uses atof(), which returns 0.0 if conversion is not possible."

/**
 * Check args for help flags.
 *
 * If no arguments were provided, i.e. argc == 1, then also prints message.
 *
 * @param argc `argc` arg count
 * @param argv `argv` argument array
 * @returns `true` if we should exit early, `false` for normal operation
 */
static bool
check_args(int argc, char **argv)
{
  // print message if no arguments
  if (argc == 1) {
    fprintf(
      stderr,
      "%s: no arguments. type \'%s %s\' for usage\n",
      PROGNAME, PROGNAME, HELP_FLAG_LONG
    );
    return true;
  }
  // check if help flag(s) were passed
  for (int i = 1; i < argc; i++) {
    if (
      !strcmp(argv[1], HELP_FLAG_SHORT) ||
      !strcmp(argv[1], HELP_FLAG_LONG)
    ) {
      printf("%s\n", HELP_STR);
      return true;
    }
  }
  return false;
}

int
main(int argc, char **argv)
{
  // return early if we shouldn't run the main program logic
  if(check_args(argc, argv))
    return EXIT_SUCCESS;
  // get n, the number of arguments, from argc
  size_t n = argc - 1;
  // create new xorll, and append all the elements to it
  pddcp_xorll *xll = pddcp_xorll_alloc();
  for (size_t i = 0; i < n; i++) {
    if (pddcp_xorll_append(xll, atof(argv[i + 1])))
      return EXIT_FAILURE;
  }
  // print the size of the linked list
  printf("size of XOR linked list at %p: %zu\n", xll, xll->n_nodes);
  // temp xor_node + swap head and tail pointers
  pddcp_xor_node *temp;
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
