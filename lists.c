/*----------------------------------------------------------------------
 * These functions append to and search a doubly-linked, non-circular
 * list of text lines.
 * Copyright c. 1988, Gary Oberbrunner.
 * This program may be freely redistributed providing that this source code
 * and the accompanying copyright notice are preserved intact.
 *----------------------------------------------------------------------
 */

#include <stdio.h>
#include <string.h>
#include "std.h"
#include "colm.h"

extern int debug;

static char SCCSId[] = "@(#)lists.c	1.1 6/3/88 (MASSCOMP) 22:44:45";

/* Declare the empty line list: */
/* Note that this header node is NOT part of the list. */
NODE Lines = { &Lines, &Lines, 0, NULL };

/* Append a new string to the list: */
append(str, list)
char *str;
NODE *list;
{
    register NODE *new;

    assert(str != NULL);
    assert(list != NULL);

    NEW(new, NODE, 1);		/* get a node */
    NEW(new->string, char, strlen(str) + 1); /* get string storage */

    strcpy(new->string, str);	/* copy the string into the storage */
    new->length = strlen(str);	/* store the length, for speed */

    new->next = list;		/* link it onto the end of the list. */
    new->prev = list->prev;
    new->prev->next = new;
    list->prev = new;
}

/* Linearly search the next 'n' items of the list for longest string */
/* If n is negative, the search goes forever. */
int
findmax(list, n)
NODE *list;
int n;
{
    register NODE *current = list->next;
    NODE *longest = current;

    assert(list != NULL);
    assert(list->next != NULL);
    assert(list->prev != NULL);

    for ( ; current != list && n != 0; current = current->next) {
        if (current->length > longest->length)
	    longest = current;
	if (n > 0) n--;
    }
    assert(longest != NULL);
    return longest->length;
}

/* Add up the lengths of all the strings in the list. */
/* If n > 0, look only at the next 'n' items. */
/* If n < 0, look through to the end. */
int
total_chars(list, n)
NODE *list;
int n;
{
    register NODE *current;
    int total = 0;

    assert(list != NULL);
    assert(list->next != NULL);
    assert(list->prev != NULL);

    for (current = list->next; current != list && n != 0;
	 current = current->next) {
	total += current->length;
	if (n > 0) n--;
    }
    return total;
}

NODE *
findn(list, n)
NODE *list;
int n;
{
    register NODE *current = list->next;

    assert(list != NULL);
    assert(list->next != NULL);
    assert(list->prev != NULL);

    for ( ; current != list && n != 0; current = current->next, n--)
	;
    assert(current != list);
    return current;
}
