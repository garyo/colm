/*----------------------------------------------------------------------
 * ReadIn - read a file into a list of lines.
 * Copyright c. 1988, Gary Oberbrunner.
 * This program may be freely redistributed providing that this source code
 * and the accompanying copyright notice are preserved intact.
 *----------------------------------------------------------------------
 */

#include <string.h>
#include <stdio.h>
#include "std.h"
#include "colm.h"
#include "lists.h"

#define MAX_LEN 20000		/* maximum string length */

extern int debug;

static char SCCSId[] = "@(#)readin.c	1.1 6/3/88 (MASSCOMP) 22:44:47";

int ReadIn(const char *fname, NODE *list)
{
    char Instr[MAX_LEN];
    FILE *infile = NULL;
    int TotalLines = 0;		/* total # of lines read */

    if (!strcmp(fname, "-"))
	infile = stdin;
    else if ((infile = fopen(fname, "r")) == NULL) {
	fprintf(stderr,"Can't open file %s\n.", fname);
	exit(9);
    }
    while (fgets(Instr, MAX_LEN, infile)) {
	if (*Instr == '\0') {
	    fprintf(stderr,
		    "Error: Null found in supposedly line-oriented data.\n");
	    exit(2);
	}
	TotalLines++;
	Instr[strlen(Instr)-1] = '\0'; /* strip newline */
	append(Instr, list); /* add it to the list */
	assert(list->prev != NULL);
    }
    return TotalLines;
}
