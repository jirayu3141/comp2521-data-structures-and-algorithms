
// Referenced From comp2521 lab01 and lab02. Modified to fit search engine by Jirayu Sirivorawong
/* 
WordTree is used as the main data structure for storing all the words from files
it uses:
1. AVL Tree to maintain a balaced tree (alphabetically)
2. LinkedList to store the URLs
 */


#ifndef BSTREE_H
#define BSTREE_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
typedef struct TreeRep *Tree;

// Items and operations on Items
typedef char* Key;
typedef Key Item; // item is just a key
#define key(it) (it)
#define cmp(k1, k2) ((k1) - (k2))
#define less(k1, k2) (cmp (k1, k2) < 0)
#define eq(k1, k2) (cmp (k1, k2) == 0)

// kinds of insertion supported
typedef enum {
	InsertAtLeaf,
	InsertAtRoot,
	InsertRandom,
	InsertRebalance,
	InsertSplay,
	InsertAVL
} Style;

// Pointer to Representation of Trees
typedef struct Node *Link;

// Pointer to Representation of UrlList
typedef struct DLListRep *DLList;


// Node of LinkedList
typedef struct DLListNode {
	char *value;        /**< value of this list item (string) */
	struct DLListNode *prev;
	                    /**< pointer to previous node in list */
	struct DLListNode *next;
	                    /**< pointer to next node in list */
} DLListNode;

// Representation of UrlList 
typedef struct DLListRep {
	size_t nitems;      /**< count of items in list */
	DLListNode *first;  /**< first node in list */
	DLListNode *curr;   /**< current node in list */
	DLListNode *last;   /**< last node in list */
} DLListRep;

// Node of a tree
typedef struct Node {
	Item value;
	Link left, right;
    DLList url;
	Tree within; // which tree contains this Node
} Node;

// Representation of Trees
typedef struct TreeRep {
	Link root;
	Style insert;
	int ncompares;
	int nrotates;
} TreeRep;

// Interface Functions

// create an empty Tree
Tree newTree (Style);
// free memory associated with Tree
void dropTree (Tree);
// display a Tree + stats about Tree
void showTree (Tree);

// insert a new value into a Tree
void TreeInsert (Tree, Item);
// delete a value from a Tree
void TreeDelete (Tree, Key);
// check whether a value is in a Tree
int TreeFind (Tree, Key);
// compute depth of Tree
int TreeDepth (Tree);
// count #nodes in Tree
int TreeNumNodes (Tree);

void TreeInsertWithURL (Tree t, Item it, char* url);


/** Create a new, empty DLList. */
DLList newDLList (void);

/** Release all resources associated with a DLList. */
void freeDLList (DLList);

/** Create an DLList by reading items from a file.
 * Assume that the file is open for reading. */
DLList getDLList (FILE *);

/** Display a DLList to file, one item per line.
 * Assume that the file is open for writing. */
void putDLList (FILE *, DLList);

/** Check internal consistency of a DLList (for testing). */
bool validDLList (DLList);

/** Return the item at current position. */
char *DLListCurrent (DLList);

/** Move current position (+ve forward, -ve backward).
 * Return false if we reach the end of list during the move. */
bool DLListMove (DLList, int);

/** move to specified position in list
 * i'th node, assuming first node has i==1 */
bool DLListMoveTo (DLList, int);

/** insert an item before current item
 * new item becomes current item */
void DLListBefore (DLList, char *);

/** insert an item after current item
 * new item becomes current item */
void DLListAfter (DLList, char *);

/** delete current item
 * new item becomes item following current
 * if current was last, current becomes new last
 * if current was only item, current becomes null */
void DLListDelete (DLList);

/** return number of elements in a list */
size_t DLListLength (DLList);

/** is the list empty? */
bool DLListIsEmpty (DLList);

bool urlExist (DLList, char *url);

void DLListInsertInOrder (DLList, char*);

void infix(Tree tree, FILE *output);

void showBSTreeNodeandList(Tree t, FILE *output);

void DLListPrint (DLList L, FILE *destination);
#endif
