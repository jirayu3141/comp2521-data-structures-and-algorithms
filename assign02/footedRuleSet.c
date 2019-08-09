// set.c ... simple, inefficient set of Strings
// Written by John Shepherd, September 2015

#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>

#include "footedRuleSet.h"

#define strEQ(s, t) (strcmp ((s), (t)) == 0)
#define strLT(s, t) (strcmp ((s), (t)) < 0)


// Function signatures

static setLink newNode (char *);
static void dropNode (setLink);
static int findNode (setLink, char *, setLink *, setLink *);

// newSet()
// - create an initially empty set
set newSet (void)
{
	set new = malloc (sizeof *new);
	if (new == NULL)
		err (EX_OSERR, "couldn't allocate set");
	*new = (setRep){.nelems = 0, .elems = NULL};
	return new;
}

// dropSet(set)
// - clean up memory associated with set
void dropSet (set s)
{
	if (s == NULL)
		return;
	setLink curr = s->elems;
	while (curr != NULL) {
		setLink next = curr->next;
		dropNode (curr);
		curr = next;
	}

	free (s);
}

// insertInto(set,Str)
// - ensure that Str is in set
void insertInto (set s, char *str)
{
	assert (s != NULL);
	setLink curr, prev;
	int found = findNode (s->elems, str, &curr, &prev);
	if (found)
		return; // already in set

	setLink new = newNode (str);
	s->nelems++;
	if (prev == NULL) {
		// add at start of list of elems
		new->next = s->elems;
		s->elems = new;
	} else {
		// add into list of elems
		new->next = prev->next;
		prev->next = new;
	}
}

// dropFrom(set,Str)
// - ensure that Str is not in set
void dropFrom (set s, char *str)
{
	assert (s != NULL);
	setLink curr, prev;
	int found = findNode (s->elems, str, &curr, &prev);
	if (!found)
		return;
	s->nelems--;
	if (prev == NULL)
		s->elems = curr->next;
	else
		prev->next = curr->next;
	dropNode (curr);
}

// isElem(set,Str)
// - check whether Str is contained in set
int isElem (set s, char *str)
{
	assert (s != NULL);
	setLink curr, prev;
	return findNode (s->elems, str, &curr, &prev);
}

// nElems(set)
// - return # elements in set
int nElems (set s)
{
	assert (s != NULL);
	return s->nelems;
}

// showSet(set)
// - display set (for debugging)
void showSet (set s)
{
	setLink curr;
	if (s->nelems == 0)
		printf ("set is empty\n");
	else {
		printf ("set has %d elements:\n", s->nelems);
		int id = 0;
		curr = s->elems;
		while (curr != NULL) {
			printf ("[%03d] %s\n", id, curr->val);
			id++;
			curr = curr->next;
		}
	}
}

// Helper functions

static setLink newNode (char *str)
{
	setLink new = malloc (sizeof (setNode));
	assert (new != NULL);
	new->val = strdup (str);
	new->next = NULL;
	return new;
}

static void dropNode (setLink curr)
{
	assert (curr != NULL);
	free (curr->val);
	free (curr);
}

// findNode(L,Str)
// - finds where Str could be added into L
// - if already in L, curr->val == Str
// - if not already in L, curr and prev indicate where to insert
// - return value indicates whether Str found or not
static int findNode (setLink list, char *str, setLink *cur, setLink *pre)
{
	setLink curr = list, prev = NULL;
	while (curr != NULL && strLT (curr->val, str)) {
		prev = curr;
		curr = curr->next;
	}
	*cur = curr;
	*pre = prev;
	return (curr != NULL && strEQ (str, curr->val));
}
