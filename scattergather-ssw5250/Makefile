#
# CMPSC311 - ScatterGather.co, - Assignment #2
# Makefile - makefile to build the base system
#

# Locations

# Make environment
INCLUDES=-I.
CC=gcc
CFLAGS=-I. -c -g -Wall $(INCLUDES)
LINKARGS=-g
LIBS=-L. -lsglib -lm -lcmpsc311 -lgcrypt -lpthread -lcurl 

# Suffix rules
.SUFFIXES: .c .o

.c.o:
	$(CC) $(CFLAGS)  -o $@ $<
	
# Files
OBJECT_FILES=	sg_sim.o \
				sg_driver.o \
				
# Productions
all : sg_sim

sg_sim : $(OBJECT_FILES) $(SGLIB)
	$(CC) $(LINKARGS) $(OBJECT_FILES) -o $@ $(LIBS)

clean : 
	rm -f sg_sim $(OBJECT_FILES)
	
