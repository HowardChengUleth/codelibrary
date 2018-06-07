CC=g++
CCFLAGS=-O2 -Wall
LDFLAGS=-static
OBJS=globals.o classes.o util.o uleth.o

all: uleth

uleth: $(OBJS)	
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CC) $(CCFLAGS) -c $<

# generate the prerequistes and append to the desired file
.prereq: $(OBJS:.o=.cpp) $(wildcard *.h) Makefile
	rm -f .prereq
	$(CC) $(CCFLAGS) -MM $(OBJS:.o=.cpp) >> ./.prereq 

# include the generated prerequisite file
include .prereq

clean:
	rm -f *.o *~ uleth
