/* Standard include file with lots of generally useful stuff */
/* Copyright c. 1988, Gary Oberbrunner. */
/* This program may be freely redistributed providing that this source code */
/* and the accompanying copyright notice are preserved intact. */
/* SCCS Id: @(#)std.h	1.2 1/21/88 19:03:14 */

#include <stdlib.h>
#include <assert.h>

#define min(x,y) ((x) < (y) ? (x) : (y))
#define max(x,y) ((x) > (y) ? (x) : (y))
#define clamp(x, mn, mx) (((x) <= (mn)) ? (mn) : (((x) >= (mx)) ? (mx) : (x)))

#define dbug0(str) \
    if (debug) fprintf(stderr, str);
#define dbug1(str, arg1) \
    if (debug) fprintf(stderr, str, arg1);
#define dbug2(str, arg1, arg2) \
    if (debug) fprintf(stderr, str, arg1, arg2);
#define dbug3(str, arg1, arg2, arg3) \
    if (debug) fprintf(stderr, str, arg1, arg2, arg3);
#define dbug4(str, arg1, arg2, arg3, arg4) \
    if (debug) fprintf(stderr, str, arg1, arg2, arg3, arg4);

/*
 * NEW is a macro to malloc 'n' new variables of type 'type'.
 */
#define NEW(var, type, num) \
    if ((var = (type *) malloc((num) * sizeof(type)))==NULL) \
	{	fprintf(stderr,\
	    "ERROR: Out of memory.\nLast request:\n");\
	fprintf(stderr,\
	    "\tNumber: num\n\tType  : type\n\tName  : var.\n");\
	fprintf(stderr,"File %s, line %d\n", __FILE__, __LINE__);\
	fprintf(stderr,\
	"\tTotal requested: num=%d x sizeof(type)=%ul\n\t =%ul bytes.\n",\
                (int)num, (int)sizeof(type), (int)((num) * sizeof(type))); \
	exit(99); }
