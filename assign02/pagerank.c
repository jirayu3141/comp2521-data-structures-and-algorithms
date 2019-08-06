
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

    int iteration = 0;
    double diff = diffPR;

    while(iteration < maxIterations && diff >= diffPR) {
        //double Win = calculateWin(g, "url11");
        //double Wout = calculateWout(g, "url11");
        
        
        iteration++;
    }

    return List_Url_PageRanks;  
    
}

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

//print Wout (for debugging)
void printWout(Graph g, char **List_of_Urls) {
    double Wout = 0;
        for (int j = 0; j < numUrl; j++) {
            for (int i = 0; i < numUrl; i++) {
                Wout = calculateWout(g, List_of_Urls[j], List_of_Urls[i]);
                if (Wout != 0) 
                    p("Wout of [%d][%d] is: %f\n", j, i, Wout);
            }
        }
}