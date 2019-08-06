//readData.h ... read data function from the collection
//Written by Jirayu Sirivorawong  July 2019

#ifndef READDATA_H
#define READDATA_H
#define MAX_FILE 1000
#define MAX_URL 100
#define p(...) printf(__VA_ARGS__)
#include "Graph.h"
extern int numUrl;
extern char **List_of_Urls; 
//remove spaces form string
void removeSpaces(char *str);

// read data from a file and put it into strings
char* ReadFile(char *filename);

// read words seperated by tabs or spaces or newline and put it into destination array
int storeTokenToArray(char *content, char**);

// read from collection.txt and return an array of strings that contain the file names
char **GetCollection();

//Create a graph from List of URLs
Graph GetGraph(char **);

//reading the url from the file and put it into graph
void buildGraphFromFile(Graph , char* filename);

//show graph in a format that shows the name
void showGraph1 (Graph g, char **names);

//read section 2 of the file
int readSection2 (char *filename, char **destination);
#endif