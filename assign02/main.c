#include "readData.h"
#include "pagerank.h"
#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>

void printDegIn(Graph g, int i) {
    printf("Indeg [%s]: %d\n", List_of_Urls[i], numInDegree(g, List_of_Urls[i]));
}

void printWin(Graph g, int i) {
    double Win = 0;
    for (int j = 0; j < numUrl; j++) {
        Win = calculateWin(g, List_of_Urls[i], List_of_Urls[j]);
        if (Win == 0)   continue;
        printf("Win of [%d][%d] is: %f\n", i, j, Win);
    }
}

void loop(Graph g, int value, void (*func)(Graph, int)) {
    for (int i = 0; i < numUrl; i++) {
        func(g, i);
    }
}

int main() {
    char **List_of_Urls = GetCollection();
    Graph g = GetGraph(List_of_Urls);
    showGraph(g, 1);
    loop(g, numUrl ,printWin);

    // double Win = calculateWin(g, List_of_Urls[1], List_of_Urls[4]);
    // printf("W in is : %f\n", Win);
    //printWout(g, List_of_Urls);

}
