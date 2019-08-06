//Calculate PageRank from a List of Page
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
#define LINE_LENGTH 100
#define BUFSIZE 50

double PageRank[MAX_URL];

int main(int argc, char *argv[])
{
	if (argc < 4) {
		fprintf(stderr, "Usage: %s  <damping factor> <difference in PageRank sum> <maximum iterations>\n", argv[0]);
		return EXIT_FAILURE;
	}

    char **List_of_Urls = GetCollection();
    Graph g = GetGraph(List_of_Urls);
    char **List = calculatePageRank(g, atof(argv[1]),  atof(argv[2]),  atof(argv[3]));
    char **OrderedList = order(List);
    outToFile(OrderedList, "pagerankList.txt");

    return EXIT_SUCCESS;
}

//Calculate Page Rank base on the given configuration
char **calculatePageRank(Graph g, double d, double diffPR, int maxIterations) {
    //NOTE: remember to free it later
    char **List_Url_PageRanks = (char **)malloc(sizeof(char*) * MAX_FILE);
    double PR = 1/(float)numUrl;

    //For each url pi in the collection, PageRank = 1/N
    for (int i = 0; i < numUrl; i++) {
        //allocate space for the strings in array List_Url_PageRank
        List_Url_PageRanks[i] = malloc (sizeof(char) * LINE_LENGTH);
        if (List_Url_PageRanks[i] == NULL) {
            err (EX_OSERR, "couldn't allocate List_Url_Pageranks[%d]", i);
        }
        addPageRank(g, List_of_Urls[i], PR);
    }

    //current Page Rank
    for (int i = 0; i < numUrl; i++) {
        PageRank[i] = PR;
    }

    int iteration = 0;
    double diff = diffPR;

    while(iteration < maxIterations && diff >= diffPR) {
        for (int i = 0; i < numUrl; i++) {
            PR = (1-d)/(double)numUrl + (d *sum_PR(g, List_of_Urls[i]));
            addPageRank(g, List_of_Urls[i], PR);
            PR = 0;
        }
        //update PageRank for prev iteration
        for (int i = 0; i < numUrl; i++) {
            PageRank[i] = getPR(g, List_of_Urls[i]);
        }

        iteration++;
    }

    //put data to List_Url_PageRanks
    for (int i = 0; i < numUrl; i++) {
        char line[BUFSIZE];
        sprintf(line, "%s, %d, %.7f", List_of_Urls[i], numOutDegree(g, List_of_Urls[i]), PageRank[i]);
        strcpy(List_Url_PageRanks[i], line);
    }
    return List_Url_PageRanks;  
    
}

//calcualte W in of given vertices
double calculateWin (Graph g, char *vertex, char *vertex2) {
    double Win = 0;
    //check if the node is connected in the first place
    if ((isConnected(g, vertex, vertex2) == false) || strcmp(vertex, vertex2) == 0) {
        return 0;
    }
    //number of in vertex2
    int vertex2In = 0;
    vertex2In = numInDegree(g, vertex2);
    // in nodes that are connected to vertex
    double Ip[BUFSIZE] = {0};
    int index = 0;

    //get the Ip values
    for (int i = 0; i < numUrl; i++){
        //find the nodes that vertex is referencing from
        if(isConnected(g, vertex, List_of_Urls[i])&& (strcmp(vertex, List_of_Urls[i]) != 0) ) {
            //within this if is the node that Vertex is referecing from

            //check the number of incomming nodes and store it in IP array
            int count = numInDegree(g, List_of_Urls[i]);
            Ip[index] = count;
            if (count == 0) {
                Ip[index] = 0.5;
            }
            index++;
        }
    }
    double sumIp = 0;
    for (int i = 0; Ip[i] != 0; i++) {
        sumIp += Ip[i];
    }

    Win = (double)vertex2In/(double)sumIp;
    return Win;
}

//calculate W out of 2 given vertices
double calculateWout (Graph g, char *vertex, char *vertex2) {
    double Wout = 0;
    //check if the node is connected in the first place
    if ((isConnected(g, vertex, vertex2) == false) || strcmp(vertex, vertex2) == 0) {
        return 0;
    }
    //calculate the out degree of vertex2
    double v2out = numOutDegree(g, vertex2);
    if (v2out == 0) v2out = 0.5;
    double Op[BUFSIZE] = {0};
    int ind = 0;
    //store the Op in Op array
    //for every vertex that the main vertex refer to, find outLink and store in array
    for (int i = 0; i < numUrl; i++) {
        if( (isConnected(g, vertex, List_of_Urls[i])) && (strcmp(vertex, List_of_Urls[i]) != 0) ) {

            //check if it is not self loop

            Op[ind] = numOutDegree(g, List_of_Urls[i]);
            if (Op[ind] == 0) {
                Op[ind] = 0.5;
            }
            ind++;
        }
    }

    double sumOp = 0;
    for (int i = 0; Op[i] != 0; i++) {
        sumOp += Op[i];
    }

    Wout = v2out/sumOp;

    return Wout;
} 

//return the out degree of a vertex
int numOutDegree(Graph g, char *vertex) {
    int count = 0;
    for (int i = 0; i < numUrl; i++) {

        if (isConnected(g, vertex, List_of_Urls[i])) {
            if (strcmp(vertex, List_of_Urls[i]) == 0) {
                continue;
            }
            count++;
        }
    }
    return count;
}

//return the In degree of a vertex
int numInDegree(Graph g, char *vertex) {
    int count = 0;
    for (int i = 0; i < numUrl; i++) {

        if (isConnected(g, List_of_Urls[i], vertex )) {
            if (strcmp(vertex, List_of_Urls[i]) == 0) {
                continue;
            }
            count++;
        }
    }
    return count;
}

//calculate the sum of pagerank (2nd part of PR equation)
double sum_PR(Graph g, char *vertex) {
    double sum = 0;
    for (int i = 0; i < numUrl; i++) {
        sum += PageRank[i] * calculateWin(g, List_of_Urls[i], vertex) * calculateWout(g, List_of_Urls[i], vertex);
    }
    return sum;
}

char **order(char **List) {
    char **Ordered_List_Urls_PageRank = (char **)malloc(sizeof(char*) * MAX_FILE);

    //allocate array space
    for (int i = 0; i < numUrl; i++) {
        //allocate space for the strings in array List_Url_PageRank
        Ordered_List_Urls_PageRank[i] = malloc (sizeof(char) * LINE_LENGTH);
        if (Ordered_List_Urls_PageRank[i] == NULL) {
            err (EX_OSERR, "couldn't allocate List_Url_Pageranks[%d]", i);
        }
    }

    double *temp_PR = malloc(sizeof(double) * numUrl);
    assert(temp_PR != NULL);
    //copy temp array for sorting
    for (int i = 0; i < numUrl; i++) {
        temp_PR[i] = PageRank[i];
    }

    double max = 0;
    int maxInd = 0;
    for (int j = 0; j < numUrl; j++) {
        for (int i = 0; i < numUrl; i++) {
            if (max < temp_PR[i]){ 
                max = temp_PR[i];
                maxInd = i;
            }
        }
        strcpy(Ordered_List_Urls_PageRank[j], List[maxInd]);
        temp_PR[maxInd] = 0;
        maxInd = 0;
        max = 0;
    }
    free (temp_PR);

    return Ordered_List_Urls_PageRank;
}

//output the list to file
void outToFile(char** List, char *name) {
    FILE *fp = fopen(name, "w");
    if (fp == NULL) {
      fprintf(stderr, "Cant create %s file", name);
      exit(1);
   }
    int i = 0;
    while(i < numUrl) {
        fprintf(fp, "%s\n", List[i]);
        i++;
    }
    fclose(fp);
}