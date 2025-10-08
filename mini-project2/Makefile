CC = g++
CFLAGS = -Wall -ggdb3 -std=c++11

OptionPrice02: OptionPrice02.o BinomialTreeModel.o Option02.o
	$(CC) $(CFLAGS) -o OptionPrice02 OptionPrice02.o BinomialTreeModel.o Option02.o

OptionPrice02.o: OptionPrice02.cpp BinomialTreeModel.h Option02.h
	$(CC) $(CFLAGS) -c OptionPrice02.cpp

Option02.o: BinomialTreeModel.h Option02.h Option02.cpp
	$(CC) $(CFLAGS) -c Option02.cpp

BinomialTreeModel.o: BinomialTreeModel.h BinomialTreeModel.cpp
	$(CC) $(CFLAGS) -c BinomialTreeModel.cpp

clean:
	rm -rf OptionPrice02 *.o