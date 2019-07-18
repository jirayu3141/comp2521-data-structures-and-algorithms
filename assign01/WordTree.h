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

//insert word into the tree.
InvertedIndexBST BSTreeInsert(InvertedIndexBST t, char *word, char *filename);

// make a new node containing a value (lab03)
InvertedIndexBST newBSTNode(char *word, char *filename);

//update TF of the word and add file to list if have not been added
InvertedIndexBST BSTreeInsertDuplicate(InvertedIndexBST t, char *word);

//add the file to filelist
FileList addFileToList (FileList l, char *filename);



FileList createNewNode(char *filename);

#endif 