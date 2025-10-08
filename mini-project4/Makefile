CC = g++
CFLAGS = -Wall -ggdb3 -std=c++11

Homework04: Homework04.o BinomialTreeModel.o Option.o
	$(CC) $(CFLAGS) -o Homework04 Homework04.o BinomialTreeModel.o Option.o

Homework04.o: Homework04.cpp BinomialTreeModel.h Option.h
	$(CC) $(CFLAGS) -c Homework04.cpp 

Option.o: Option.cpp Option.h BinomialTreeModel.h  
	$(CC) $(CFLAGS) -c Option.cpp
	
BinomialTreeModel.o: BinomialTreeModel.cpp BinomialTreeModel.h
	$(CC) $(CFLAGS) -c BinomialTreeModel.cpp
	
clean:
	rm -rf Homework04 *.o

