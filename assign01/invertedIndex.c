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
#include <stdbool.h>
#define MAX_FILE 1000
#define BUF_SIZE 1000

/*FUNCTION PROTOTYPE */
void removeSpaces(char *str);
char* ReadFile(char *filename);
int storeTokenToArray(char *content, char**);
int fileNodeExist(InvertedIndexBST, char *);
//tree functions

//degug
double calculatetf(char *file, char *word);
void addFileNode(InvertedIndexBST indexWordNode, FileList fileNode);
FileList newFileNode(char *inputfile);
void fileNodeInsert(InvertedIndexBST indexWordNode, FileList fileNode);
void infix(InvertedIndexBST tree, FILE *output);
void showBSTreeNodeandList(InvertedIndexBST t, FILE *output);
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
    //numFile is the totol number of files in collectionFilename
    int numFile = storeTokenToArray(collection_content, file_names);
   

    for (int i = 0; i < numFile; i++) {
        //store the text into a temp array
        char *all_words_in_file = ReadFile(file_names[i]);
        //array to store each word
        char* buffer[BUF_SIZE];
        int numWords = storeTokenToArray(all_words_in_file, buffer);
        //put all the words into tree
        for (int j = 0; j < numWords; j++) {
            wordTree = WordInsert(wordTree, buffer[j]);
            //check if the filelist has been inserted
            int flag = fileNodeExist(BSTreeFind(wordTree, buffer[j]),file_names[i]);
            //put in the filelist
            if (flag == false) {
                FileList newfn = newFileNode(file_names[i]);
                newfn->tf = calculatetf(file_names[i], buffer[j]);
                addFileNode(BSTreeFind(wordTree, buffer[j]), newfn);
            }
            //FileList newFL = 
        }
    }
    //BSTreeInfix(wordTree);
    //if duplicate is found, change the tf
    // for (int i = 0; i < numWords; i++)
    //     printf("%s\n", buffer[i]);
    return wordTree;
}

void printInvertedIndex(InvertedIndexBST tree) {
    if(tree == NULL) return;
    FILE *output;
    output = fopen("invertedIndex.txt", "w");

    infix(tree, output);

    fclose(output);
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










double calculatetf(char *file, char *word){
  double tf = 0;
  double wordcount = 0;
  double wordpresent = 0;
  FILE *fff;
  char infileword[1000];
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

void addFileNode(InvertedIndexBST indexWordNode, FileList fileNode)
{
  if(indexWordNode->fileList == NULL){
    indexWordNode->fileList = fileNode;
  }
  else{
    fileNodeInsert(indexWordNode, fileNode);
  }
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

void fileNodeInsert(InvertedIndexBST indexWordNode, FileList fileNode){

  FileList itterativepointer = indexWordNode->fileList;
  if (strcmp(itterativepointer->filename,  fileNode->filename) > 0) {
    fileNode->next = itterativepointer;
    indexWordNode->fileList = fileNode;
    return;
  }
  else{
    while(strcmp(itterativepointer->filename,  fileNode->filename) < 0){
      if(itterativepointer->next == NULL){
        itterativepointer->next = fileNode;
        return;
      }
      else if(strcmp(itterativepointer->next->filename,  fileNode->filename) > 0){
        break;
      }
      else{
        itterativepointer = itterativepointer->next;
      }
    }
    fileNode->next = itterativepointer->next;
    itterativepointer->next = fileNode;
    return;
  }

}

void infix(InvertedIndexBST tree, FILE *output){
  if(tree == NULL){
    return;
  }
  infix(tree->left, output);
  showBSTreeNodeandList(tree, output);
  infix(tree->right, output);
}

void showBSTreeNodeandList(InvertedIndexBST t, FILE *output)
{
	if (t == NULL) return;
	fprintf(output, "%s ", t->word);
  FileList itterative = t->fileList;
  while(itterative != NULL){
    fprintf(output, "%s ", itterative->filename);
    itterative = itterative->next;
  }
  fprintf(output, "\n");
}