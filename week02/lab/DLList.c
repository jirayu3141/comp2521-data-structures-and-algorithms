// DLList.c - Implementation of doubly-linked list ADT
// Written by John Shepherd, March 2013
// Modified by John Shepherd, August 2014, August 2015
// Modified by Jirayu Sirivorawong, 16 June 2019
#include <assert.h>
#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>

#include "DLList.h"

// data structures representing DLList

typedef struct DLListNode {
	char *value;        /**< value of this list item (string) */
	struct DLListNode *prev;
	                    /**< pointer to previous node in list */
	struct DLListNode *next;
	                    /**< pointer to next node in list */
} DLListNode;

typedef struct DLListRep {
	size_t nitems;      /**< count of items in list */
	DLListNode *first;  /**< first node in list */
	DLListNode *curr;   /**< current node in list */
	DLListNode *last;   /**< last node in list */
} DLListRep;

static DLListNode *newDLListNode(char *it);
static void freeDLListNode(DLListNode *node);

/** Create a new, empty DLList. */
DLList newDLList (void)
{
	DLListRep *new = malloc (sizeof *new);
	if (new == NULL) err (EX_OSERR, "couldn't allocate DLList");
	new->curr = new->first = new->last = NULL;
	new->nitems = 0;
	return new;
}

/** Release all resources associated with a DLList. */
void freeDLList (DLList L)
{
	if (L == NULL) return;

	DLListNode *curr = L->first;
	while (curr != NULL) {
		DLListNode *next = curr->next;
		freeDLListNode (curr);
		curr = next;
	}
	free (L);
}

/** Create a new DLListNode (private function) */
static DLListNode *newDLListNode (char *it)
{
	DLListNode *new = malloc (sizeof *new);
	if (new == NULL) err (EX_OSERR, "couldn't allocate DLList node");
	new->value = strdup (it);
	new->prev = new->next = NULL;
	return new;
}

/** Release a DLListNode (private function) */
static void freeDLListNode (DLListNode *node)
{
	if (node == NULL) return;
	free (node->value);
	free (node);
}

/** Trim off newline from strings (private function)
 * This is needed for getDLList() because fgets(3). */
static char *trim (char *s)
{
	char *nl = strrchr (s, '\n');
	if (nl != NULL) *nl = '\0';
	return s;
}

/** Create an DLList by reading items from a file.
 * Assume that the file is open for reading. */
DLList getDLList (FILE *in)
{
	DLList L = newDLList ();	//create empty list
	char line[BUFSIZ];	
	while (fgets (line, BUFSIZ, in) != NULL) {	
		DLListNode *new = newDLListNode (trim (line));
		if (L->last == NULL) {	//if list is empty
			L->first = L->last = new;
		} else {
			L->last->next = new;
			new->prev = L->last;
			L->last = new;
		}
		L->nitems++;
	}
	L->curr = L->first; //storing curr pointer
	return L;
}

/** Display a DLList to file, one item per line.
 * Assume that the file is open for writing. */
void putDLList (FILE *out, DLList L)
{
	assert (out != NULL);
	assert (L != NULL);
	for (DLListNode *curr = L->first; curr != NULL; curr = curr->next)
		fprintf (out, "%s\n", curr->value);
}

/** Check internal consistency of a DLList. */
bool validDLList (DLList L)
{
	if (L == NULL) {
		fprintf (stderr, "DLList is null\n");
		return false;
	}

	if (L->first == NULL) {
		// list is empty; curr and last should be null
		if (L->last != NULL || L->curr != NULL) {
			fprintf (stderr, "Non-null pointers in empty list\n");
			return false;
		}
	} else {
		// list is not empty; curr and last should be non-null
		if (L->last == NULL || L->curr == NULL) {
			fprintf (stderr, "Null pointers in non-empty list\n");
			return false;
		}
	}

	// check scanning forward through list
	size_t count = 0;
	for (DLListNode *curr = L->first; curr != NULL; curr = curr->next) {
		if (curr->prev != NULL && curr->prev->next != curr) {
			fprintf (
				stderr,
				"Invalid forward link into node (%s)\n",
				curr->value);
			return 0;
		}

		if (curr->next != NULL && curr->next->prev != curr) {
			fprintf (
				stderr,
				"Invalid backward link into node (%s)\n",
				curr->value);
			return 0;
		}
		count++;
	}

	if (count != L->nitems) {
		fprintf (
			stderr,
			"Forward count mismatch; counted=%zu, nitems=%zu\n",
			count,
			L->nitems);
		return false;
	}

	// check scanning backward through list
	count = 0;
	for (DLListNode *curr = L->last; curr != NULL; curr = curr->prev)
		count++;

	if (count != L->nitems) {
		fprintf (
			stderr,
			"Backward count mismatch; counted=%zu, nitems=%zu\n",
			count,
			L->nitems);
		return false;
	}
	// nothing went wrong => must be ok
	return true;
}

/** Return the item at current position. */
char *DLListCurrent (DLList L)
{
	assert (L != NULL);
	return L->curr != NULL ? L->curr->value : NULL;
}

/** Move current position (+ve forward, -ve backward).
 * Return false if we reach the end of list during the move. */
bool DLListMove (DLList L, int n)
{
	assert (L != NULL);
	if (L->curr == NULL) {
		return true;
	} else if (n > 0) {
		while (n > 0 && L->curr->next != NULL) {
			L->curr = L->curr->next;
			n--;
		}
	} else if (n < 0) {
		while (n < 0 && L->curr->prev != NULL) {
			L->curr = L->curr->prev;
			n++;
		}
	}

	return L->curr != L->first && L->curr != L->last;
}

/** move to specified position in list
 * i'th node, assuming first node has i==1 */
bool DLListMoveTo (DLList L, int i)
{
	assert (L != NULL);
	//assert (i > 0);
	L->curr = L->first;
	return DLListMove (L, i - 1);
}

/** insert an item before current item
 * new item becomes current item */
void DLListBefore (DLList L, char *it)
{
	assert (L != NULL);
	//create a new node with string stored in it
	DLListNode *new = newDLListNode(it);
	//if it is an empty list, insert it infront
	if (L->last == NULL) {
		//points the first pointer to new node with the new stirng
		L->first = new;
		L->curr = new;
		L->last = new;
		L->nitems++;
	} else {	//if the list already exists,
		//for the first node insert
		if (L->curr == L->first) {
			new->next = L->curr;	//points the next node to the curr
			new->prev = NULL; //points the prev to the prev node
			L->curr->prev = new;
			L->curr = new;	//new item becomes current item
			L->first = new;	//update the first pointer
			L->nitems++;
		} else {
			new->next = L->curr;	//points the next node to the curr
			new->prev = L->curr->prev; //points the prev to the prev node
			L->curr->prev->next = new;	//point the prev of curr to the new
			L->curr->prev = new;
			L->curr = new;	//new item becomes current item
			L->nitems++;
		}
	}
}

/** insert an item after current item
 * new item becomes current item */
void DLListAfter (DLList L, char *it)
{
	assert (L != NULL);
	DLListNode *new = newDLListNode(it);
	//one node
	if (L->curr == L->last) {
		new->next = NULL;
		new->prev = L->last;
		L->curr->next = new;
		L->curr = new;
		L->nitems++;
		L->last = new;
	//more than one node
	} else {
		new->next = L->curr->next;
		new->prev = L->curr;
		L->curr->next->prev = new;
		L->curr->next = new;
		L->curr = new;
		L->nitems++;
	}
}

/** delete current item
 * new item becomes item following current
 * if current was last, current becomes new last
 * if current was only item, current becomes null */
void DLListDelete (DLList L)
{
	assert (L != NULL);
	DLListNode *new = L->curr;

	//if only one element is left
	if (L->first == L->last) {
		L->nitems--;
		L->first = NULL;
		free(L);
	//if delete the first node
	} else if (L->curr == L->first) {
	    L->curr->next->prev = NULL;
	    L->curr = L->curr->next;
		L->first = L->curr;
	    free(new);
		L->nitems--;
	//if delete the last node
	} else if (L->curr == L->last) {
	    L->curr->prev->next = NULL;
	    L->curr = L->curr->prev;
		L->last = L->curr;
	    free(new);
		L->nitems--;
	//if delete anything in between
	} else {
		L->curr->prev->next = L->curr->next;
		L->curr->next->prev = L->curr->prev;
		L->curr = L->curr->next;
		free(new);
		L->nitems--;
	}
}

/** return number of elements in a list */
size_t DLListLength (DLList L)
{
	return L->nitems;
}

/** is the list empty? */
bool DLListIsEmpty (DLList L)
{
	return L->nitems == 0;
}