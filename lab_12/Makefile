CPP  = clang++
OPTS = -g -Wall -W -Wunused -Wuninitialized -Wshadow -std=c++11


genealogy: genealogy.o 
	$(CPP) $(OPTS) -o genealogy genealogy.o 
	./genealogy

genealogy.o: genealogy.cpp Makefile
	$(CPP) $(OPTS) -c genealogy.cpp -o genealogy.o
