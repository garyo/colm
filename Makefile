#
# Makefile for colm - auto-columnation filter
# 5-31-88
#
SCCSID="@(#)makefile	1.1 6/3/8822:49:03"
CFLAGS=-g -I/usr/garyo/lib
LIBS=
LDFLAGS=-g
OBJECTS = colm.o lists.o column.o vcolumn.o readin.o output.o

colm: $(OBJECTS)
	cc $(LDFLAGS) -o $@ $(OBJECTS) $(LIBS)

$(OBJECTS): colm.h
