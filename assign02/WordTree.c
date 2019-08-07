// Tree.h ... implementation of binary search tree ADT
// Written by John Shepherd, March 2013
// Modified by Jirayu Sirivorawong, Aug 2019

#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <stdbool.h>

#include "WordTree.h"
#include "readData.h"


// Forward references for private functions

static Link newNode (Item);
static Link newNodeWithUrl (Item v, char* url);
static void drop (Link);
static int depth (Link);
static int size (Link);

static Link insert (Link, Item);
static Link insertAtRoot (Link, Item);
static Link insertRandom (Link, Item);
static Link insertRebalance (Link, Item);
static Link insertSplay (Link, Item);
static Link insertAVL (Link, Item);
static Link insertAVLwithURL (Link t, Item it, char *filename);

static Link search (Link, Key);
static Link searchSplay (Link, Key, int *);

static Link delete (Link, Key);
static Link deleteRoot (Link);

static Link partition (Link, int);
static Link rebalance (Link);
static Link rotateR (Link);
static Link rotateL (Link);

static void doShowTree (Link);

static DLListNode *newDLListNode(char *it);
// used to hold current tree during insertion

static Tree thisTree = NULL;

// Interface: create a new empty Tree
Tree newTree (Style ins)
{
	TreeRep *new = malloc (sizeof *new);
	if (new == NULL) err (EX_OSERR, "couldn't allocate Tree");
	*new = (TreeRep) { .insert = ins };
	return new;
}

// Helper: make a new node containing a value
static Link newNode (Item v)
{
	Node *new = malloc (sizeof *new);
	if (new == NULL) err (EX_OSERR, "couldn't allocate Tree node");
	*new = (Node) { .value = v, .within = thisTree, .url = NULL};
	
	return new;
}

static Link newNodeWithUrl (Item v, char* url)
{
	Node *new = malloc (sizeof *new);
	if (new == NULL) err (EX_OSERR, "couldn't allocate Tree node");
	*new = (Node) { .value = v, .within = thisTree, .url = NULL};

	if (new->url == NULL) {
		//create new list
		new->url = newDLList();
		new->url->first = new->url->last = newDLListNode(url);
		new->url->nitems = 0;
		new->url->nitems = 1;

	}	
	return new;
}

// Interface: free memory associated with Tree
void dropTree (Tree t)
{
	if (t == NULL) return;
	drop (t->root);
	free (t);
}

// Helper: recursive drop
static void drop (Link t)
{
	if (t == NULL)
		return;
	drop (t->left);
	drop (t->right);
	free (t);
}

// Interface: display a Tree
void showTree (Tree t)
{
	assert (t != NULL);
	printf (
		"#nodes = %d,  depth = %d,  #compares = %d,  #rotates = %d\n",
		size (t->root),
		depth (t->root),
		t->ncompares,
		t->nrotates);
	if (size (t->root) > 60)
		printf ("Tree too large to display\n");
	else
		doShowTree (t->root);
}

// Interface: depth of Tree (max path length)
int TreeDepth (Tree t)
{
	return depth (t->root);
}

// Helper: recursive depth calculation
static int depth (Link t)
{
	if (t == NULL) return 0;
	int ldepth = depth (t->left);
	int rdepth = depth (t->right);
	return 1 + ((ldepth > rdepth) ? ldepth : rdepth);
}

// Interface: count #nodes in Tree
int TreeNumNodes (Tree t)
{
	return size (t->root);
}

// Helper: recursive node counter
static int size (Link t)
{
	if (t == NULL) return 0;
	return 1 + size (t->left) + size (t->right);
}

// Interface: insert a new value into a Tree
void TreeInsert (Tree t, Item it)
{
	thisTree = t;
	switch (t->insert) {
	case InsertAtLeaf:
		t->root = insert (t->root, it);
		break;
	case InsertAtRoot:
		t->root = insertAtRoot (t->root, it);
		break;
	case InsertRandom:
		t->root = insertRandom (t->root, it);
		break;
	case InsertRebalance:
		t->root = insertRebalance (t->root, it);
		break;
	case InsertSplay:
		t->root = insertSplay (t->root, it);
		break;
	case InsertAVL:
		t->root = insertAVL (t->root, it);
		break;
	}

	// printf("After inserting %d, tree is:\n",key(it));
	// showTree(t);
}

void TreeInsertWithURL (Tree t, Item it, char* url)
{
		t->root = insertAVLwithURL (t->root, it, url);
}

// Helpers: various styles of insert
static Link insert (Link t, Item it)
{
	if (t == NULL) return newNode (it);	//create new tree with 'it' in it.

	int diff = cmp (key (it), key (t->value));	//>0 if it is more than value
	t->within->ncompares++;
	if (diff == 0)      t->value = it;	//replace current node with new 'it'
	else if (diff <  0) t->left  = insert (t->left,  it);
	else if (diff  > 0) t->right = insert (t->right, it);
	return t;
}

static Link insertAtRoot (Link t, Item it)
{
	if (t == NULL) return newNode (it);
	int diff = cmp (key (it), key (t->value));
	t->within->ncompares++;
	if (diff == 0) {
		t->value = it;
	} else if (diff < 0) {
		t->left = insertAtRoot (t->left, it);
		t = rotateR (t);
	} else if (diff > 0) {
		t->right = insertAtRoot (t->right, it);
		t = rotateL (t);
	}
	return t;
}

static Link insertRandom (Link t, Item it)
{
	if (t == NULL) return newNode (it);
	int chance = rand () % 2;
	if (chance != 0)
		t = insertAtRoot (t, it);
	else
		t = insert (t, it);
	return t;
}

static inline double ratio (double a, double b)
{
	double max = a > b ? b : a;
	double min = a > b ? a : b;
	return max >= 1 ? min / max : __builtin_inf ();
}

static Link insertRebalance (Link t, Item it)
{
	t = insert (t, it);
	double lsize = size (t->left), rsize = size (t->right);
	if (ratio (lsize, rsize) > 1.1) t = rebalance (t);
	return t;
}

static Link insertSplay (Link t, Item it)
{
	if (t == NULL) return newNode (it);

	Key v = key (it);
	int diff = cmp (v, key (t->value));
	t->within->ncompares++;
	if (diff == 0) {
		t->value = it;

	} else if (diff < 0) {

		if (t->left == NULL) {
			t->left = newNode (it);
		} else if (less (v, key (t->left->value))) {
			t->within->ncompares++;
			t->left->left = insertSplay (t->left->left, it);
			t = rotateR (t);
		} else {
			t->within->ncompares++;
			t->left->right = insertSplay (t->left->right, it);
			t->left = rotateL (t->left);
		}

		t = rotateR (t);

	} else if (diff > 0) {
		if (t->right == NULL) {
			t->right = newNode (it);
		} else if (less (key (t->right->value), v)) {
			t->within->ncompares++;
			t->right->right = insertSplay (t->right->right, it);
			t = rotateL (t);
		} else {
			t->within->ncompares++;
			t->right->left = insertSplay (t->right->left, it);
			t->right = rotateR (t->right);
		}
		t = rotateL (t);
	}

	return t;
}

// static Link insertAVL (Link t, Item it)
// {
// 	if (t == NULL) return newNode (it);
// 	int diff = cmp (key (it), key (t->value));
// 	t->within->ncompares++;
// 	if (diff == 0)     t->value = it;
// 	else if (diff < 0) t->left  = insertAVL (t->left, it);
// 	else if (diff > 0) t->right = insertAVL (t->right, it);

// 	int dL = depth (t->left);
// 	int dR = depth (t->right);
// 	if ((dL - dR) > 1) t = rotateR (t);
// 	if ((dR - dL) > 1) t = rotateL (t);
// 	return t;
// }

static Link insertAVL (Link t, Item it)
{
	if (t == NULL) {
		return newNode (it);
	}
	int diff = cmp (key (it), key (t->value));
	t->within->ncompares++;
	if (diff == 0)     t->value = it;
	else if (diff < 0) t->left  = insertAVL (t->left, it);
	else if (diff > 0) t->right = insertAVL (t->right, it);

	int dL = depth (t->left);
	int dR = depth (t->right);
	if ((dL - dR) > 1) t = rotateR (t);
	if ((dR - dL) > 1) t = rotateL (t);


	return t;
}

static Link insertAVLwithURL (Link t, Item it, char *filename)
{
	if (t == NULL) {
		return newNodeWithUrl (it, filename);
	}
	//t->within->ncompares++;
	if (strcmp(t->value, it) == 0) {
		t->value = it;
		//if url is not already in the list, insert it
		if ((urlExist(t->url, filename)) == false) {
			//insertinorder
			DLListInsertInOrder(t->url, filename);
		}
	}
	else if (strcmp(t->value, it) > 0) t->left  = insertAVLwithURL (t->left, it, filename);
	else if (strcmp(t->value, it) < 0) t->right = insertAVLwithURL (t->right, it, filename);

	int dL = depth (t->left);
	int dR = depth (t->right);
	if ((dL - dR) > 1) t = rotateR (t);
	if ((dR - dL) > 1) t = rotateL (t);


	return t;
}

bool urlExist(DLList l, char* url) {
	l->curr = l->first;
	while (l->curr != NULL) {
		if (strcmp(l->curr->value, url) == 0)
			return true;
		l->curr = l->curr->next;
	}
	return false;
}

/** url into correct place in a sorted IntList. */
void DLListInsertInOrder (DLList L, char *url)
{
	L->curr = L->first;
	DLListNode *new = newDLListNode(url);

	//if list is empty
	if (L->first == NULL) {
		L->first = newDLListNode(url);
		L->nitems++;
	// insert at beginning
	} else if (strcmp(L->first->value, new->value) > 0) {
		new->next = L->first;
		new->next->prev = new;
		L->first = new;
		L->nitems++;
	} else {
		while (L->curr->next != NULL && strcmp(L->curr->next->value, new->value) < 0) {
			L->curr = L->curr->next;
		}
		new->next = L->curr->next;

		if (L->curr->next != NULL)
			new->next->prev = new;

		L->curr->next = new;
		new->prev = L->curr;
		
		L->nitems++;
	}
}

// Interface: check whether a value is in a Tree
int TreeFind (Tree t, Key k)
{
	Link res;
	int found = 0;
	if (t->insert == InsertSplay) {
		t->root = searchSplay (t->root, k, &found);
		res = found ? t->root : NULL;
	} else
		res = search (t->root, k);
	return (res != NULL);
}

// Helpers: search functions to return Node containing key
static Link search (Link t, Key k)
{
	if (t == NULL)
		return NULL;
	Link res = NULL;
	int diff = cmp (k, t->value);
	t->within->ncompares++;
	if (diff == 0)
		res = t;
	else if (diff < 0)
		res = search (t->left, k);
	else if (diff > 0)
		res = search (t->right, k);
	return res;
}

static Link searchSplay (Link t, Key k, int *found)
{
	Link res;
	if (t == NULL) {
		// item not found
		*found = 0;
		res = NULL;
	} else if (eq (key (t->value), k)) {
		*found = 1; // item found, store true
		t->within->ncompares++;
		res = t;
	} else if (less (k, key (t->value))) {
		t->within->ncompares++;
		if (t->left == NULL) {
			*found = 0; // item not found
			// res = rotateRight(t);
			res = t;
		} else if (eq (key (t->left->value), k)) {
			t->within->ncompares++;
			*found = 1;
			res = rotateR (t);
		} else {
			t->within->ncompares++;
			if (less (k, key (t->left->value))) {
				t->within->ncompares++;
				// left-left
				t->left->left = searchSplay (t->left->left, k, found);
				t = rotateR (t);
			} else {
				t->within->ncompares++;
				// left-right
				t->left->right = searchSplay (t->left->right, k, found);
				t->left = rotateL (t->left);
			}
			res = rotateR (t);
		}
	} else { // k > key(t->value)
		t->within->ncompares++;
		if (t->right == NULL) {
			*found = 0; // item not found
			// res = rotateLeft(t);
			res = t;
		} else if (eq (key (t->right->value), k)) {
			t->within->ncompares++;
			*found = 1;
			res = rotateL (t);
		} else {
			if (less (key (t->right->value), k)) {
				/* right-right */
				t->within->ncompares++;
				t->right->right =
					searchSplay (t->right->right, k, found);
				t = rotateL (t);
			} else {
				/* right-left */
				t->within->ncompares++;
				t->right->left = searchSplay (t->right->left, k, found);
				t->right = rotateR (t->right);
			}
			res = rotateL (t);
		}
	}
	return res;
}

// Interface: delete a value from a Tree
void TreeDelete (Tree t, Key k)
{
	t->root = delete (t->root, k);
}

// Helper: recursive delete
static Link delete (Link t, Key k)
{
	if (t == NULL)
		return NULL;
	int diff = cmp (k, t->value);
	t->within->ncompares++;
	if (diff == 0)
		t = deleteRoot (t);
	else if (diff < 0)
		t->left = delete (t->left, k);
	else if (diff > 0)
		t->right = delete (t->right, k);
	return t;
}

// Helper: delete root of tree
static Link deleteRoot (Link t)
{
	Link newRoot;
	// if no subtrees, tree empty after delete
	if (t->left == NULL && t->right == NULL) {
		free (t);
		return NULL;
	}
	// if only right subtree, make it the new root
	else if (t->left == NULL && t->right != NULL) {
		newRoot = t->right;
		free (t);
		return newRoot;
	}
	// if only left subtree, make it the new root
	else if (t->left != NULL && t->right == NULL) {
		newRoot = t->left;
		free (t);
		return newRoot;
	}
	// else (t->left != NULL && t->right != NULL)
	// so has two subtrees
	// - find inorder successor (grab value)
	// - delete inorder successor node
	// - move its value to root
	Link succ = t->right; // not null!
	while (succ->left != NULL) {
		succ = succ->left;
	}
	Key succVal = succ->value;
	t = delete (t, succVal);
	t->value = succVal;
	return t;
}

// Helper: rebalance tree by moving median to root
static Link rebalance (Link t)
{
	if (t == NULL) return NULL;
	if (size (t) < 2) return t;
	// put node with median key at root
	t = partition (t, size (t) / 2);
	// then rebalance each subtree
	t->left = rebalance (t->left);
	t->right = rebalance (t->right);
	return t;
}

// Helper: move i'th element to root
static Link partition (Link t, int i)
{
	if (t == NULL) return NULL;
	assert (0 <= i && i < size (t));
	int n = size (t->left);
	if (i < n) {
		t->left = partition (t->left, i);
		t = rotateR (t);
	}
	if (i > n) {
		t->right = partition (t->right, i - n - 1);
		t = rotateL (t);
	}
	return t;
}

// Helper: rotate tree left around root
static Link rotateL (Link n2)
{
	//empty node
	if (n2 == NULL) return NULL;
	//create temp link (child)
	Link n1 = n2->right;
	//empty child
	if (n1 == NULL) return n2;
	//make left of child equal to the right of ex-root
	n2->right = n1->left;
	//make the previous root the new child
	n1->left = n2;
	//n2->within->nrotates++;
	return n1;
}

// Helper: rotate tree right around root
static Link rotateR (Link n1)
{
	if (n1 == NULL) return NULL;
	Link n2 = n1->left;
	if (n2 == NULL) return n1;
	n1->left = n2->right;
	n2->right = n1;
	//n1->within->nrotates++;
	return n2;
}

////////////////////////////////////////////////////////////////////////

// ASCII tree printer
// Courtesy: ponnada
// Via: http://www.openasthra.com/c-tidbits/printing-binary-trees-in-ascii/

// data structures
typedef struct asciinode_struct asciinode;
struct asciinode_struct {
	asciinode *left, *right;
	// length of the edge from this node to its children
	int edge_length;
	int height;
	int lablen;
	//-1=I am left, 0=I am root, 1=I am right
	int parent_dir;
	// max supported unit32 in dec, 10 digits max
	char label[11];
};

// functions
static void print_level (asciinode *node, int x, int level);
static void compute_edge_lengths (asciinode *node);
static void compute_lprofile (asciinode *node, int x, int y);
static void compute_rprofile (asciinode *node, int x, int y);
static asciinode *build_ascii_tree (Link t);
static void free_ascii_tree (asciinode *node);

#define MAX_HEIGHT 10000
static int lprofile[MAX_HEIGHT];
static int rprofile[MAX_HEIGHT];
#define INFINITY (1 << 20)

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

static int gap = 3; // gap between left and right nodes

// used for printing next node in the same level,
// this is the x coordinate of the next char printed
static int print_next;

// prints ascii tree for given Tree structure
static void doShowTree (Link t)
{
	asciinode *proot;
	int xmin, i;
	if (t == NULL)
		return;
	proot = build_ascii_tree (t);
	compute_edge_lengths (proot);
	for (i = 0; i < proot->height && i < MAX_HEIGHT; i++)
		lprofile[i] = INFINITY;
	compute_lprofile (proot, 0, 0);
	xmin = 0;
	for (i = 0; i < proot->height && i < MAX_HEIGHT; i++)
		xmin = MIN (xmin, lprofile[i]);
	for (i = 0; i < proot->height; i++) {
		print_next = 0;
		print_level (proot, -xmin, i);
		printf ("\n");
	}
	if (proot->height >= MAX_HEIGHT) {
		printf (
			"(Tree is taller than %d; may be drawn incorrectly.)\n",
			MAX_HEIGHT);
	}
	free_ascii_tree (proot);
}

// This function prints the given level of the given tree, assuming
// that the node has the given x cordinate.
static void print_level (asciinode *node, int x, int level)
{
	int i, isleft;
	if (node == NULL)
		return;
	isleft = (node->parent_dir == -1);
	if (level == 0) {
		for (i = 0;
			 i < (x - print_next - ((node->lablen - isleft) / 2));
			 i++)
			printf (" ");
		print_next += i;
		printf ("%s", node->label);
		print_next += node->lablen;
	} else if (node->edge_length >= level) {
		if (node->left != NULL) {
			for (i = 0; i < (x - print_next - (level)); i++) {
				printf (" ");
			}
			print_next += i;
			printf ("/");
			print_next++;
		}
		if (node->right != NULL) {
			for (i = 0; i < (x - print_next + (level)); i++) {
				printf (" ");
			}
			print_next += i;
			printf ("\\");
			print_next++;
		}
	} else {
		print_level (
			node->left,
			x - node->edge_length - 1,
			level - node->edge_length - 1);
		print_level (
			node->right,
			x + node->edge_length + 1,
			level - node->edge_length - 1);
	}
}

// This function fills in the edge_length and
// height fields of the specified tree
static void compute_edge_lengths (asciinode *node)
{
	int h, hmin, i, delta;
	if (node == NULL)
		return;
	compute_edge_lengths (node->left);
	compute_edge_lengths (node->right);

	/* first fill in the edge_length of node */
	if (node->right == NULL && node->left == NULL)
		node->edge_length = 0;
	else {
		if (node->left == NULL)
			hmin = 0;
		else {
			for (i = 0; i < node->left->height && i < MAX_HEIGHT; i++)
				rprofile[i] = -INFINITY;
			compute_rprofile (node->left, 0, 0);
			hmin = node->left->height;
		}
		if (node->right == NULL)
			hmin = 0;
		else {
			for (i = 0; i < node->right->height && i < MAX_HEIGHT; i++)
				lprofile[i] = INFINITY;
			compute_lprofile (node->right, 0, 0);
			hmin = MIN (node->right->height, hmin);
		}
		delta = 4;
		for (i = 0; i < hmin; i++) {
			int w = gap + 1 + rprofile[i] - lprofile[i];
			delta = (delta > w) ? delta : w;
		}

		// If the node has two children of height 1, then we allow the
		// two leaves to be within 1, instead of 2
		if (((node->left != NULL && node->left->height == 1) ||
			 (node->right != NULL && node->right->height == 1)) &&
			delta > 4)
			delta--;
		node->edge_length = ((delta + 1) / 2) - 1;
	}

	// now fill in the height of node
	h = 1;
	if (node->left != NULL)
		h = MAX (node->left->height + node->edge_length + 1, h);
	if (node->right != NULL)
		h = MAX (node->right->height + node->edge_length + 1, h);
	node->height = h;
}

static asciinode *build_ascii_tree_recursive (Link t)
{
	asciinode *node;

	if (t == NULL)
		return NULL;
	node = malloc (sizeof (asciinode));
	node->left = build_ascii_tree_recursive (t->left);
	node->right = build_ascii_tree_recursive (t->right);
	if (node->left != NULL)
		node->left->parent_dir = -1;
	if (node->right != NULL)
		node->right->parent_dir = 1;
	sprintf (node->label, "%s", t->value);
	node->lablen = (int) strlen (node->label);
	return node;
}

// Copy the tree into the ascii node structre
static asciinode *build_ascii_tree (Link t)
{
	asciinode *node;
	if (t == NULL)
		return NULL;
	node = build_ascii_tree_recursive (t);
	node->parent_dir = 0;
	return node;
}

// Free all the nodes of the given tree
static void free_ascii_tree (asciinode *node)
{
	if (node == NULL)
		return;
	free_ascii_tree (node->left);
	free_ascii_tree (node->right);
	free (node);
}

// The following function fills in the lprofile array for the given
// tree. It assumes that the center of the label of the root of this tree
// is located at a position (x,y).  It assumes that the edge_length
// fields have been computed for this tree.
static void compute_lprofile (asciinode *node, int x, int y)
{
	int i, isleft;
	if (node == NULL)
		return;
	isleft = (node->parent_dir == -1);
	lprofile[y] = MIN (lprofile[y], x - ((node->lablen - isleft) / 2));
	if (node->left != NULL) {
		for (i = 1; i <= node->edge_length && y + i < MAX_HEIGHT; i++)
			lprofile[y + i] = MIN (lprofile[y + i], x - i);
	}
	compute_lprofile (
		node->left,
		x - node->edge_length - 1,
		y + node->edge_length + 1);
	compute_lprofile (
		node->right,
		x + node->edge_length + 1,
		y + node->edge_length + 1);
}

static void compute_rprofile (asciinode *node, int x, int y)
{
	int i, notleft;
	if (node == NULL)
		return;
	notleft = (node->parent_dir != -1);
	rprofile[y] = MAX (rprofile[y], x + ((node->lablen - notleft) / 2));
	if (node->right != NULL) {
		for (i = 1; i <= node->edge_length && y + i < MAX_HEIGHT; i++)
			rprofile[y + i] = MAX (rprofile[y + i], x + i);
	}
	compute_rprofile (
		node->left,
		x - node->edge_length - 1,
		y + node->edge_length + 1);
	compute_rprofile (
		node->right,
		x + node->edge_length + 1,
		y + node->edge_length + 1);
}

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
//empty list??
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
///note:: need to change the case for no nodes///
void DLListDelete (DLList L)
{
	assert (L != NULL);
	DLListNode *new = L->curr;
	//empty list
	if (L->first == NULL) {
		return;
	} else if (L->first == L->last) {
	//if only one element is left
		L->nitems--;
		L->first = NULL;
		//free(L);
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

/* print DLList. Assument File is open for writing */
void DLListPrint (DLList L, FILE *destination)
{
	assert (L != NULL);
	L->curr = L->first;
	for (; L->curr != NULL; L->curr = L->curr->next)
		fprintf (destination, "%s ", L->curr->value);
	fprintf(destination, "\n");
}

