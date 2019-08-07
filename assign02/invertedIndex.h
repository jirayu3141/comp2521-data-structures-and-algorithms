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

/** The function takes the file, Read Secion 2 and build a tree from it (note: url don't need to include .txt, the function takes care of it) */
Tree buildTreeFromFile(Tree wordTree, char *url);

/** The function output a give inverted index tree to a file named
    invertedIndex.txt. One line per word, words arealphabetically ordered,
    using ascending order. Each list of filenames (for a single word) are 
    alphabetically ordered, using ascending order.
*/
void printInvertedIndex(Tree tree);

void showWordAndUrl (Link t, FILE *output);

#endif