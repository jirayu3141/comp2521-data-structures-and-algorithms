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
double calTF(char *file_name, char *word);
void add_file_to_list(InvertedIndexBST index, FileList Node);
FileList newFileNode(char *file);
void fileNodeInsert(InvertedIndexBST index, FileList Node);
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
    
    //collection_content has the file_name contents
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
                newfn->tf = calTF(file_names[i], buffer[j]);
                add_file_to_list(BSTreeFind(wordTree, buffer[j]), newfn);
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
       // Seek the last byte of the file_name
       fseek(handler, 0, SEEK_END);
       // Offset from the first to the last byte, or in other words, filesize
       string_size = ftell(handler);
       // go back to the start of the file_name
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

       // Always remember to close the file_name.
       fclose(handler);
    }

    return buffer;
}
int storeTokenToArray(char *content, char *destination[]){
    char *array[MAX_FILE];
     //extract file_name names into array
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

//calculate tf of the word
double calTF(char *file_name, char *word){
  double tf = 0;
  double word_count = 0;
  double word_duplicate = 0;
  FILE *fp;
  char words_in_file[MAX_FILE];
  if ((fp = fopen(file_name,"r")) == NULL) {
      fprintf(stderr, "Can't open file_name %s\n", file_name);
      return EXIT_FAILURE;
  }
  rewind(fp);
  while(fscanf(fp, "%s", words_in_file) != EOF){
    if(strcmp(normaliseWord(words_in_file), word) == 0){
      word_duplicate++;
    }
    word_count++;
  }
  fclose(fp);
  tf = word_duplicate/word_count;
  return tf;
}

//this function add file name the the FileList
void add_file_to_list(InvertedIndexBST index, FileList Node)
{
    //if file is not in the list
  if(index->fileList == NULL)
        index->fileList = Node;
  else{
        fileNodeInsert(index, Node);
  }
}

FileList newFileNode(char *file)
{
  FileList new = malloc(sizeof(struct FileListNode));
  if (new == NULL) {
      printf("cant allocte space");
      exit(1);
  }
  new->filename = malloc(strlen(file)+ 1);
  strcpy(new->filename, file);
  new->next = NULL;
  new->tf = 0;
  return new;
}

void fileNodeInsert(InvertedIndexBST index, FileList Node){

  FileList itPtr = index->fileList;
  if (strcmp(itPtr->filename,  Node->filename) > 0) {
    Node->next = itPtr;
    index->fileList = Node;
    return;
  } else{
        while(strcmp(itPtr->filename,  Node->filename) < 0){
            if(itPtr->next == NULL){
            itPtr->next = Node;
        return;
      }
      else if(strcmp(itPtr->next->filename,  Node->filename) > 0){
            break;
      }
      else{
            itPtr = itPtr->next;
      }
    }
    Node->next = itPtr->next;
    itPtr->next = Node;
    return;
  }

}
//printing in infix (taken from lab030)
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
    FileList it = t->fileList;
    while(it != NULL) {
        fprintf(output, "%s ", it->filename);
        it = it->next;
    }
    fprintf(output, "\n");
}