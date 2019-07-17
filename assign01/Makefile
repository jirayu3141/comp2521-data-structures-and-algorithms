

CC		= gcc
CFLAGS	= -Wall  -std=gnu11 -g
.PHONY: all
all: invertedIndex

invertedIndex: main.c invertedIndex.o
invertedIndex.o: invertedIndex.c


.PHONY: clean
clean:
	-rm -f useIntList IntList.o useIntList.o
	-rm -f randList randList.o
