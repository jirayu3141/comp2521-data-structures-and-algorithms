# COMP2521 19T1 ... lab 4 code

CC	= 2521 3c
LDLIBS	= -lbsd		 
//CFLAGS	= -Wall -Werror -std=gnu11 -g


.PHONY: all
all:	tlab

tlab:	tlab.o Tree.o
tlab.o:	tlab.c Tree.h
Tree.o:	Tree.c Tree.h

.PHONY: clean
clean:
	-rm -f tlab tlab.o Tree.o

.PHONY: give
give: Tree.c
	give cs2521 lab04 $^
