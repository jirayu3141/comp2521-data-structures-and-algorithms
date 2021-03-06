// crawl.c ... build a graph of part of the web
// Written by John Shepherd, September 2015
// Uses the cURL library and functions by Vincent Sanders
// <vince@kyllikki.org>

#include <ctype.h>
#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "graph.h"
#include "html.h"
#include "set.h"
#include "stack.h"
#include "url_file.h"
#include "queue.h"
#define BUFSIZE 1024

static void setFirstURL (char *, char *);
URL_FILE* openLink (char* url, char* nextUrlBuff);


int main (int argc, char **argv)
{
	URL_FILE *handle;
	char buffer[BUFSIZE];
	char baseURL[BUFSIZE];
	char firstURL[BUFSIZE];
	char next[BUFSIZE];
	int maxURLs;

	if (argc > 2) {
		strcpy (baseURL, argv[1]);
		setFirstURL (baseURL, firstURL);
		maxURLs = atoi (argv[2]);
		if (maxURLs < 40)
			maxURLs = 40;
	} else {
		fprintf (stderr, "Usage: %s BaseURL MaxURLs\n", argv[0]);
		exit (1);
	}

	// You need to modify the code below to implement:
	//
	// add firstURL to the ToDo list
	// initialise Graph to hold up to maxURLs
	// initialise set of Seen URLs
	// while (ToDo list not empty and Graph not filled) {
	//    grab Next URL from ToDo list
	//    if (not allowed) continue
	//    foreach line in the opened URL {
	//       foreach URL on that line {
	//          if (Graph not filled or both URLs in Graph)
	//             add an edge from Next to this URL
	//          if (this URL not Seen already) {
	//             add it to the Seen set
	//             add it to the ToDo list
	//          }
	//       }
	//    }
	//    close the opened URL
	//    sleep(1)
	// }

	// add firstURL to the ToDo list
	Queue ToDo = newQueue();
	enterQueue(ToDo, firstURL);
	//initialise Graph to hold up to maxURLs
	Graph myGraph = newGraph((size_t)maxURLs);
	//initialise set of seen URL
	Set seenURL = newSet();
	insertInto(seenURL, firstURL);
	// showQueue(ToDo);
	// showSet(seenURL);

	handle = openLink(firstURL, next);

	while (!emptyQueue(ToDo) && nVertices(myGraph) <= (size_t)maxURLs) { 
		strcpy(firstURL, leaveQueue(ToDo));
		handle = openLink(firstURL, next);
		
		
		while (!url_feof (handle)) {	//while in page
			url_fgets (buffer, sizeof (buffer), handle); //reading lines in html
			// fputs(buffer,stdout);
			int pos = 0;
			char result[BUFSIZE];
			memset (result, 0, BUFSIZE);
			//found html tag
			while ((pos = GetNextURL (buffer, firstURL, result, pos)) > 0) {
				printf ("Found: '%s'\n", result);
				if (nVertices(myGraph) <= (size_t)maxURLs) {
					addEdge(myGraph, firstURL, result);
				}
				if (isElem(seenURL, firstURL) == false) {
					insertInto(seenURL, result);
				}
				memset (result, 0, BUFSIZE);
			}
		}
		url_fclose (handle);
        free(firstURL);
        sleep (1);
	}
	showGraph(myGraph,1);
    dropQueue(ToDo);
    dropSet(seenURL);
    dropGraph(myGraph);
	return 0;
}

// setFirstURL(Base,First)
// - sets a "normalised" version of Base as First
// - modifies Base to a "normalised" version of itself
static void setFirstURL (char *base, char *first)
{
	char *c;
	if ((c = strstr (base, "/index.html")) != NULL) {
		strcpy (first, base);
		*c = '\0';
	} else if (base[strlen (base) - 1] == '/') {
		strcpy (first, base);
		strcat (first, "index.html");
		base[strlen (base) - 1] = '\0';
	} else {
		strcpy (first, base);
		strcat (first, "/index.html");
	}
}


URL_FILE* openLink (char* url, char* nextUrlBuff) {
	URL_FILE *handle;
	if (!(handle = url_fopen (url, "r"))) {
		fprintf (stderr, "Couldn't open %s\n", nextUrlBuff);
		exit (1);
	}
	return handle;
}
