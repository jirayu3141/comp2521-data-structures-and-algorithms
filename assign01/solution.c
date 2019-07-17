//z5208916 (Paul Tawanwad Kulthonthong) Assignment 1
//COMP2521 T2
//inverted index

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <libgen.h>
#include <math.h>

#include "invertedIndex.h"

#define MAX 100

InvertedIndexBST newBSTNode(char *inputword);
InvertedIndexBST BSTreeInsert(InvertedIndexBST t, char *inputword);
InvertedIndexBST newBSTree();
int BSTreeNumNodes(InvertedIndexBST t);
char * getfiledir(char * dir, char *filename);
void showBSTreeNodeandList(InvertedIndexBST t, FILE *output);
InvertedIndexBST BSTreeFind(InvertedIndexBST t, char *inputword);
void infix(InvertedIndexBST tree, FILE *output);

FileList newFileNode(char *inputfile);
void addFileNode(InvertedIndexBST indexWordNode, FileList fileNode);
int fileNodeExist(InvertedIndexBST t, char *inputfile);
double calculatetf(char *file, char *word);
void fileNodeInsert(InvertedIndexBST indexWordNode, FileList fileNode);

int numofdocuments(char *collectionFilename);
TfIdfList calculateTfIdf(InvertedIndexBST tree, char *searchWord , int D);
TfIdfList newTfIdfNode(char *file);
TfIdfList addsingleTfIdfNode(TfIdfList addonto, char *file, double tf, int D, int numoffiles);
void printTfIdf(TfIdfList t);
double calcsingleTfIdfsum(double tf, int D, double numoffiles);
TfIdfList checktfIdfNode(TfIdfList t, char * filecheck);
TfIdfList duplicatetdidfnode(TfIdfList original);
TfIdfList sorttdidf(TfIdfList orginal, TfIdfList addonto);


//This function normalises a word and check the final letter
//to see if specific character and decides if it is deleted
char * normaliseWord(char *tobenormalised){
  int i = 0;
  while(tobenormalised[i]){
    tobenormalised[i] = tolower(tobenormalised[i]);
    i++;
  }
  if(tobenormalised[i-1] == '.'|| tobenormalised[i-1] == ',' || tobenormalised[i-1] == ';' || tobenormalised[i-1] == '?'){
    tobenormalised[i-1] = '\0';
  }
  return tobenormalised;
}

/** The function needs to read a given file with collection of file names,
    read each of these files, generate inverted index as discussed in
    the specs and return the inverted index.
*/
InvertedIndexBST generateInvertedIndex(char *collectionFilename){

  InvertedIndexBST newtree = malloc(sizeof(InvertedIndexBST));
  newtree = newBSTree();
  FILE *f;
  FILE *ff;
  char filestemp[MAX];
  char infileword[MAX];
  char *temp;

  if ((f = fopen(collectionFilename,"r")) == NULL) {
  		fprintf(stderr, "Can't open file %s\n", collectionFilename);
  		return NULL;
	}


  char *collectionfullcopy = malloc(strlen(collectionFilename));
  strcpy(collectionfullcopy, collectionFilename);
  temp = dirname(collectionfullcopy);
  char* dir = malloc(strlen(temp)*sizeof(char));
  strcpy(dir, temp);


  while(fscanf(f, "%s", filestemp) != EOF){
    char * filename = getfiledir(dir, filestemp);

    if ((ff = fopen(filename,"r")) == NULL) {
    		fprintf(stderr, "Can't open file %s\n", filename);
    		return newtree;
  	}
    rewind(ff);
    int num_of_words = 0;
    while(fscanf(ff, "%s", infileword) != EOF){
      temp = normaliseWord(infileword);
      char* normalised = malloc(strlen(temp)*sizeof(char) + 1);
      strcpy(normalised, temp);
      newtree = BSTreeInsert(newtree, normalised);
      int check = fileNodeExist(BSTreeFind(newtree, normalised), filestemp);
      if(check == 0){
        FileList newfn = newFileNode(filestemp);
        newfn->tf = calculatetf(filename, normalised);
        addFileNode(BSTreeFind(newtree, normalised), newfn);
      }
      num_of_words ++;
    }
    fclose(ff);
  }
  fclose(f);

  return newtree;
}

//Calculates the tf of a specific word within a specific file
double calculatetf(char *file, char *word){
  double tf = 0;
  double wordcount = 0;
  double wordpresent = 0;
  FILE *fff;
  char infileword[MAX];
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

//Creates a new file node
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

//Adds a filenode to a word node
void addFileNode(InvertedIndexBST indexWordNode, FileList fileNode)
{
  if(indexWordNode->fileList == NULL){
    indexWordNode->fileList = fileNode;
  }
  else{
    fileNodeInsert(indexWordNode, fileNode);
  }
}

//Inserts the file node in the correct position within the
//word node's files list with alphabetical order
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

//Checks if a filenode with the same filename already exist within a
//word node's file list if it does exist return 1 if it doesnt then
//return 0
int fileNodeExist(InvertedIndexBST t, char *inputfile){
  FileList itterative = t->fileList;
  int returnvalue = 0;
  while(itterative != NULL){
    if(strcmp(itterative->filename, inputfile) == 0){
      returnvalue = 1;
    }
    itterative = itterative->next;
  }
  return returnvalue;
}

//creates a new tree pointer to work with
InvertedIndexBST newBSTree()
{
	return NULL;
}

//Creates a new word node that will form the tree
InvertedIndexBST newBSTNode(char *inputword)
{
	InvertedIndexBST new = malloc(sizeof(struct InvertedIndexNode));
	assert(new != NULL);
	new->word = malloc(strlen(inputword) + 1);
  assert(new->word != NULL);
  strcpy(new->word, inputword);
	new->left = new->right = NULL;
  new->fileList = NULL;
	return new;
}

//Adds the word node in the correct position within the tree
InvertedIndexBST BSTreeInsert(InvertedIndexBST t, char *inputword)
{

	if (t == NULL){
		return newBSTNode(inputword);
  }
	else if (strcmp(inputword, t->word) < 0){
    t->left = BSTreeInsert(t->left, inputword);
  }
	else if (strcmp(inputword, t->word) > 0){
		t->right = BSTreeInsert(t->right, inputword);
  }
	else; // (strcmp(inputword, t->word) == 0)
		// don't insert duplicates */
	return t;
}

//Return a specific word node on the tree based on a specific word
InvertedIndexBST BSTreeFind(InvertedIndexBST t, char *inputword)
{
	if (t == NULL)
		return NULL;
	else if (strcmp(inputword, t->word) < 0)
		return BSTreeFind(t->left, inputword);
	else if (strcmp(inputword, t->word) > 0)
		return BSTreeFind(t->right, inputword);
	else // (v == t->value)
		return t;
}

//Counts the number of nodes (used for testing)
int BSTreeNumNodes(InvertedIndexBST t)
{
	if (t == NULL)
		return 0;
	else
		return 1 + BSTreeNumNodes(t->left)
	             + BSTreeNumNodes(t->right);
}

//prints the word node including their filelist
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


char * getfiledir(char * dir, char *filename)
{
  char *filedir = malloc((strlen(filename)+strlen(dir)+2)*sizeof(char));
  strcpy(filedir, dir);
  strcat(filedir, "/");
  strcat(filedir, filename);
  strcat(filedir, "\0");
  return filedir;
}

/** The function should output a give inverted index tree to a file named
    invertedIndex.txt. One line per word, words should be alphabetically ordered,
    using ascending order. Each list of filenames (for a single word) should be
    alphabetically ordered, using ascending order.
*/
void printInvertedIndex(InvertedIndexBST tree)
{
  if (tree == NULL) return;
  FILE *output;
  output = fopen("invertedIndex.txt", "w");


  infix(tree, output);

  fclose(output);

}

void infix(InvertedIndexBST tree, FILE *output){
  if(tree == NULL){
    return;
  }
  infix(tree->left, output);
  showBSTreeNodeandList(tree, output);
  infix(tree->right, output);
}

//Counts the number of word documents within the collection
int numofdocuments(char *collectionFilename){
  FILE *f;
  char temp[MAX];
  int num_of_docs = 0;
  if ((f = fopen(collectionFilename,"r")) == NULL) {
  		fprintf(stderr, "Can't open file %s\n", collectionFilename);
  		return EXIT_FAILURE; // ASK ABOUT EXIT FAILURE
	}
  while(fscanf(f, "%s", temp) != EOF){
    num_of_docs++;
  }
  fclose(f);
  return num_of_docs;
}

/** The function returns an ordered list where each node contains filename and the
    corresponding tf-idf value for a given searchWord. You only need to return
    documents (files) that contain a given searchWord. The list must be in descending
    order of tf-idf values. If you have multple files with same tf-idf value, order
    such files (documents) on their filenames using ascending order.
*/
TfIdfList calculateTfIdf(InvertedIndexBST tree, char *searchWord , int D){
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

//Adds the tfidf node in the correct position on the tfidf linked list
TfIdfList addsingleTfIdfNode(TfIdfList addonto, char *file, double tf, int D, int numoffiles){

  TfIdfList newtfidfnode = newTfIdfNode(file);
  TfIdfList itterativepointer = addonto;
  newtfidfnode->tfidf_sum = calcsingleTfIdfsum(tf, D, numoffiles);

  if(itterativepointer == NULL){
    addonto = newtfidfnode;
  }
  else{
    if (newtfidfnode->tfidf_sum>itterativepointer->tfidf_sum) {
      newtfidfnode->next = itterativepointer;
      addonto = newtfidfnode;
      return addonto;
    }
    else{
      while(newtfidfnode->tfidf_sum<=itterativepointer->tfidf_sum){
        if(itterativepointer->next == NULL){
          itterativepointer->next = newtfidfnode;
          return addonto;
        }
        else if(newtfidfnode->tfidf_sum>itterativepointer->next->tfidf_sum){
          break;
        }
        else if(newtfidfnode->tfidf_sum==itterativepointer->next->tfidf_sum){
          if(strcmp(itterativepointer->next->filename,  newtfidfnode->filename) > 0){
            break;
          }
          else{
            newtfidfnode->next = itterativepointer->next->next;
            itterativepointer->next->next = newtfidfnode;
            return addonto;
          }
        }
        else{
          itterativepointer = itterativepointer->next;
        }
      }
      newtfidfnode->next = itterativepointer->next;
      itterativepointer->next = newtfidfnode;
      return addonto;
    }
  }
  return addonto;

}

//Calculates the tfidf of a specific tfidf node
double calcsingleTfIdfsum(double tf, int D, double numoffiles){
  double tfidfsum = 0;
  tfidfsum = tf*log10(D/numoffiles);
  return tfidfsum;
}

//Prints the tfidf node's information for testing
void printTfIdf(TfIdfList t){
  if(t == NULL){
    return;
  }
  printf("PrintingTfIdf: ");
  while(t != NULL){
    printf("%s [tdidf: %f] ",t->filename, t->tfidf_sum);
    t = t->next;
  }
  printf("\n");
}

//Creates and mallocs appropriate space for a new tfidf node
TfIdfList newTfIdfNode(char *file)
{
  TfIdfList new = malloc(sizeof(struct TfIdfNode));
	assert(new != NULL);
  new->filename = malloc(strlen(file)*sizeof(char) + 1);
  strcpy(new->filename, file);
  new->tfidf_sum = 0;
  new->next = NULL;
  return new;
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

/** The function returns an ordered list where each node contains filename and summation
    of tf-idf values of all matching searchWords. You only need to return documents (files)
    that contain one or more searchWords. The list must be in descending order of summation
    of tf-idf values (tfidf_sum). If you have multple files with same tfidf_sum value, order
    such files (documents) on their filenames using ascending order.
*/
TfIdfList retrieve(InvertedIndexBST tree, char* searchWords[] , int D){
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