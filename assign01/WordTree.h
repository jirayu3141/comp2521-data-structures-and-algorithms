//writen by Jirayu Sirivorawong, June 2019
#ifndef WORDTREE_H
#define WORDTREE_H

#include "invertedIndex.h"

InvertedIndexBST WordInsert(InvertedIndexBST t, char* word);
InvertedIndexBST newNode (char*);
void BSTreeInfix(InvertedIndexBST t);
void showBSTreeNode(InvertedIndexBST t);
InvertedIndexBST newBSTree(void);
InvertedIndexBST BSTreeFind(InvertedIndexBST t, char *inputword);

#endif