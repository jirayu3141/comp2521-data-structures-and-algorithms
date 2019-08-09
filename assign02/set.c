// set.c ... simple, inefficient Set of Strings
// Written by John Shepherd, September 2015
// Modified to be able to save pageRank by Jirayu Sirivorawong
// functions in the prototype function are newly written
// functions modified includes insertInto to insert in order of pagerank.
// function that has 1 infront of the names are for regular set for string
// fucntion without 1 are for type url struct (part 1 & 2)

#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>

#include "readData.h"
#include "set.h"

#define strEQ(s, t) (strcmp ((s), (t)) == 0)
#define strLT(s, t) (strcmp ((s), (t)) < 0)

#define MAX_FILESIZE 5000


//fucntion prototype
char **extractSearhUrl(char *filename, char *word);
double findPageRank(char *url);
void addWordScore(SetLink s, char* word);
void insertMode (SetLink list, char *str, SetLink *cur, SetLink *pre);
//char* mystrdup(const char* s);	//incase it is not included in string.h
char *mystrtok_r (char *s, const char *delim, char **save_ptr); //in case it is not included in some version of gcc string.h


// Function signatures

static SetLink newNode (char *);
static void dropNode (SetLink);
static int findNode (SetLink, char *, SetLink *, SetLink *);

// newSet()
// - create an initially empty Set
Set newSet (void)
{
	Set new = malloc (sizeof *new);
	if (new == NULL)
		err (EX_OSERR, "couldn't allocate Set");
	*new = (SetRep){.nelems = 0, .elems = NULL};
	return new;
}

// dropSet(Set)
// - clean up memory associated with Set
void dropSet (Set s)
{
	if (s == NULL)
		return;
	SetLink curr = s->elems;
	while (curr != NULL) {
		SetLink next = curr->next;
		dropNode (curr);
		curr = next;
	}

	free (s);
}

//for url insersion
// insertInto(Set,Str)
// - ensure that Str is in Set
void insertInto (Set s, char *str)
{
	assert (s != NULL);
	SetLink curr, prev;
	int found = findNode (s->elems, str, &curr, &prev);
	//insert based on PageRank score
	//insertMode(s->elems, str, &curr, &prev);
	if (found) {
		addWordScore(s->elems, str);
		return; // already in Set
	}

	SetLink new = newNode (str);
	s->nelems++;
	if (prev == NULL) {
		// add at start of list of elems
		new->next = s->elems;
		s->elems = new;
	} else {
		// add into list of elems
		new->next = prev->next;
		prev->next = new;
	}
}


// dropFrom(Set,Str)
// - ensure that Str is not in Set
void dropFrom (Set s, char *str)
{
	assert (s != NULL);
	SetLink curr, prev;
	int found = findNode (s->elems, str, &curr, &prev);
	if (!found)
		return;
	s->nelems--;
	if (prev == NULL)
		s->elems = curr->next;
	else
		prev->next = curr->next;
	dropNode (curr);
}

// isElem(Set,Str)
// - check whether Str is contained in Set
int isElem (Set s, char *str)
{
	assert (s != NULL);
	SetLink curr, prev;
	return findNode (s->elems, str, &curr, &prev);
}

// nElems(Set)
// - return # elements in Set
int nElems (Set s)
{
	assert (s != NULL);
	return s->nelems;
}

// showSet(Set)
// - display Set (for debugging)
void showSet (Set s)
{
	SetLink curr;
	if (s->nelems == 0)
		printf ("Set is empty\n");
	else {
		printf ("Set has %d elements:\n", s->nelems);
		int id = 0;
		curr = s->elems;
		while (curr != NULL) {
			printf ("[%d] %s WordScore: %d PageRank: %f\n", id, curr->val, curr->wordScore, curr->page_rank_score);
			id++;
			curr = curr->next;
		}
	}
}

// Helper functions
//newNode for url stuct (part 1 & 2)
static SetLink newNode (char *str)
{
	SetLink new = malloc (sizeof (SetNode));
	assert (new != NULL);
	new->val = mystrdup (str);
	new->next = NULL;
	new->wordScore = 1;
	new->page_rank_score = findPageRank(str);
	return new;
}

static void dropNode (SetLink curr)
{
	assert (curr != NULL);
	free (curr->val);
	free (curr);
}

// special finadNode used for urls
static int findNode (SetLink list, char *str, SetLink *cur, SetLink *pre)
{
	SetLink curr = list, prev = NULL;
	while (curr != NULL && (curr->page_rank_score > findPageRank(str))) {
		prev = curr;
		curr = curr->next;
	}
	*cur = curr;
	*pre = prev;
	return (curr != NULL && (curr->page_rank_score == findPageRank(str)));
}
void insertMode (SetLink list, char *str, SetLink *cur, SetLink *pre)
{
	SetLink curr = list, prev = NULL;
	while (curr != NULL && curr->page_rank_score < findPageRank(str)) {
		prev = curr;
		curr = curr->next;
	}
	*cur = curr;
	*pre = prev;
	//return (curr != NULL && strEQ (str, curr->val));
}

// returns number of words in str 
// taken from geeksforgeeks.orgs
unsigned countWords(char *str) 
{ 
    int state = 0; 
    unsigned wc = 0;  // word count 
  
    // Scan all characters one by one 
    while (*str) 
    { 
        // If next character is a separator, set the  
        // state as OUT 
        if (*str == ' ' || *str == '\t') 
            state = 0; 
		else if (*str == '\n') {
			break;
		}
        // If next character is not a word separator and  
        // state is OUT, then set the state as IN and  
        // increment word count 
        else if (state == 0) 
        { 
            state = 1; 
            ++wc; 
        } 
  
        // Move to next character 
        ++str; 
    } 
  
    return wc; 
} 
char **extractSearhUrl(char *filename, char *word) {
	//find the url corresponding with
	char **extractedURL = malloc(sizeof(char*) * MAX_FILE);
	char wordSearch[BUFSIZ];
	strcpy(wordSearch, word);
	normaliseWord(wordSearch);
	char *buffer = ReadFile(filename);
	char *foundword = buffer;

	foundword = strstr(foundword, wordSearch);
	//word not found
	if (foundword == NULL) {
		return extractedURL;
	} 
	//go to 'url'
	foundword += strlen(wordSearch) + 1;
	
	//check if url is located
	if (*foundword != 'u' && *(foundword+1) != 'r' && *(foundword+2) != 'l') {
		return extractedURL;
	}

	//copy urls to extractedURL array
	char *token;
	for (int i = 0; (token = mystrtok_r(foundword, " ", &foundword)) && *token != '\n'; i++) {
		extractedURL[i] = malloc(sizeof(char) * MAX_FILE);
		strcpy(extractedURL[i], token);
	}
	
	
	return extractedURL;
}

//look at pageranklist.txt and extrct pagerank of that url
double findPageRank(char *url) {
	//open file
	FILE *fptr;
	if ((fptr = fopen("pagerankList.txt","r")) == NULL) {
       printf("Error! opening file");
       exit(EXIT_FAILURE);
   	}
	double pagerank;
	char line[MAX_FILE];	//array to store line data
	while (fgets(line, sizeof(line), fptr) != NULL) {
		if (strstr(line, url) != NULL) {
			sscanf(line, "%*s %*s %10lf", &pagerank);
			//printf("%f\n", pagerank);
		}
	}

	return pagerank;
}

void addWordScore (SetLink list, char *str)
{
	SetLink curr = list;
	while (curr != NULL && curr->page_rank_score != findPageRank(str)) {
		curr = curr->next;
	}
	curr->wordScore++;
}



//taken from "https://code.woboq.org/userspace/glibc/string/strtok_r.c.html"
char *mystrtok_r (char *s, const char *delim, char **save_ptr)
{
  char *end;
  if (s == NULL)
    s = *save_ptr;
  if (*s == '\0')
    {
      *save_ptr = s;
      return NULL;
    }
  /* Scan leading delimiters.  */
  s += strspn (s, delim);
  if (*s == '\0')
    {
      *save_ptr = s;
      return NULL;
    }
  /* Find the end of the token.  */
  end = s + strcspn (s, delim);
  if (*end == '\0')
    {
      *save_ptr = end;
      return s;
    }
  /* Terminate the token and make *SAVE_PTR point past it.  */
  *end = '\0';
  *save_ptr = end + 1;
  return s;
}