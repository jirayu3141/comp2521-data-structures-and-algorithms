#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <string.h>
#include <ctype.h>
#include "readData.h"

#define MAX_WORD 5000
int numUrl = 0;
char **List_of_Urls;
Graph urlGraph;

char * normaliseWord(char *str) 
{
    //removeing leading and trailing spaces
    removeSpaces(str);     

    //converting all charcters to lowercase
    for (int i = 0, j = strlen(str); i < j; i++) {
        if (isupper(str[i]))
            str[i] = tolower(str[i]);
    }
    //remove punctuation marks
    char *endPtr = str + strlen(str) - 1;
    if (*endPtr == '.' || *endPtr == ',' || *endPtr == ';'|| *endPtr == '?')
        *endPtr = '\0';
    
    return str;
}
//remove spaces form string
void removeSpaces(char *str) {
    int i;
    int begin = 0;
    int end = strlen(str) - 1;
    while (isspace((unsigned char) str[begin]))
        begin++;

    while ((end >= begin) && isspace((unsigned char) str[end]))
        end--;

    // Shift all characters back to the start of the string array.
    for (i = begin; i <= end; i++)
        str[i - begin] = str[i];

    str[i - begin] = '\0'; // Null terminate string.


}

// read data from a file and put it into strings
char* ReadFile(char *filename)
{
    char *buffer = NULL;
    int string_size, read_size;
    FILE *handler = fopen(filename, "r");
    if(handler == NULL) {
        printf("Error!, can't open file\n");
        exit(1); 
    }
    // Seek the last byte of the file
    fseek(handler, 0, SEEK_END);
    // Offset from the first to the last byte, or in other words, filesize
    string_size = ftell(handler);
    // go back to the start of the file
    rewind(handler);

    // Allocate a string that can hold it all
    buffer = (char*) malloc(sizeof(char) * (string_size + 1) );

    // Read it all in one operation
    read_size = fread(buffer, sizeof(char), string_size, handler);

    // fread doesn't set it so put a \0 in the last position
    // and buffer is now officially a string
    buffer[string_size] = '\0';

    if (string_size != read_size)
    {
        // Something went wrong, throw away the memory and set
        // the buffer to NULL
        free(buffer);
        buffer = NULL;
    }

       // Always remember to close the file.
       fclose(handler);

    return buffer;
}

// read words seperated by tabs or spaces or newline and put it into destination array
int storeTokenToArray(char *content, char *destination[]){
    char *array[MAX_FILE];
    //extract file names into array
    char *word;
    word = strtok(content, " ,   \n");
    int i = 0;
    while(word != NULL) {
        word = normaliseWord(word);
        array[i] = malloc(sizeof(char) * strlen(word+1));
        //printf("strlen %s = %ld\n", word, sizeof(word));
        // array[i] = word;
        strcpy(array[i], word);
        word = strtok(NULL, " ,   \n");
        i++;
    }
    //copy array
    for (int j = 0; j < i; j++) {
        // destination[j] = array[j];
        // Copy to destination
        destination[j] = malloc(sizeof(char) * strlen(array[j] +1));
        strcpy(destination[j], array[j]);
        free(array[j]);
    }
    return i;
}

// read from collection.txt and return an array of strings that contain the file names
char** GetCollection() {
    List_of_Urls = malloc(MAX_FILE * MAX_FILE);
    numUrl = storeTokenToArray(ReadFile("collection.txt"), List_of_Urls);
    return List_of_Urls;
}

//this function reads the url from collection and build a graph out of it
Graph GetGraph(char **List_of_Urls) {
    //create new graph
    Graph g = newGraph(numUrl);
    //buildGraphFromFile(g, List_of_Urls[1]);
    //iterate through each file and see what is is linked with each other using breath first search
    for (int i = 0; i < numUrl; i++) {
        //printf("%s\n", List_of_Urls[i]);
        // for each url, read the links and build graph

        buildGraphFromFile(g, List_of_Urls[i]);
    }
    return g;
}

//read section 1 and put the url into a graph
void buildGraphFromFile(Graph g, char* filename) {
    //read from .. to ...
    //char buffer[MAX_FILE];
    //char *buff = malloc (MAX_FILE * MAX_FILE); 
    // char* url[MAX_FILE];
    char** url = (char **) malloc(sizeof(char *) * MAX_URL);

    char *filename_txt = malloc (sizeof(char) * MAX_URL);
    strcpy(filename_txt, filename);

    strcat(filename_txt, ".txt");
    char *buff = ReadFile(filename_txt);
    free(filename_txt);
    //put the pointer after "start section 1"
    buff+= 16;
    //find the end of section 1 and put null character
    char *curr = strstr(buff ,"#end Section-1");
    *curr = '\0';

    int urlCount = storeTokenToArray (buff, url);
    for (int i = 0; i < urlCount; i++) {
        //put up the vertices)
        addEdge(g, filename, url[i]);
    }
}


//read section 2 of the file and store it in an array
int readSection2 (char *filename, char *destination[]) {
    //add .txt extension
    char filename_txt[MAX_URL];
    strcpy(filename_txt, filename);
    strcat(filename_txt, ".txt");
    //flag if reading fails
    int i = 0;
    //initialize arrays
    char *fileContent = ReadFile(filename_txt);
    //char word[MAX_WORD] = {'\0'};
    
    //find section 2 
    fileContent = strstr(fileContent, "#start Section-2");
    char *start= fileContent + strlen("#start Section-2");

    //remove end Section 2
    char *end;
    end = strstr(fileContent, "#end Section-2");
    *end = '\0';
    //tokenise
    storeTokenToArray(start, destination);
    return i;
}
