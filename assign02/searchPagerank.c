//Written by Jirayu Sirivorawong  July 2019
#include "set.h"
#include "pagerank.h"
#include "invertedIndex.h"
#include <assert.h>
#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <math.h>


///FUNCTION PROTOTYPE///

int main (int argc, char *argv[]) {
    // /* BUILD AN AVL TREE FROM COLLECTION.TXT */
    
    // GetCollection();
    // Tree invertedTree = generateInvertedIndex("collection.txt");

    // printInvertedIndex(invertedTree);

    //create a set
    Set searchResult = newSet();
    
    //find a resulted url and put it into set
    //insertUrlInto("invertedIndex.txt" ,searchResult, "mars");
    normaliseWord(" test");
    //insertUrlInto("invertedIndex.txt" ,searchResult, "design");


}

