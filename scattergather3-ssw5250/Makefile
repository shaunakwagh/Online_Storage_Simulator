#
# CMPSC311 - ScatterGather.co, - Assignment #3
# Makefile - makefile to build the base system
#

# Locations

# Make environment
INCLUDES=-I.
CC=gcc
CFLAGS=-I. -c -g -Wall $(INCLUDES)
LINKARGS=-g
LIBS=-lm -lcmpsc311 -L. -lgcrypt -lpthread -lcurl
AR=ar

# Suffix rules
.SUFFIXES: .c .o

.c.o:
	$(CC) $(CFLAGS)  -o $@ $<
	
# Files
OBJECT_FILES=	sg_sim.o \
				sg_driver.o \
				
SGLIB=libsglib.a

# Productions
all : sg_sim

sg_sim : $(OBJECT_FILES) $(SGLIB)
	$(CC) $(LINKARGS) $(OBJECT_FILES) -o $@ -lsglib $(LIBS)

test:
	./sg_sim -v cmpsc311-assign3-workload.txt

debug:
	gdb ./sg_sim -ex "r -v cmpsc311-assign3-workload.txt"

valgrind:
	valgrind ./sg_sim -v cmpsc311-assign3-workload.txt

clean : 
	rm -f sg_sim $(OBJECT_FILES)
	
