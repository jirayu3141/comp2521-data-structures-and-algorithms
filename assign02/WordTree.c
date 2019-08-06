#define MAX 1000
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

// // insert a new value into a BSTree
// InvertedIndexBST BSTreeInsert(InvertedIndexBST t, char *word, char *filename, int numWords) {
//     if (t == NULL)
//         return newBSTNode(word, filename, numWords);
//     else if (strcmp(word, t->word) < 0)
//         t->left = BSTreeInsert(t->left, word, filename, numWords);
//     else if (strcmp(word, t->word) > 0)
//         t->right = BSTreeInsert(t->right, word, filename, numWords);
//     else {
//         //add the newFileList
//         int i = addFileToList(t->fileList, filename, numWords);
//         if ( i == 1) {
//         }
//         //update tf if it is an existing file
//         updateTF(t, filename, numWords);
//     }
//     return t;
// }

// make a new node containing a value
InvertedIndexBST newBSTNode(char *word, char *filename, int numWords) 
{
	InvertedIndexBST new = malloc(sizeof(struct InvertedIndexNode));
    if (new == NULL) err(EX_OSERR, "couldn't allocate BST node");
    new->word = malloc(strlen(word) + 1);
    if (new->word == NULL) err(EX_OSERR, "couldn't allocate word in BST node");
    strcpy(new->word, word);
    new->left = new->right = NULL;
    new->fileList = createNewNode(filename, numWords);
    return new;
}

InvertedIndexBST BSTreeInsertDuplicate(InvertedIndexBST t, char *word){
  printf("the duplicate word is: %s\n", word);
  //update tf of the word
  //update the file that contiains the word
  return t;
}

int addFileToList (FileList l, char *filename, int numWords) {
    int flag = 0;
    if (!(isInList(l, filename))) {
        FileList new = createNewNode(filename, numWords);
        new->tf = 0;
        FileList curr = l;
        //insert head
        if (strcmp(filename, curr->filename) < 0) {
            new->next = l;
            l = new;
        //1 element
        } else if (strcmp(filename, curr->filename) > 0 && curr->next == NULL) {
            l->next = new;
        } else {
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
        flag = 1;
    }
    return flag;
}

int isInList(FileList l, char *filename) {
    FileList curr = l;
    while (curr != NULL) {
        if (strcmp(filename, curr->filename) == 0)
            return 1;
        curr = curr->next;
    }
    return 0;
}

FileList createNewNode(char *filename, int totalWord) {
    struct FileListNode *new = malloc(sizeof(struct FileListNode));
    if (new == NULL) err (EX_OSERR, "couldn't allocated fileList");
    new->filename = malloc(strlen(filename)+ 1);
    strcpy(new->filename, filename);
    //points next to null
    new->next = NULL;
    //update the inital tf
    new->tf = (double)1/(double)totalWord;
    return new;
}

void updateTF(InvertedIndexBST t, char*filename, int numWords) {

    //find which filelist it needs to adjust
    FileList curr = t->fileList;
    while(curr != NULL) {
        if (strcmp(filename, curr->filename) == 0) {
            curr->tf += 1/(double)numWords;
            break;
        }
        curr = curr->next;
    }
    
}
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

FileList newFileNode(char *inputfile)
{
  FileList new = malloc(sizeof(struct FileListNode));
  assert(new != NULL);
  new->filename = malloc(strlen(inputfile)*sizeof(char) + 1);
  strcpy(new->filename, inputfile);
  new->tf = 0;
  new->next = NULL;
  return new;
}

//Calculates the tf of a specific word within a specific file
double calculatetf(char *file, char *word){
  double tf = 0;
  double wordcount = 0;
  double wordpresent = 0;
  FILE *fff;
  char infileword[MAX];
  if ((fff = fopen(file,"r")) == NULL) {
      fprintf(stderr, "Can't open file %s\n", file);
      return EXIT_FAILURE;
  }
  rewind(fff);
  while(fscanf(fff, "%s", infileword) != EOF){
    if(strcmp(normaliseWord(infileword), word) == 0){
      wordpresent++;
    }
    wordcount++;
  }
  fclose(fff);
  tf = wordpresent/wordcount;
  return tf;
}

//Adds a filenode to a word node
void addFileNode(InvertedIndexBST indexWordNode, FileList fileNode)
{
  if(indexWordNode->fileList == NULL){
    indexWordNode->fileList = fileNode;
  }
  else{
    fileNodeInsert(indexWordNode, fileNode);
  }
}
