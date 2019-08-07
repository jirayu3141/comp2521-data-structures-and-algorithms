// set.h ... interface to simple Set of Strings
// Written by John Shepherd, September 2015

#ifndef SET_H
#define SET_H

typedef struct SetNode *SetLink;
typedef struct SetRep *Set;
typedef struct SetNode {
	char *val;
	int wordScore;
	double page_rank_score;
	SetLink next;
} SetNode;

typedef struct SetRep {
	int nelems;
	SetLink elems;
} SetRep;


// Function signatures

Set newSet (void);
void dropSet (Set);
void insertInto (Set, char *);
void dropFrom (Set, char *);
int isElem (Set, char *);
int nElems (Set);
void showSet (Set);
char **extractSearhUrl(char *filename, char *word);
#endif
