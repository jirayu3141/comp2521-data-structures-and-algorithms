#ifndef _INVERTEDINDEX_GUARD
#define _INVERTEDINDEX_GUARD

#include "WordTree.h"

/** The function reads a given file with collection of file names, 
    read each of these files, generate inverted index and return the inverted index.
*/
Tree generateInvertedIndex(char *collectionFilename);


/** The function output a give inverted index tree to a file named 
    invertedIndex.txt. One line per word, words are alphabetically ordered, 
    using ascending order.
*/
void printInvertedIndex(Tree tree);


#endif