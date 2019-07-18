// Written by Jirayu Sirivorawong, June 2019
#include "invertedIndex.h"
#include "WordTree.h"

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
#define MAX_FILE 1000
#define BUF_SIZE 1000
#define MAX 1000

/*FUNCTION PROTOTYPE */
void removeSpaces(char *str);
char* ReadFile(char *filename);
int storeTokenToArray(char *content, char**);
int fileNodeExist(InvertedIndexBST, char *);


char * normaliseWord(char *str) 
{
    //removeing leading and trailing spaces
    removeSpaces(str);     

    //converting all charcters to lowercase
    for (int i = 0, j = strlen(str); i < j; i++) {
        if (isupper(str[i]))
            str[i] = tolower(str[i]);
    }
    //remove punctuation marks
    char *endPtr = str + strlen(str) - 1;
    if (*endPtr == '.' || *endPtr == ',' || *endPtr == ';'|| *endPtr == '?')
        *endPtr = '\0';
    
    return str;
}

InvertedIndexBST generateInvertedIndex(char *collectionFilename) 
{
    //create a new tree to store all words
    InvertedIndexBST wordTree = newBSTree();
    //collection_content has the file contents
    char *collection_content = ReadFile(collectionFilename);
    char *file_names[FILENAME_MAX];
    //numFile is the total number of files in collectionFilename
    int numFile = storeTokenToArray(collection_content, file_names);
    //looping through each file
    //for (int i = 0; i < numFile; i++) {
    for (int i = 0; i < 1; i++) {
        //store the text into a temp array
        char *all_words_in_file = ReadFile(file_names[i]);
        //array to store each word
        char* buffer[BUF_SIZE];
        int numWords = storeTokenToArray(all_words_in_file, buffer);
        //put all the words into tree
        
        for (int j = 0; j < numWords - 1; j++) {
            wordTree = BSTreeInsert(wordTree, buffer[j], file_names[i]);
            //update file list
            //update tf

            //printf("%s\n", buffer[j]);
        }
    }


    BSTreeInfix(wordTree);

    return wordTree;
}

/*OTERH FUNCTIONS*/
void removeSpaces(char *str) {
    int i;
    int begin = 0;
    int end = strlen(str) - 1;

    while (isspace((unsigned char) str[begin]))
        begin++;

    while ((end >= begin) && isspace((unsigned char) str[end]))
        end--;

    // Shift all characters back to the start of the string array.
    for (i = begin; i <= end; i++)
        str[i - begin] = str[i];

    str[i - begin] = '\0'; // Null terminate string.
}
char* ReadFile(char *filename)
{
    char *buffer = NULL;
    int string_size, read_size;
    FILE *handler = fopen(filename, "r");
    if(handler == NULL) {
        printf("Error!");          
    }
    // Seek the last byte of the file
    fseek(handler, 0, SEEK_END);
    // Offset from the first to the last byte, or in other words, filesize
    string_size = ftell(handler);
    // go back to the start of the file
    rewind(handler);

    // Allocate a string that can hold it all
    buffer = (char*) malloc(sizeof(char) * (string_size + 1) );

    // Read it all in one operation
    read_size = fread(buffer, sizeof(char), string_size, handler);

    // fread doesn't set it so put a \0 in the last position
    // and buffer is now officially a string
    buffer[string_size] = '\0';

    if (string_size != read_size)
    {
        // Something went wrong, throw away the memory and set
        // the buffer to NULL
        free(buffer);
        buffer = NULL;
    }

       // Always remember to close the file.
       fclose(handler);

    return buffer;
}
int storeTokenToArray(char *content, char *destination[]){
    char *array[MAX_FILE];
     //extract file names into array
    char *word;
    word = strtok(content, " ,   \n");
    int i = 0;
    while(word != NULL) {
        word = normaliseWord(word);
        array[i] = word;
        word = strtok(NULL, " ,   \n");
        i++;
    }
    //copy array
    for (int j = 0; j < i; j++) {
        destination[j] = array[j];
    }
    return i;
}