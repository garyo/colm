/*----------------------------------------------------------------------
 * output - output routines for auto-columnator
 * Copyright c. 1988, Gary Oberbrunner.
 * This program may be freely redistributed providing that this source code
 * and the accompanying copyright notice are preserved intact.
 *----------------------------------------------------------------------
 */

#include "stdio.h"
#include "std.h"
#include "colm.h"

static char SCCSId[] = "@(#)output.c	1.1 6/3/88 (MASSCOMP) 22:44:46";

static OutCol = 0;		/* current output column */

Output(s, maxlen)
char *s;
int maxlen;
{
    OutCol += printf("%.*s", maxlen, s);
}

/* This ignores the opts right now. */
ToCol(tocol, opts)
int tocol;			/* column to move to (org 0) */
OPTS *opts;			/* how to move there */
{
    if (opts->ColSep[0] != '\0')
	printf("%s", opts->ColSep);
    if (opts->Leader == '\0') {	/* no leader, use spaces & tabs */
	while (OutCol < (tocol - tocol % opts->Tabsize)) {
	    putchar('\t');
	    OutCol += opts->Tabsize;
	    OutCol -= OutCol % opts->Tabsize;
	}
    }
    while (OutCol < tocol) {
	putchar(opts->Leader == '\0' ? ' ' : opts->Leader);
	OutCol++;
    }
}

OutNL()
{
    putchar('\n');
    OutCol = 0;
}
