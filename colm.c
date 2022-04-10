/*----------------------------------------------------------------------
 * colm: auto-columnate, with various options.
 * by Gary Oberbrunner  5-31-88
 *
 * -v		variable-width columns (default: fixed-width)
 * -s		spread output columns to fit output width (default: don't)
 * -g #		Separate columns by a #-column gutter
 * -c #		column width (size) (default: max line length of input)
 * -n #		use # columns (default: program picks max # that will fit)
 * -w #		output width is # characters/line (default: 80)
 * -l #		page length is # lines (default: infinite)
 * -L c		leader character is 'c' (default: space)
 * -t s		use string "s" as separator; (default: enough space to line up)
 * -T #		tab size # chars (default 8)
 *
 * Copyright c. 1988, Gary Oberbrunner.
 * This program may be freely redistributed providing that this source code
 * and the accompanying copyright notice are preserved intact.
 *----------------------------------------------------------------------
 */

#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include "std.h"
#include "colm.h"
#include "column.h"
#include "readin.h"

int debug = 1;
static char SCCSId[] = "@(#)colm.c	1.2 6/3/88 (MASSCOMP) 23:17:12";
static char Copyright[] = "Copyright c. 1988, Gary Oberbrunner.";
void Usage();


OPTS Opts;			/* global copy of original options */
int main (argc, argv)
int argc;
char **argv;
{
    int	c;
    extern int optind;
    extern char	*optarg;
    int errflg = 0;
    int totlines = 0;

    /* Command-line options: */
    OPTS opts;
    opts.Vflg = FALSE;		/* variable col width */
    opts.Sflg = FALSE;		/* stretch output columns to fit */
    opts.Gutter = 1;		/* gutter width */
    opts.ColWidth = 0;		/* fixed col width */
    opts.Ncols = 0;		/* number of cols */
    opts.Width = 79;		/* output width */
    opts.Tabsize = 8;		/* tab size */
    opts.PageLen = 0;		/* page length */
    opts.ColSep[0] = '\0';	/* column separator string */
    opts.Leader = '\0';		/* leader char for column separation */

    while ((c =	getopt (argc, argv, "vsg:c:n:w:l:L:t:T:")) != EOF) {
	switch	(c) {
	  case 'v':
	    if (opts.ColWidth != 0) errflg++;
	    opts.Vflg = TRUE;
	    break;
	  case 's':
	    opts.Sflg = TRUE;
	    break;
	  case 'c':
	    if (opts.Vflg == TRUE) errflg++;
	    opts.ColWidth = atoi(optarg);
	    if (opts.ColWidth < 1) errflg++;
	    break;
	  case 'g':
	    opts.Gutter = atoi(optarg);
	    if (opts.Gutter < 0) errflg++;
	    break;
	  case 'n':
	    opts.Ncols = atoi(optarg);
	    if (opts.Ncols < 0) errflg++;
	    break;
	  case 'w':
	    opts.Width = atoi(optarg);
	    if (opts.Width < 1) errflg++;
	    break;
	  case 'l':
	    opts.PageLen = atoi(optarg);
	    if (opts.PageLen < 1) errflg++;
	    break;
	  case 'T':
	    opts.Tabsize = atoi(optarg);
	    if (opts.Tabsize < 1) errflg++;
	    break;
	  case 'L':
	    opts.Leader = optarg[0];
	    break;
	  case 't':
	    strcpy(opts.ColSep, optarg);
	    break;
	  case '?':
	    errflg++;
	}
    }
    Opts = opts;		/* copy to global, so we know what was */
				/* actually user-specified and what we */
				/* computed */
    if (errflg)	{
	Usage();
	exit (2);
    }
    if (optind >= argc)
	totlines = ReadIn("-", &Lines); /* read stdin */
    else {
	for( ; optind < argc; optind++) {
	    totlines += ReadIn(argv[optind], &Lines); /* read text files */
	}
    }
    if (totlines > 0)
	Columnate(&Lines, &opts, totlines);
}

void Usage()
{
    fprintf(stderr,"\n\
Usage: colm [-cglnstvwLT] <files>\n\
 Use '-' for stdin.\n\
 Options:\n\
 -v	variable-width columns (default: fixed-width)\n\
 -s	spread columns to fit output width (default: don't spread)\n\
 -g #	separate columns by a #-character 'gutter'.\n\
 -c #	column width (size) (default: max line length of input)\n\
 -n #	use # columns (default: program picks max # that will fit)\n\
 -w #	output width is # characters/line (default: 80)\n\
 -l #	page length is # lines (default: infinite)\n\
 -L c	leader character is 'c' (default: space)\n\
 -t s	use string \"s\" as separator (default: enough space to line up)\n\
 -T #	use #-space tabs (default: 8)\n");
}
