TwoPipesTwoChildren TwoPipesThreeChildren dynpipe : TwoPipesTwoChildren.o TwoPipesThreeChildren.o DynPipe.o
	g++ -o TwoPipesTwoChildren TwoPipesTwoChildren.o
	g++ -o TwoPipesThreeChildren TwoPipesThreeChildren.o
	g++ -o dynpipe DynPipe.o
TwoPipesTwoChildren.o TwoPipesThreeChildren.o DynPipe.o :
	g++ -c TwoPipesTwoChildren.cpp
	g++ -c TwoPipesThreeChildren.cpp
	g++ -c DynPipe.cpp
clean:
	rm -f core TwoPipesTwoChildren TwoPipesTwoChildren.o
	rm -f core TwoPipesThreeChildren TwoPipesThreeChildren.o
	rm -f core dynpipe DynPipe.o

