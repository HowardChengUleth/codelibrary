FLAGS= # -DDEBUG # -DCGL_DEBUG


uleth: uleth.cpp board.h player.h util.h heuristics.h ClosestGoldLookahead.hh Makefile
	g++ uleth.cpp $(FLAGS) -Wall -O2 -o uleth

%.cpp %.h %.hh:
	touch $@

clean:
	rm -f *~ *.o *\#
