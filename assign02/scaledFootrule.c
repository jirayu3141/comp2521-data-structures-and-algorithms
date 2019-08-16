//part 3
//written by Jirayu Sirivorawong

#include <stdio.h>
#include <stdlib.h>

//#include "readData.h"
#include "footedRuleSet.h"
#include "IntList.h"
#define MAX_FILE 50
//functions prototypes

void putIntoSet (set, char *filaname);
double calcualteResult (set);
void searchmin(setLink curr, IntList Possible_P, IntList PinUrl) ;
int main (int argc, char *argv[]) 
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s file1 file2 ...\n", argv[0]);
        return EXIT_FAILURE;
    }

    //put all the urls into set
    set url = newSet();
    //printf("%s", argv[1]);
    for (int i = 1; i < argc; i++) {
        putIntoSet(url, argv[i]); 
    }

    double result = calcualteResult(url);
    showSet(url);


    //find minimum weight sum
    return EXIT_SUCCESS;
}


void putIntoSet(set s, char *filename) {
    FILE *fptr = fopen(filename, "r");
    if (fptr == NULL) {
        printf ("error opening file: %s", filename);
        exit(EXIT_FAILURE);
    }
    char temp[MAX_FILE];

    int index = 1;
    while (fscanf(fptr, "%s", temp) != EOF) {
        insertInto(s, temp, index);
        index++;
    }
    fclose(fptr);
}

//calculate result
double calculateResult (set s) {
    double result = 0;
    //create possible position P
    IntList Possible_P = newIntList();
    for (int i = i; i <= nElems(s); i++) {
        IntListInsert(Possible_P, i);
    }

    //flag if there is no other change to the result
    int finished = false;

    //loop through each of the elememnt in the set to calulate min value
    setLink curr = s->elems;
    while (curr != NULL) {
        //if min is found, eliminate from the Possible P
        searchmin(curr, Possible_P, curr->P);
        curr = curr->next;
    }

    return result;
}

//if min is found, eliminate from the Possible P
void searchmin(setLink curr, IntList Possible_P, IntList PinUrl) {
    double min = 0;
    double result = 0;
    for (int i = 0; i < IntListLength(Possible_P); i++) {
        if (min < result) 
            min = result;
    }
}