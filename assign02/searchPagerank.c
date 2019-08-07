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
#include <ctype.h>
#include <math.h>

#define MAX_OUTPUT 50
typedef struct result {
	char *val;
	int wordScore;
	double page_rank_score;
} Result;

///FUNCTION PROTOTYPE///
void insertUrlInto(char *filename, Set s, char *word);
void formatWordScore(Set searchResult);
int cmpfunc (const void *a, const void *b);
int main (int argc, char *argv[]) {
    // /* BUILD AN AVL TREE FROM COLLECTION.TXT */
    /*NOTE: IF invertedIndex.txt IS NOT GIVEN, THESE FUNCTION READS COLLECTION.TXT AND MAKE THE FILE OUT OF IT. */

    /* 
    GetCollection();
    Tree invertedTree = generateInvertedIndex("collection.txt");

    printInvertedIndex(invertedTree);
    */

    if (argc == 1) {
        printf("usage %s: Search Word\n", argv[0]);
    }   
    //create a set to store the search terms and informaion related to the search term
    Set searchResult = newSet();
    
    //find a resulted url and put it into set
    //note: set.c is modified to insert based on Page Rank

    for (int i = 1; i < argc; i++) {
        insertUrlInto("invertedIndex.txt" ,searchResult, argv[i]);
    }
    //formatted result
    formatWordScore(searchResult);

}

//this function takes the searched word "url" into a Set
void insertUrlInto(char *filename, Set s, char *word) {
    // extract searh url into array
	char **urls = extractSearhUrl(filename, word);
	for (int i = 0; urls[i] != NULL; i++) {
		//for each of the urls found, put it into set
        insertInto(s, urls[i]);
        
    }

}

void formatWordScore(Set searchResult) {
    int nResult = nElems(searchResult);
    //initialize output
    Result output[MAX_FILE];
    for (int i = 0; i < nResult; i++) {
        output[i].val = malloc(MAX_URL);
    }
    SetLink curr = searchResult->elems;
    //copy set result into sturct output
    int j = 0;
    while (curr != NULL) {
        strcpy(output[j].val , curr->val);
        output[j].wordScore = curr->wordScore;
        output[j].page_rank_score = curr->page_rank_score;
        curr = curr->next;
        j++;
    }
    
    qsort(output, nResult, sizeof(Result), cmpfunc);
    for (int i = 0; i < nResult && i < MAX_OUTPUT; i++) {
        printf("%s\n", output[i].val);
        //printf("%s   word matched: %d   PageRank: %f\n", output[i].val, output[i].wordScore, output[i].page_rank_score);
    }
}

//compare function used for qsort
int cmpfunc (const void *a, const void *b) {
    Result *resultA = (Result *)a;
    Result *resultB = (Result *)b;
    return (resultB->wordScore - resultA->wordScore);
}