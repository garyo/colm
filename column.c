/*----------------------------------------------------------------------
 * column.c - does the actual work of columnating the list of lines
 * Copyright c. 1988, Gary Oberbrunner.
 * This program may be freely redistributed providing that this source code
 * and the accompanying copyright notice are preserved intact.
 *----------------------------------------------------------------------
 */

#include <stdio.h>
#include "std.h"
#include "colm.h"
#include "column.h"
#include "vcolumn.h"
#include "output.h"
#include "lists.h"

extern int debug;

static char SCCSId[] = "@(#)column.c	1.1 6/3/88 (MASSCOMP) 22:44:43";

/* returns TRUE or FALSE, according to whether the text */
/* will fit on the page. */
/* FixedColumns() requires that ColWidth, NCols and Width be set. */
static BOOLEAN FixedColumns(NODE *list, OPTS *opts, int totlines)
{
    NODE *cols[MAX_COLS];	/* NODE pointers for each column */
    register int i,j;
    int outlines;		/* total # of output lines */

    /* This is the easy case - just put out the lines. */

    assert(opts->ColWidth > 0 && opts->Ncols > 0);
    /* We only print an error message here if the user actually */
    /* specified (via capital-O Opts) a number of columns and width. */
    if (Opts.Ncols > 0 && Opts.ColWidth > 0 &&
	opts->ColWidth * opts->Ncols > opts->Width) {
	fprintf(stderr,
		"%d columns of %d characters won't fit in output width",
		opts->Ncols, opts->ColWidth);
	fprintf(stderr," of %d chars.\n", opts->Width);
	exit(3);
    }
    /* It's not always possible to form up proper columns from a given */
    /* number of input lines; 'proper' here means all columns even */
    /* except the last, which can be as short as one element.  For */
    /* instance, five text lines can't be formed into four columns. */
    /* But they can be formed into three columns of two each, except */
    /* the last column which will contain only one line. */
    /* Here we always opt for using fewer columns instead. */
    /* We spread the extra space (if any) out over all the columns. */

    outlines = (totlines + opts->Ncols - 1) / opts->Ncols;

    if ((totlines + outlines-1) / outlines != opts->Ncols) {
	int extraspace = opts->ColWidth
	    * (opts->Ncols - (totlines + outlines-1) / outlines);
	/* Here again, we only print the error message if the user */
	/* explicitly requested (via Opts) a number of columns.  If we */
	/* figured it out, we don't need to tell her that we guessed */
	/* wrong. */
	if (Opts.Ncols > 0) {
	    fprintf(stderr,"Can't form %d text lines into %d proper columns; ",
		    totlines, opts->Ncols);
	    fprintf(stderr,"using %d columns instead.\n",
		    (totlines + outlines-1) / outlines);
	}
	opts->Ncols = (totlines + outlines-1) / outlines;
	/* Only spread columns if cwidth wasn't explicitly specified. */
	if (Opts.ColWidth == 0 && opts->Sflg == TRUE)
	    opts->ColWidth += extraspace / opts->Ncols;
	return FixedColumns(list, opts, totlines);
    }
    if (opts->PageLen > 0 && outlines > opts->PageLen) return FALSE;
    for (i = 0; i < opts->Ncols; i++)
	cols[i] = findn(list, i * outlines);

    for (j = 0; j < outlines; j++) {
	for (i = 0; i < opts->Ncols; i++) {
	    if (cols[i] != list) {
		Output(cols[i]->string, opts->ColWidth);
		if (i != opts->Ncols - 1 && cols[i+1] != list)
		    ToCol(opts->ColWidth * (i+1), opts);
		cols[i] = cols[i]->next;
	    }
	}
	OutNL();
    }
    return TRUE;
}

/* SinglePageCol returns TRUE or FALSE, according to whether the text */
/* will fit on the page. */
static BOOLEAN
SinglePageCol(NODE *list, OPTS *opts, int totlines)
{
    int try_again = TRUE;
    while (try_again) {
	try_again = FALSE;

	if (opts->ColWidth > 0 && opts->Ncols > 0)
	    return FixedColumns(list, opts, totlines);
	if (opts->Vflg == TRUE)
	    return VarColumns(list, opts, totlines);
	if (opts->Ncols > 0 && opts->Sflg == TRUE  && opts->ColWidth == 0) {
	    opts->ColWidth = opts->Width / opts->Ncols;
	    return FixedColumns(list, opts, totlines);
	}
	/* We can figure out the column width by finding the largest */
	/* string.  Once we find that, we process again to pick up any */
	/* other options. */
	if (opts->ColWidth == 0) {
	    opts->ColWidth = findmax(list, -1) + opts->Gutter;
	    try_again = TRUE;
	}
	/* If we know the column width and the output width, we can */
	/* figure out the number of columns. */
	if (opts->ColWidth > 0 && opts->Width > 0 && opts->Ncols == 0) {
	    opts->Ncols = opts->Width / opts->ColWidth;
	    try_again = TRUE;
	}
    }
    return TRUE;
}

BOOLEAN
static MultiPageCol(NODE *list, OPTS *opts, int totlines)
{
    fprintf(stderr,"Can't do multi-page columnation (yet).\n");
    return FALSE;
}

/* Returns success or failure. */
BOOLEAN
Columnate(NODE *list, OPTS *opts, int totlines)
{
    /* First, check to make sure it's possible to columnate the output. */
    if (findmax(list, totlines) > opts->Width) {
	fprintf(stderr, "\
Error: Some input line is longer than the output width of %d chars.\n",
		opts->Width);
	exit(1);
    }
    if (!SinglePageCol(list, opts, totlines))
	return MultiPageCol(list, opts, totlines);
    else return TRUE;
}
