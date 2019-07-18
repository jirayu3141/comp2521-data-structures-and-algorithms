
#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <string.h>

#include "WordTree.h"

struct FileListRep {
    struct FileListNode *first;
    struct FileListNode *last;
};


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
InvertedIndexBST BSTreeInsert(InvertedIndexBST t, char *word, char *filename) {
    if (t == NULL)
        return newBSTNode(word, filename);
    else if (strcmp(word, t->word) < 0)
        t->left = BSTreeInsert(t->left, word, filename);
    else if (strcmp(word, t->word) > 0)
        t->right = BSTreeInsert(t->right, word, filename);
    else {
        //insert duplicates
        //BSTreeInsertDuplicate(t, word);
        //update tf **
    }
    return t;
}

// make a new node containing a value
InvertedIndexBST newBSTNode(char *word, char *filename) 
{
	InvertedIndexBST new = malloc(sizeof(struct InvertedIndexNode));
    if (new == NULL) err(EX_OSERR, "couldn't allocate BST node");
    new->word = malloc(strlen(word) + 1);
    if (new->word == NULL) err(EX_OSERR, "couldn't allocate word in BST node");
    strcpy(new->word, word);
    new->left = new->right = NULL;
    new->fileList = addFileToList(new->fileList,filename);
    return new;
}

InvertedIndexBST BSTreeInsertDuplicate(InvertedIndexBST t, char *word){
  printf("the duplicate word is: %s\n", word);
  //update tf of the word
  //update the file that contiains the word
  return t;
}

FileList addFileToList (FileList l, char *filename) {
	//if file list does not exist, create filelist, and insert head
	if (l == NULL) {
		l = createNewNode(filename);
	} else {
        FileList curr = l;
        FileList new = createNewNode(filename);
        while (curr->next != NULL) {
            //create new node to store value
            //create a temporary pointer to the nodes in the list
            if (strcmp(filename, l->filename) < 0) {
                //insert before current node
                l->next = l;
                l = new;
            } else if (strcmp(filename, l->filename) > 0 && strcmp(filename, l->next->filename) < 0) {
                //in between
                new->next = curr->next;
                curr->next = new;
            } else if(strcmp(filename, l->next->filename) > 0 && curr->next->next == NULL) {
                //insert last
                curr->next->next = new;
            }
            curr = curr->next;
        }
    }
	return l;
}


FileList createNewNode(char *filename) {
    struct FileListNode *new = malloc(sizeof(struct FileListNode));
    if (new == NULL) err (EX_OSERR, "couldn't allocated fileList");
    new->filename = malloc(strlen(filename)+ 1);
    strcpy(new->filename, filename);
    return new;
}