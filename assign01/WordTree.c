
#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <string.h>

#include "WordTree.h"


void TreeInsert(InvertedIndexBST t, char* word) {
    if (t == NULL) return newNode (word);
}

// Helper: make a new node containing a value
static InvertedIndexBST newNode (char* v)
{
	InvertedIndexNode *new = malloc (sizeof *new);

}