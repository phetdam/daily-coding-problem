# makefile for the xor linked list testing program.
#
# Changelog:
#
# 06-08-2019
#
# initial creation. added targets for main, xorll, and clean.

CC = gcc
CFLAGS = -Wall -g

# target names
TARGET_N = main
XORLL_N = xorll

# dependencies
TARGET_DEPS = $(TARGET_N).c $(XORLL_N).o

# targets
$(TARGET_N): $(TARGET_DEPS)
	$(CC) $(CFLAGS) -o $(TARGET_N) $(TARGET_DEPS)
$(XORLL_N).o: $(XORLL_N).c
	$(CC) $(CFLAGS) -c $(XORLL_N).c
clean:
	$(RM) -vf *~ *.o
