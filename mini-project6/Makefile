CC = g++
CFLAGS = -Wall -ggdb3 -std=c++11

HW06: Homework6.o BinomialTreeModel.o Option.o
	$(CC) $(CFLAGS) -o Homework6 Homework6.o BinomialTreeModel.o Option.o

Homework06.o: Homework6.cpp BinomialTreeModel.h Option.h BinLattice.h
	$(CC) $(CFLAGS) -c Homework6.cpp

Option08.o: Option.cpp Option.h BinomialTreeModel.h BinLattice.h
	$(CC) $(CFLAGS) -c Option.cpp

BinomialTreeModel.o: BinomialTreeModel.cpp BinomialTreeModel.h
	$(CC) $(CFLAGS) -c BinomialTreeModel.cpp

clean:
	rm -rf HW06 *.o
