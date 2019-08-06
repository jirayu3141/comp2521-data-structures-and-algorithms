
// Referenced From comp2521 lab01 and lab02. Modified to fit search engine by Jirayu Sirivorawong
/* 
WordTree is used as the main data structure for storing all the words from files
it uses:
1. AVL Tree to maintain a balaced tree (alphabetically)
2. LinkedList to store the URLs
 */


#include <stdio.h>
#ifndef BSTREE_H
#define BSTREE_H

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

#endif
