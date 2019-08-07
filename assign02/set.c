// set.c ... simple, inefficient Set of Strings
// Written by John Shepherd, September 2015
// Modified to be able to save pageRank by Jirayu Sirivorawong

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

typedef struct Node *Link;

typedef struct Node {
	char *val;
	int wordScore;
	double page_rank_score;
	Link next;
} Node;

typedef struct SetRep {
	int nelems;
	Link elems;
} SetRep;

// Function signatures

static Link newNode (char *);
static void dropNode (Link);
static int findNode (Link, char *, Link *, Link *);

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
	Link curr = s->elems;
	while (curr != NULL) {
		Link next = curr->next;
		dropNode (curr);
		curr = next;
	}

	free (s);
}

// insertInto(Set,Str)
// - ensure that Str is in Set
void insertInto (Set s, char *str)
{
	assert (s != NULL);
	Link curr, prev;
	int found = findNode (s->elems, str, &curr, &prev);
	if (found)
		return; // already in Set

	Link new = newNode (str);
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
	Link curr, prev;
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
	Link curr, prev;
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
	Link curr;
	if (s->nelems == 0)
		printf ("Set is empty\n");
	else {
		printf ("Set has %d elements:\n", s->nelems);
		int id = 0;
		curr = s->elems;
		while (curr != NULL) {
			printf ("[%03d] %s\n", id, curr->val);
			id++;
			curr = curr->next;
		}
	}
}

// Helper functions

static Link newNode (char *str)
{
	Link new = malloc (sizeof (Node));
	assert (new != NULL);
	new->val = strdup (str);
	new->next = NULL;
	return new;
}

static void dropNode (Link curr)
{
	assert (curr != NULL);
	free (curr->val);
	free (curr);
}

// findNode(L,Str)
// - finds where Str could be added into L
// - if already in L, curr->val == Str
// - if not already in L, curr and prev indicate where to insert
// - return value indicates whether Str found or not
static int findNode (Link list, char *str, Link *cur, Link *pre)
{
	Link curr = list, prev = NULL;
	while (curr != NULL && strLT (curr->val, str)) {
		prev = curr;
		curr = curr->next;
	}
	*cur = curr;
	*pre = prev;
	return (curr != NULL && strEQ (str, curr->val));
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

	foundword += strlen(wordSearch) + 1;
	if (*foundword != 'u' && *(foundword+1) != 'r' && *(foundword+2) != 'l') {
		return extractedURL;
	}

	//printf("%s", foundword);
	//sscanf(foundword, "%*s %s\n", str);
	for (int i = 0; i < countWords(foundword); i ++) {
		
	}
	char *token;
	
	for (int i = 0; (token = strtok_r(foundword, " ", &foundword)) && *token != '\n'; i++) {
		extractedURL[i] = malloc(sizeof(char) * MAX_FILE);
		strcpy(extractedURL[i], token);
	}
	
	
	return extractedURL;
}
void insertUrlInto(char *filename, Set s, char *word) {
	
	
	char str[BUFSIZ] = {'\0'};
	char **urls = extractSearhUrl(filename, word);
	for (int i = 0; urls[i] != NULL; i++) 
		printf("%s\n", urls[i]);
	// Link curr, prev;
	// int found = findNode (s->elems, str, &curr, &prev);
	// if (found)
	// 	return; // already in Set

	// Link new = newNode (str);
	// s->nelems++;
	// if (prev == NULL) {
	// 	// add at start of list of elems
	// 	new->next = s->elems;
	// 	s->elems = new;
	// } else {
	// 	// add into list of elems
	// 	new->next = prev->next;
	// 	prev->next = new;
	// } 

}