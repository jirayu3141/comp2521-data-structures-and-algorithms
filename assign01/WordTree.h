//writen by Jirayu Sirivorawong, June 2019
#ifndef WORDTREE
#define WORDTREE

#include "invertedIndex.h"

//insert new word to the tree
InvertedIndexBST WordInsert(InvertedIndexBST, char*);

//create new node of the tree
InvertedIndexBST newNode (char*);

//print the values in tree
void BSTreeInfix(InvertedIndexBST t);

//helper funciton to print value
void showBSTreeNode(InvertedIndexBST t);

//create new tree
InvertedIndexBST newBSTree(void);

//find the if the word exists in tree
InvertedIndexBST BSTreeFind(InvertedIndexBST t, char *word);

#endif