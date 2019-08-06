//Written by Jirayu Sirivorawong  July 2019
#include <assert.h>
#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <math.h>
#include "pagerank.h"
#include "invertedIndex.h"

int main (int argc, char *argv[]) {
    generateInvertedIndex("collection.txt");
}