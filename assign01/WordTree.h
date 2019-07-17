//writen by Jirayu Sirivorawong, June 2019
#ifndef WORDTREE
#define WORDTREE

#include "invertedIndex.h"

WordInsert(InvertedIndexBST, char*);
InvertedIndexBST newNode (char*);
void BSTreeInfix(InvertedIndexBST t);
void showBSTreeNode(InvertedIndexBST t);
InvertedIndexBST newBSTree(void);
InvertedIndexBST BSTreeFind(InvertedIndexBST t, char *inputword);

#endif