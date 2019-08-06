#ifndef PAGERANK_H
#define PAGERANK_H

#include "Graph.h"
#include "readData.h"

char **calculatePageRank(Graph g, double d, double diffPR, int maxIterations);

double calculateWin (Graph g, char *vertex, char *vertex2);
double calculateWout (Graph g, char *vertex, char *vertex2);
int numOutDegree(Graph g, char *vertex);
int numInDegree(Graph g, char *vertex);
void printWout(Graph g, char **List_of_Urls);


#endif