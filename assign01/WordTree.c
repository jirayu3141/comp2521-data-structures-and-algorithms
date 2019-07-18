
#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <string.h>

#include "WordTree.h"

void BSTreeInfix(InvertedIndexBST tree) {
  if(tree == NULL){
    return;
  }
  BSTreeInfix(tree->left);
  showBSTreeNode(tree);
  BSTreeInfix(tree->right);
}

void showBSTreeNode(InvertedIndexBST t) {
    if (t == NULL) return;
    printf("%s\n", t->word);
}

InvertedIndexBST newBSTree(void) { return NULL; }

// insert a new value into a BSTree
InvertedIndexBST BSTreeInsert(InvertedIndexBST t, char *word) {
    if (t == NULL)
        return newBSTNode(word);
    else if (strcmp(t->word , word) < 0)
        t->left = BSTreeInsert(t->left, word);
    else if (strcmp(t->word , word) > 0)
        t->right = BSTreeInsert(t->right, word);
    else  // (v == t->value)
        /* don't insert duplicates */;
    return t;
}

// make a new node containing a value
InvertedIndexBST newBSTNode(char *word) {
    InvertedIndexBST new = malloc(sizeof *new);
    if (new == NULL) err(EX_OSERR, "couldn't allocate BST node");
    new->word = malloc(strlen(word) + 1);
    if (new->word == NULL) err(EX_OSERR, "couldn't allocate word in BST node");
    strcpy(new->word, word);
    new->left = new->right = NULL;
    return new;
}

