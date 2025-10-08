CC = g++
CFLAGS = -Wall -ggdb3 -std=c++11

HW10: main.o Model.o PathDepOption.o
	$(CC) $(CFLAGS) -o HW10 main.o Model.o PathDepOption.o

main.o: main.cpp Model.h PathDepOption.h
	$(CC) $(CFLAGS) -c main.cpp

PathDepOption.o: Model.h PathDepOption.h PathDepOption.cpp
	$(CC) $(CFLAGS) -c PathDepOption.cpp
	
Model.o: Model.h Model.cpp
	$(CC) $(CFLAGS) -c Model.cpp
	
clean:
	rm -rf  HW10 *.o



