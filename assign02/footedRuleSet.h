// set.h ... interface to simple set of Strings
// Written by John Shepherd, September 2015

#ifndef SET_H
#define SET_H
#include "IntList.h"
typedef struct setNode *setLink;
typedef struct SetRep *set;

typedef struct setNode {
	char *val;
    int *position_original;
    IntList P; 
    double weighetedResult;
	setLink next;
} setNode;

typedef struct SetRep {
	int nelems;
	setLink elems;
} setRep;
// Function signatures

set newSet (void);
void dropSet (set);
void insertInto (set, char *);
void dropFrom (set, char *);
int isElem (set, char *);
int nElems (set);
void showSet (set);

#endif
