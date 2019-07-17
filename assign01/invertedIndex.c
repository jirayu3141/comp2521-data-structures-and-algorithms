// Written by Jirayu Sirivorawong, June 2019
#include "invertedIndex.h"
//#include "WordTree.h"

#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <string.h>
#include <ctype.h>

#define MAX_FILE 1000
#define BUF_SIZE 1000

/*FUNCTION PROTOTYPE */
void removeSpaces(char *str);
char* ReadFile(char *filename);
int storeTokenToArray(char *content, char**);
//tree functions
static InvertedIndexBST WordInsert(InvertedIndexBST, char*);
InvertedIndexBST newNode (char*);

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
    InvertedIndexBST wordTree;
    
    //collection_content has the file contents
    char *collection_content = ReadFile(collectionFilename);
    char *file_names[FILENAME_MAX];
    //numFile is the totol number of files in collectionFilename
    int numFile = storeTokenToArray(collection_content, file_names);
   
    //lets test with one file first
    //store the text into a temp array
    char *all_words_in_file = ReadFile(file_names[0]);
    //array to store each word
    char* buffer[BUF_SIZE];
    int numWords = storeTokenToArray(all_words_in_file, buffer);

    //put all the words into tree
    for (int i = 0; i < numWords; i++) {
        wordTree = WordInsert(wordTree, buffer[i]);
    }
    //if duplicate is found, change the tf
    for (int i = 0; i < numWords; i++)
        printf("%s\n", buffer[i]);


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

   if (handler)
   {
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
    }

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

/*TREE FUNCTIONS*/

static InvertedIndexBST WordInsert(InvertedIndexBST t, char* word) {
    if (t == NULL) return newNode (word);   //crete new tree with word in it
    if (strcmp(word, t->word) < 0) {
        t->left = WordInsert(t->left, word);
    } else if (strcmp (word, t->word) > 0) {
        t->right = WordInsert(t->right, word);
    }
    return t;
}
InvertedIndexBST newNode (char* word) {
    struct InvertedIndexNode *new = malloc(sizeof *new);
    if (new == NULL) err (EX_OSERR, "couldn't allocate Tree node");
    new->fileList = NULL;
    new->left = NULL;
    new->right = NULL;
    new->word = word;
    return new;
}