CC = g++
CFLAGS = -Wall -ggdb3 -std=c++11

Homework05: Homework05.o BinomialTreeModel.o Option.o DigitOpt.o SpreadOption.o
	$(CC) $(CFLAGS) -o Homework05 Homework05.o BinomialTreeModel.o Option.o DigitOpt.o SpreadOption.o

Homework05.o: Homework05.cpp BinomialTreeModel.h Option.h DigitOpt.h SpreadOption.h
	$(CC) $(CFLAGS) -c Homework05.cpp

Option.o: BinomialTreeModel.h Option.h Option.cpp
	$(CC) $(CFLAGS) -c Option.cpp

BinomialTreeModel.o: BinomialTreeModel.h BinomialTreeModel.cpp
	$(CC) $(CFLAGS) -c BinomialTreeModel.cpp

DigitOpt.o: DigitOpt.h DigitOpt.cpp Option.h
	$(CC) $(CFLAGS) -c DigitOpt.cpp

SpreadOption.o: SpreadOption.cpp SpreadOption.h Option.h
	$(CC) $(CFLAGS) -c SpreadOption.cpp

clean:
	rm -rf Homework05 *.o