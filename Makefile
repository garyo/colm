#
# Makefile for colm - auto-columnation filter
# 5-31-88
#
CFLAGS=-g
LIBS=
LDFLAGS=-g
OBJECTS = colm.o lists.o column.o vcolumn.o readin.o output.o

colm: $(OBJECTS)
	cc $(LDFLAGS) -o $@ $(OBJECTS) $(LIBS)

$(OBJECTS): colm.h
