//writen by Jirayu Sirivorawong, June 2019
#ifndef WORDTREE_H
#define WORDTREE_H

#include <stdio.h>
#include "invertedIndex.h"

// print values in infix order (refereced from lab03)
void BSTreeInfix(InvertedIndexBST tree);

void showBSTreeNode(InvertedIndexBST t);

// create a new empty BSTree(refereced from lab03)
InvertedIndexBST newBSTree(void);

InvertedIndexBST BSTreeInsert(InvertedIndexBST t, char *word);

// make a new node containing a value (lab03)
InvertedIndexBST newBSTNode(char *word);



#endif