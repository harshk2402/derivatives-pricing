CC = g++
CFLAGS = -Wall -ggdb3 -std=c++11

hw8: main02.o NonlinearSolver02.o Function02.o
	$(CC) $(CFLAGS) -o hw8 main02.o NonlinearSolver02.o Function02.o

main02.o: main02.cpp Function02.h NonlinearSolver02.h
	$(CC) $(CFLAGS) -c main02.cpp

NonlinearSolver02.o: NonlinearSolver02.cpp NonlinearSolver02.h Function02.h
	$(CC) $(CFLAGS) -c NonlinearSolver02.cpp

Function02.o: Function02.cpp Function02.h
	$(CC) $(CFLAGS) -c Function02.cpp

clean:
	rm -rf hw8 *.o
