// testList.c - testing DLList data type
// Written by John Shepherd, March 2013
// Modified by Jirayu Sirivorawong z5125805, 16 June 2019

// size of line buffers
#define MAX 8192
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "DLList.h"

/*FUNCTION DECLARATION */
void test_add_before(DLList myList);
void test_add_after(DLList myList);
void test_delete(DLList myList);
void printList (DLList myList);	//print list with length

/* GLOBAL VARIABLES FOR TESTINGS */
static char *first = "this is first line";
static char *a_first = "this is line after first";
static char *b_last = "this is before last line";
static char *last = "this is last line";

int main (void)
{
	DLList myList = getDLList (stdin);
	printList (myList);
	assert (validDLList (myList));


	/*TESTS*/
	test_add_before(myList);
	test_delete(myList);
	test_add_after(myList);



	freeDLList (myList);
	return EXIT_SUCCESS;
}

void test_add_before(DLList myList) {
	printf("------TEST: ADD BEFORE (add first and line before last)------\n");
	int init_length = (int)DLListLength(myList);

	/* add to the fist line */
	printf("adding first line..\n");
	DLListMoveTo (myList, 1);
	DLListBefore (myList, first);
	assert(strcmp(DLListCurrent(myList), first) == 0);
	printList(myList);
	printf("Current-> %s\n\n",DLListCurrent(myList));
	assert((int)DLListLength(myList) == init_length+1);
	DLListDelete(myList);
	
	/* add to the line before last */
	printf("adding line before last..\n");
	DLListMoveTo (myList, (int)DLListLength(myList));
	DLListBefore (myList, b_last);
	assert(strcmp(DLListCurrent(myList), b_last) == 0);
	printList(myList);
	printf("Current-> %s\n\n",DLListCurrent(myList));
	assert((int)DLListLength(myList) == init_length+1);
	DLListDelete(myList);

	/* check validity and length */
	assert (validDLList (myList));
	assert((int)DLListLength(myList) == init_length);
	
	printf("------ADD BEFORE: ALL TEST PASSED------\n");
}

void test_delete(DLList myList) {
	int init_length = (int)DLListLength(myList);
	printf("\n------TEST: DELETE (delete first and line before last)------\n");
	/*delete first line */
	printf("deleting first line..\n");
	DLListMoveTo (myList, 1);
	DLListDelete(myList);
	printList(myList);
	printf("Current-> %s\n\n",DLListCurrent(myList));
	assert((int)DLListLength(myList) == init_length-1);

	/*delete line before last*/
	if(DLListLength(myList) > 1) {
		printf("deleting line before last..\n");
		DLListMoveTo (myList, (int)DLListLength(myList)-1);
		DLListDelete(myList);
		printList(myList);
		printf("Current-> %s\n\n",DLListCurrent(myList));
		assert((int)DLListLength(myList) == init_length-2);
	}

	/* check validity and length */
	if(DLListLength(myList) != 0)	assert (validDLList (myList));
	
	printf("------DELETE: ALL TEST PASSED------\n");
}

void test_add_after(DLList myList) {
	printf("\n------TEST: ADD AFTER (add line after first and last)------\n");
	int init_length = (int)DLListLength(myList);
	/* add to line after first */
	printf("adding line after first..\n");
	DLListMoveTo(myList, 1);
	DLListAfter (myList, a_first);	//add to line after first
	assert(strcmp(DLListCurrent(myList), a_first) == 0);
	printList(myList);
	printf("Current-> %s\n\n",DLListCurrent(myList));
	assert((int)DLListLength(myList) == init_length+1);
	DLListDelete(myList);
	

	/* add to last line */
	printf("adding to last line..\n");
	DLListMoveTo(myList, (int)DLListLength(myList));
	DLListAfter (myList, last);	//add to last line
	assert(strcmp(DLListCurrent(myList), last) == 0);
	printList(myList);
	printf("Current-> %s\n\n",DLListCurrent(myList));
	assert((int)DLListLength(myList) == init_length+1);
	DLListDelete(myList);

	/* check validity and length */
	assert (validDLList (myList));
	assert((int)DLListLength(myList) == init_length);
	
	printf("---ADD AFTER: ALL TEST PASSED---\n");

}

void printList(DLList myList)
{
	printf("showing current list: \n\n");
	putDLList(stdout, myList);
	printf("\nnumber of element: %d\n", (int)DLListLength(myList));

}
