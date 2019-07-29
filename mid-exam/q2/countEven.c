
/* countEven.c 
   Written by Ashesh Mahidadia, October 2017
*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "BSTree.h"

/* 
    You will submit only this one file.

    Implement the function "countEven" below. Read the exam paper for 
    detailed specification and description of your task.  

    - You can add helper functions in this file, if required.  

    - DO NOT modify code in the file BSTree.h . 
    - DO NOT add "main" function in this file. 
*/
//void printorder(BSTree t);

// int countEven(BSTree t){

// 	static int count = 0;

//     if (t == NULL)
//         return count;
// 	else {
//         countEven(t->left);
//         countEven(t->right);
//         if (t -> key % 2 == 0) {
//             count++;
//         }
//     }
//     return count;
// }

int countEven(BSTree t) {

	int count = 0;
    if (t == NULL)
        return 0;
    else {
        if(t->key % 2 == 0)
            count = 1;
        return count + countEven(t->left) + countEven (t->right);
    }
}