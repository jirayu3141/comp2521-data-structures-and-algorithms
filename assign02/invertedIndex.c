// Written by Jirayu Sirivorawong, June 2019
#include "invertedIndex.h"
#include "WordTree.h"
#include "readData.h"

#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>

#define MAX_WORD 5000

Tree generateInvertedIndex(char *collectionFilename) {
    //create a new tree
    Tree wordTree = newTree(InsertAVL);
    //buildTreeFromFile(wordTree, List_of_Urls[0]);
    for (int i = 0; i < numUrl; i++) {
        buildTreeFromFile(wordTree, List_of_Urls[i]);
    }
    //showTree(wordTree);
    return wordTree;
}

Tree buildTreeFromFile(Tree wordTree, char *url) {
    //read data from url (section 2)
    char *WordsInFile[MAX_WORD] = {'\0'};
    readSection2(url, WordsInFile);    
    //scan the word array and insert into a tree
    for (int i = 0; WordsInFile[i] != NULL; i++) {
        TreeInsertWithURL(wordTree, WordsInFile[i], url);
    }
    //if the word is did not exist, add url LL node
    return wordTree;
}


void printInvertedIndex(Tree t) {
    assert(t != NULL);
    FILE *output = fopen("invertedIndex.txt", "w");
    assert(output != NULL);

    Link root = t->root;
    showWordAndUrl (root, output);

    fclose(output);
}

void showWordAndUrl (Link t, FILE *output) {
    if (t == NULL) {
        return;
    }

    showWordAndUrl(t->left, output);
    fprintf(output,"%s ", t->value);
    DLListPrint(t->url, output);
    showWordAndUrl(t->right, output);
}