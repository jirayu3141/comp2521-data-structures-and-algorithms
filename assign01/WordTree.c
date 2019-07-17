
#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <string.h>

#include "WordTree.h"

InvertedIndexBST WordInsert(InvertedIndexBST t, char* word) {
    if (t == NULL)
        return newNode (word);   //crete new tree with word in it
    else if (strcmp(word, t->word) < 0) {
        t->left = WordInsert(t->left, word);
    } else if (strcmp (word, t->word) > 0) {
        t->right = WordInsert(t->right, word);
    } else;
    return t;
}

InvertedIndexBST newNode (char* word) {
    InvertedIndexBST new = malloc(sizeof *new);
    if (new == NULL) err (EX_OSERR, "couldn't allocate BST node");
    new->fileList = NULL;
    new->left = new->right = NULL;
    //allocate space for word
    new->word = malloc(strlen(word) *sizeof(char) + 1);
    assert(new->word != NULL);
    strcpy(new->word, word);
    return new;
}

void BSTreeInfix(InvertedIndexBST t) {
    if (t == NULL) return;

    BSTreeInfix(t->left);
    showBSTreeNode(t);
    BSTreeInfix(t->right);
}

void showBSTreeNode(InvertedIndexBST t) {
    if (t == NULL) return;
    printf("%s\n", t->word);
}

InvertedIndexBST newBSTree(void) { return NULL; }

int fileNodeExist(InvertedIndexBST t, char *word) {
    FileList itterative = t->fileList;
    int returnvalue = 0;
    while (itterative != NULL) {
        if (strcmp(itterative->filename, word) == 0) {
            returnvalue = 1;
        }
        itterative = itterative->next;
    }
    return returnvalue;
}

InvertedIndexBST BSTreeFind(InvertedIndexBST t, char *inputword)
{
	if (t == NULL)
		return NULL;
	else if (strcmp(inputword, t->word) < 0)
		return BSTreeFind(t->left, inputword);
	else if (strcmp(inputword, t->word) > 0)
		return BSTreeFind(t->right, inputword);
	else // (v == t->value)
		return t;
}