/*----------------------------------------------------------------------
 * Header file for auto-columnator
 * Copyright c. 1988, Gary Oberbrunner.
 * This program may be freely redistributed providing that this source code
 * and the accompanying copyright notice are preserved intact.
 * SCCS ID: @(#)colm.h	1.1 6/3/88 22:47:12
 *----------------------------------------------------------------------
 */

#define MAX_COLS 1000		/* maximum number of columns */

#define TRUE 1
#define FALSE 0

typedef int BOOLEAN;

/* A doubly-linked list of text lines */
typedef struct NODE {
    struct NODE *next;
    struct NODE *prev;
    int length;
    char *string;
} NODE;

extern NODE *findn();

extern NODE Lines;		/* the head of the list */
				/* defined in lists.c */

extern int TotalLines;		/* total # of lines read */
				/* defined in readin.c */

/* Command-line options: */
typedef struct OPTS {
    int Vflg;			/* variable col width */
    int Sflg;			/* stretch output columns to fit */
    int Gutter;			/* blank space between columns */
    int ColWidth;		/* fixed col width */
    int Ncols;			/* number of cols */
    int Width;			/* output width */
    int PageLen;		/* page length */
    int Tabsize;		/* tab size (normally 8) */
    char ColSep[100];		/* separator string */
    char Leader;		/* leader char for column separation */
} OPTS;

extern OPTS Opts;		/* defined in colm.c */
