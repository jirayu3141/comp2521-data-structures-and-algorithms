// getPeaks.c 
// Written by Ashesh Mahidadia, August 2017

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "DLList.h"


/* 
    You will submit only this one file.

    Implement the function "getPeaks" below. Read the exam paper for 
    detailed specification and description of your task.  

    - DO NOT modify code in the file DLList.h . 
    - You can add helper functions in this file.  
    - DO NOT add "main" function in this file. 
    
*/

void insertnode(DLList, int);

DLList getPeaks(DLList L)
{
	DLList peaksL = newDLList();
    //for empty list, 1 and 2 element
    if (L->nitems >= 0 && L->nitems <= 2) {
        return peaksL;
    } else {
        while (L->curr->next != L->last) {
            L->curr = L->curr->next;
           //L->curr->next = L->curr->next;
            //if val of curr is more than prev n next, create a node and push it in
            if (L->curr->value > L->curr->prev->value && L->curr->value > L->curr->next->value) {
                insertnode(peaksL, L->curr->value);
                }
            }
        }
	return peaksL;
}

void insertnode (DLList L, int val)
{
    //create new node
    DLListNode *new = newDLListNode(val);
    //insert to empty list
    if (L->nitems == 0) {
        L->first = new;
    //insert to an exisiting list
    } else {
        //adjust new proteties
        new->prev = L->curr;
        //adjust L properties

    }
    L->last = new;
    L->curr = new;
    L->nitems++;
}
