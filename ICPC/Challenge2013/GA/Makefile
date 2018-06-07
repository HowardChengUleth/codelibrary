all: init nextgen rungame run_sequential

init: init.o util.o Chromosome.o
	g++ -o init init.o util.o Chromosome.o -Wall -O2 -static

init.o: init.cc util.h Chromosome.h
	g++ -c init.cc -Wall -O2

nextgen: nextgen.o util.o Chromosome.o
	g++ -o nextgen nextgen.o util.o Chromosome.o -Wall -O2 -static

nextgen.o: nextgen.cc util.h Chromosome.h
	g++ -c nextgen.cc -Wall -O2

rungame: rungame.o util.o Chromosome.o
	g++ -o rungame rungame.o util.o Chromosome.o -Wall -O2 -static

rungame.o: rungame.cc util.h
	g++ -c rungame.cc -Wall -O2

util.o: util.h util.cc
	g++ -c util.cc -Wall -O2

run_sequential: run_sequential.cc util.o Chromosome.o
	g++ -o run_sequential run_sequential.cc util.o Chromosome.o -Wall -O2

run_parallel: run_parallel.cc util.o Chromosome.o
	g++ -o run_parallel run_parallel.cc util.o Chromosome.o -Wall -O2

Chromosome.o: util.h Chromosome.h Chromosome.cc
	g++ -c Chromosome.cc -Wall -O2

clean:
	rm -f *.o init nextgen rungame run_sequential rungame *~
