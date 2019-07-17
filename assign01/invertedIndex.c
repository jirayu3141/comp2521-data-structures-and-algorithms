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

//degug
double calculatetf(char *file, char *word);
void addFileNode(InvertedIndexBST indexWordNode, FileList fileNode);
FileList newFileNode(char *inputfile);
void fileNodeInsert(InvertedIndexBST indexWordNode, FileList fileNode);
void infix(InvertedIndexBST tree, FILE *output);
void showBSTreeNodeandList(InvertedIndexBST t, FILE *output);
TfIdfList addsingleTfIdfNode(TfIdfList addonto, char *file, double tf, int D, int numoffiles);
TfIdfList checktfIdfNode(TfIdfList t, char * filecheck);
TfIdfList sorttdidf(TfIdfList orginal, TfIdfList addonto);
double calcsingleTfIdfsum(double tf, int D, double numoffiles);
TfIdfList duplicatetdidfnode(TfIdfList original);

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
            WordInsert(wordTree, buffer[j]);
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

TfIdfList calculateTfIdf(InvertedIndexBST tree, char *searchWord , int D) {
      InvertedIndexBST wordNode = BSTreeFind(tree, searchWord);
  if(wordNode == NULL){
    return NULL;
  }
  FileList itterativecounter = wordNode->fileList;
  double filecounter = 0;
  while(itterativecounter!=NULL){
      filecounter++;
      itterativecounter = itterativecounter->next;
  }

  if(wordNode == NULL){
    return NULL;
  }
  else{
    TfIdfList returnvalue = NULL;
    FileList increment = wordNode->fileList;
    while(increment != NULL){
      returnvalue = addsingleTfIdfNode(returnvalue, increment->filename, increment->tf, D, filecounter);
      increment = increment->next;
    }
    TfIdfList sorted = NULL;
    sorted= sorttdidf(returnvalue, sorted);
    return sorted;
  }
}
TfIdfList retrieve(InvertedIndexBST tree, char* searchWords[] , int D) {
int num_of_search_words = 0;
    while(searchWords[num_of_search_words] != NULL){
        num_of_search_words++;
    }

    TfIdfList retrievetfidflist = NULL;

    int counter = 0;
    while(counter<num_of_search_words){
        InvertedIndexBST word = BSTreeFind(tree, searchWords[counter]);
        if(word != NULL){
            FileList itterativecounter = word->fileList;
            double filecount = 0;
            while(itterativecounter!=NULL){
                filecount++;
                itterativecounter = itterativecounter->next;
            }

            FileList increment = word->fileList;
            while(increment != NULL){
                if(checktfIdfNode(retrievetfidflist, increment->filename) == NULL){
                    retrievetfidflist = addsingleTfIdfNode(retrievetfidflist, increment->filename, increment->tf, D, filecount);
                }
                else{
                    TfIdfList temp = checktfIdfNode(retrievetfidflist, increment->filename);
                    double addition = calcsingleTfIdfsum(increment->tf, D, filecount);
                    temp->tfidf_sum += addition;
                }
                increment = increment->next;
            }
      }
        counter++;
    }


    TfIdfList sorted = NULL;
    sorted= sorttdidf(retrievetfidflist, sorted);
    return sorted;
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

//Calculates the tfidf of a specific tfidf node
double calcsingleTfIdfsum(double tf, int D, double numoffiles){
  double tfidfsum = 0;
  tfidfsum = tf*log10(D/numoffiles);
  return tfidfsum;
}
//Sorts out a tfidf list in order of descending tfidf sum
//However if tfidf is equal then sort in ascending alphabetically
TfIdfList sorttdidf(TfIdfList original, TfIdfList addonto){

    TfIdfList originalpointer = original;
    TfIdfList itterativepointer = addonto;
    while(originalpointer != NULL){
        TfIdfList newtfidfnode = duplicatetdidfnode(originalpointer);
        if(itterativepointer == NULL){
            addonto = newtfidfnode;
        }
        else{
            if(newtfidfnode->tfidf_sum>itterativepointer->tfidf_sum) {
                newtfidfnode->next = itterativepointer;
                addonto = newtfidfnode;
            }
            else if(newtfidfnode->tfidf_sum==itterativepointer->tfidf_sum){
                if(strcmp(newtfidfnode->filename, itterativepointer->filename) > 0){
                    itterativepointer->next = newtfidfnode;
                }
                else{
                  newtfidfnode->next = itterativepointer;
                  addonto = newtfidfnode;
                }
            }
            else{
                while(newtfidfnode->tfidf_sum <= itterativepointer->tfidf_sum){
                  if(itterativepointer->next == NULL){
                    itterativepointer->next = newtfidfnode;
                    break;
                  }
                  else if(newtfidfnode->tfidf_sum>itterativepointer->next->tfidf_sum){
                    newtfidfnode->next = itterativepointer->next;
                    itterativepointer->next = newtfidfnode;
                    break;
                  }
                  else if(newtfidfnode->tfidf_sum==itterativepointer->next->tfidf_sum){
                    if(strcmp(newtfidfnode->filename, itterativepointer->next->filename) > 0){
                      itterativepointer = itterativepointer->next;
                    }
                    else{
                      newtfidfnode->next = itterativepointer->next;
                      itterativepointer->next = newtfidfnode;
                      break;
                    }
                  }
                  else{
                    itterativepointer = itterativepointer->next;
                  }
                }
            }
        }
        itterativepointer = addonto;
        originalpointer = originalpointer->next;
    }
    return addonto;
}

//Duplicates a tfidf node to allow it to be sorted
TfIdfList duplicatetdidfnode(TfIdfList original){
  TfIdfList replicate = malloc(sizeof(struct TfIdfNode));
  assert(replicate != NULL);
  replicate->filename = malloc(strlen(original->filename)*sizeof(char));
  strcpy(replicate->filename, original->filename);
  replicate->tfidf_sum = original->tfidf_sum;
  replicate->next = NULL;
  return replicate;

}

//Checks if a tdidf node with the same filename already exist within
//the tdidf list that is to be returned
//returns the node if exist or else returns NULL
TfIdfList checktfIdfNode(TfIdfList t, char * filecheck){
    while(t != NULL){
        if(strcmp(t->filename, filecheck) == 0){
            return t;
        }
        t = t->next;
    }
    return NULL;
}
