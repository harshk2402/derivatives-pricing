CC = g++
CFLAGS = -Wall -ggdb3 -std=c++11

Homework7: Homework7.o BinomialTreeModel.o Option.o
	$(CC) $(CFLAGS) -o Homework7 Homework7.o BinomialTreeModel.o Option.o

Homework7.o: Homework7.cpp BinomialTreeModel.h Option.h BinLattice.h
	$(CC) $(CFLAGS) -c Homework7.cpp

Option.o: Option.cpp Option.h BinomialTreeModel.h BinLattice.h
	$(CC) $(CFLAGS) -c Option.cpp

BinomialTreeModel.o: BinomialTreeModel.cpp BinomialTreeModel.h
	$(CC) $(CFLAGS) -c BinomialTreeModel.cpp

clean:
	rm -rf Homework7 *.o
