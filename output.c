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
#include "output.h"

static char SCCSId[] = "@(#)output.c	1.1 6/3/88 (MASSCOMP) 22:44:46";

static int OutCol = 0;		/* current output column */

void Output(const char *s, int maxlen)
{
    OutCol += printf("%.*s", maxlen, s);
}

/* This ignores the opts right now. */
void ToCol(int tocol, const OPTS *opts)
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

void OutNL()
{
    putchar('\n');
    OutCol = 0;
}
