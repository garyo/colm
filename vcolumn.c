/*----------------------------------------------------------------------
 * vcolumn.c - columnates with variable-size columns
 * Copyright c. 1988, Gary Oberbrunner.
 * This program may be freely redistributed providing that this source code
 * and the accompanying copyright notice are preserved intact.
 *----------------------------------------------------------------------
 */

#include <stdio.h>
#include "std.h"
#include "colm.h"
#include "lists.h"
#include "output.h"
#include "vcolumn.h"

static char SCCSId[] = "@(#)vcolumn.c	1.2 6/3/88 (MASSCOMP) 23:11:49";

extern int debug;
static void ClearWidths(int w[], int nc);
static BOOLEAN VariCol(NODE *list, OPTS *opts,
		       int textlines, int outlines, int widths[]);
static void OutVCols(NODE *list, OPTS *opts, int textlines, int outlines, int widths[]);

BOOLEAN
    ManVarColumns(NODE *list, OPTS *opts, int totlines)
{
    int outlines;
    int widths[MAX_COLS];	/* column-width table */
    register NODE *current;
    register int line, col;
    int twidth = 0;

    assert(opts->Ncols > 0);
    outlines = (totlines + opts->Ncols - 1) / opts->Ncols;

    ClearWidths(widths, MAX_COLS);
    for (current = list->next, col = 0, line=0;
	 current != list && totlines != 0;
	 current = current->next, totlines--) {
	if (current->length > widths[col])
	    widths[col] = current->length;
	/* Do next line, and if at bottom start next column */
	if (++line >= outlines) {
	    col++;
	    line = 0;
	}
    }
    for (col = 0; col < opts->Ncols; col++)
	if (col != opts->Ncols -1)
	    widths[col] += opts->Gutter;
    twidth += widths[col];
    if (twidth > opts->Width) {
	fprintf(stderr,"\
%d text columns won't fit into output width of %d chars; %d chars too wide.\n",
		opts->Ncols, opts->Width, twidth - opts->Width);
	return FALSE;
    }
    OutVCols(list, opts, totlines, outlines, widths);
    return TRUE;
}

BOOLEAN
    AutoVarColumns(NODE *list, OPTS *opts, int totlines)
{
    int widths[MAX_COLS];	/* column-width table */
    register int outlines;
    int avglen;

    /* We start with the smallest possible number of output lines and */
    /* successively increase by ones until it works. */

    /* First, compute the smallest possible outlines: */
    outlines = max(1, total_chars(list, totlines) / opts->Width);

    while (1) {
	assert(outlines <= totlines); /* runaway check */
	ClearWidths(widths, MAX_COLS);
	if (VariCol(list, opts, totlines, outlines, widths) == TRUE)
	    break;
	outlines++;
    }
    /* Now opts->Ncols is correct, and VariCol has filled in widths[]. */
    OutVCols(list, opts, totlines, outlines, widths);
    return TRUE;
}

static void ClearWidths(int w[], int nc)
{
    assert (nc > 0);
    while (nc--)
	w[nc] = 0;
}

static BOOLEAN VariCol(NODE *list, OPTS *opts, int textlines, int outlines, int widths[])
{
    register int col;		/* current column */
    register int line;		/* current out line */
    register int twidth = 0;
    register NODE *current;

    for (current = list->next, col = 0, line=0;
	 current != list && textlines != 0;
	 current = current->next, textlines--) {
	if (current->length > widths[col])
	    widths[col] = current->length;
	/* Do next line, and if at bottom start next column */
	if (++line >= outlines) {
	    col++;
	    line = 0;
	}
    }
    if (line != 0) col++;
    opts->Ncols = col;
    for (col = 0; col < opts->Ncols; col++) {
	if (col != opts->Ncols -1)
	    widths[col] += opts->Gutter;
	twidth += widths[col];
    }
    if (twidth <= opts->Width) { /* we made it */
	/* Partition the extra space as evenly as possible */
	int extra_chars = opts->Width - twidth;
	while (extra_chars > 0 && opts->Ncols > 1 && opts->Sflg == TRUE)
	    for (col = 0; col < opts->Ncols-1 && extra_chars > 0; col++) {
		widths[col]++;
		extra_chars--;
	    }
	return (TRUE);
    }
    return FALSE;
}

static void OutVCols(NODE *list, OPTS *opts, int textlines, int outlines, int widths[])
{
    register int col, line;
    int ccol;			/* current character column position */
    NODE *cols[MAX_COLS];

    /* Get pointers to text lines, one for each column */
    for (col = 0; col < opts->Ncols; col++) {
	cols[col] = findn(list, outlines * col);
    }
    for (line = 0; line < outlines; line++) {
	ccol = 0;
	for (col = 0; col < opts->Ncols; col++) {
	    if (cols[col] != list) {
		Output(cols[col]->string, widths[col]);
		if (col != opts->Ncols - 1 && cols[col+1] != list)
		    ToCol(ccol += widths[col], opts);
		cols[col] = cols[col]->next;
	    }
	}
	OutNL();
    }
}

BOOLEAN VarColumns(NODE *list, OPTS *opts, int totlines)
{
    assert(Opts.ColWidth == 0);
    /* Only auto-columnate if she didn't say how many columns, or if */
    /* the requested number of columns fails. */
    if (Opts.Ncols == 0 || ManVarColumns(list, opts, totlines) == FALSE)
        return AutoVarColumns(list, opts, totlines);
    else
        return 1;
}
