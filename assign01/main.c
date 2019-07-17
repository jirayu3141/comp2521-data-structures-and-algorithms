#include "invertedIndex.h"
#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
void checkNormalisedString(char *origString, char *answerString){

	char originalString[100];
	char *stuString;

	strcpy(originalString, origString);
	stuString = normaliseWord(originalString);

	if( (originalString == stuString) && 
	    (strcmp(stuString, answerString) == 0) ){
		printf("> Test Passed: %s to %s\n", origString, stuString);
	}
	else {
		printf("> Test Failed: %s to %s [Expected %s]\n", origString, stuString, answerString);
	}	
}
void testNormalise(){
	printf("Testing function  normaliseWord \n");
	checkNormalisedString(".Net", ".net");
	checkNormalisedString("smh.com.au", "smh.com.au");
	checkNormalisedString("Sydney!", "sydney!");
	checkNormalisedString("wHy?", "why");
	checkNormalisedString("ORDER.", "order");
	checkNormalisedString("Text;", "text");
	checkNormalisedString("abc.net.au.", "abc.net.au");
	checkNormalisedString("Sydney???", "sydney??");
}


int main() {
        //testNormalise(); 

		InvertedIndexBST invertedTree =  generateInvertedIndex("collection.txt");
		printInvertedIndex(invertedTree); 

    return 0;
}   