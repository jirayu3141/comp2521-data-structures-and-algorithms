//readData.h ... read data function from the collection
//Written by Jirayu Sirivorawong  July 2019

#ifndef READDATA_H
#define READDATA_H
#define MAX_FILE 1000
#define MAX_URL 100



void removeSpaces(char *str);
char* ReadFile(char *filename);
int storeTokenToArray(char *content, char**);
char **getCollection();

#endif