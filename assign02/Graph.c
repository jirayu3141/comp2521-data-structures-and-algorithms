// graph.c ... Graph of strings (adjacency matrix)
// Written by John Shepherd, September 2015

#include <assert.h>
#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>

#include "graph.h"
#include "readData.h"

#define strEQ(g, t) (strcmp ((g), (t)) == 0)

typedef double Num;

typedef struct GraphRep {
	size_t nV;
	size_t maxV;
	size_t nE;
	char **vertex;
	Num **edges;
	double *PageRank;
} GraphRep;

// Function signatures

static size_t addVertex (char *, char **, size_t);

// newGraph()
// - create an initially empty Graph
Graph newGraph (size_t maxV)
{
	Graph new = malloc (sizeof *new);
	if (new == NULL)
		err (EX_OSERR, "couldn't allocate Graph");
	*new = (GraphRep){.nV = 0, .maxV = maxV};
	new->vertex = calloc (maxV, sizeof (char *));
	if (new->vertex == NULL)
		err (EX_OSERR, "calloc");
	new->edges = calloc (maxV, sizeof (Num *));
	if (new->edges == NULL)
		err (EX_OSERR, "calloc");
	new->PageRank = calloc (maxV, sizeof(double));
	if (new->PageRank == NULL)
		err (EX_OSERR, "calloc");

	for (size_t i = 0; i < maxV; i++) {
		new->edges[i] = calloc (maxV, sizeof (Num));
		if (new->edges[i] == NULL)
			err (EX_OSERR, "calloc");

		for (size_t j = 0; j < maxV; j++) {
			new->edges[i][j] = 0;
		}
		new->PageRank[i] = 0;		
	}

	return new;
}

// dropGraph(Graph)
// - clean up memory associated with Graph
void dropGraph (Graph g)
{
	if (g == NULL)
		return;

	for (size_t i = 0; i < g->nV; i++)
		free (g->vertex[i]);
	for (size_t i = 0; i < g->maxV; i++)
		free (g->edges[i]);

	free (g->vertex);
	free (g->edges);
	free (g);
}

// addEdge(Graph,Src,Dest)
// - add an edge from Src to Dest
// - returns 1 if edge successfully added
// - returns 0 if unable to add edge
//   (usually because nV exceeds maxV)
bool addEdge (Graph g, char *src, char *dest)
{
	assert (g != NULL);

	ssize_t v = vertexID (src, g->vertex, g->nV);
	if (v < 0) {
		if (g->nV >= g->maxV)
			return false;
		v = (ssize_t) addVertex (src, g->vertex, g->nV);
		g->nV++;
	}

	ssize_t w = vertexID (dest, g->vertex, g->nV);
	if (w < 0) {
		if (g->nV >= g->maxV)
			return false;
		w = (ssize_t) addVertex (dest, g->vertex, g->nV);
		g->nV++;
	}
	g->edges[v][w] = 1;
	g->nE++;

	return true;
}

// isConnected(Graph,Src,Dest)
// - check whether there is an edge from Src->Dest
bool isConnected (Graph g, char *src, char *dest)
{
	assert (g != NULL);
	ssize_t v = vertexID (src, g->vertex, g->nV);
	ssize_t w = vertexID (dest, g->vertex, g->nV);
	if (v < 0 || w < 0)
		return 0;
	return g->edges[v][w] > 0;
}

// nVertices(Graph)
// - return # vertices currently in Graph
size_t nVertices (Graph g)
{
	assert (g != NULL);
	return g->nV;
}

// showGraph(Graph)
// - display Graph
void showGraph (Graph g, int mode)
{
	assert (g != NULL);
	if (g->nV == 0) {
		puts ("Graph is empty");
		return;
	}

	printf ("Graph has %zu vertices:\n", g->nV);
	if (mode == 1) {
		for (size_t i = 0; i < g->nV; i++) {
			for (size_t j = 0; j < g->nV; j++)
				printf ("%f  ", g->edges[i][j]);
			putchar ('\n');
		}
	} else {
		for (size_t i = 0; i < g->nV; i++) {
			printf ("Vertex: %s\n", g->vertex[i]);
			printf ("connects to\n");
			for (size_t j = 0; j < g->nV; j++) {
				if (g->edges[i][j])
					printf ("   %s\n", g->vertex[j]);
			}
		}
	}
}

// display graph, using names for vertices
void showGraph1 (Graph g, char **names)
{
	assert (g != NULL);
	printf ("#vertices=%ld, #edges=%ld\n\n", g->nV, g->nE);
	int v, w;
	for (v = 0; v < g->nV; v++) {
		printf ("%d %s\n", v, names[v]);
		for (w = 0; w < g->nV; w++) {
			if (g->edges[v][w]) {
				printf ("\t%s (%f)\t PR: %f\n", names[w], g->edges[v][w], g->PageRank[w]);
			}
		}
		printf ("\n");
	}
}

void addPageRank (Graph g, char *vertex, double PR) {
	assert (g != NULL);
	int id = vertexID(vertex, List_of_Urls, numUrl);
	g->PageRank[id] = PR;
}

// Helper functions

// vertexID(Str,Names,N)
// - searches for Str in array of Names[N]
// - returns index of Str if found, -1 if not
ssize_t vertexID (char *str, char **names, size_t N)
{
	for (size_t i = 0; i < N; i++)
		if (strEQ (str, names[i]))
			return (ssize_t) i;
	return -1;
}

// addVertex(Str,Names,N)
// - add Str at end of Names
static size_t addVertex (char *str, char **names, size_t N)
{
	names[N] = strdup (str);
	return N;
}

double getPR (Graph g, char *vertex) {
	int id = vertexID(vertex, List_of_Urls, numUrl);
	return g->PageRank[id];
}