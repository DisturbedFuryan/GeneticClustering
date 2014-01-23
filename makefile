CC=g++
CFLAGS=-c -std=c++0x -Wall


all: GeneticClustering

GeneticClustering: main.o GeneticClustering.o Individual.o Object.o Population.o Timer.o Utility.o Vector.o
	$(CC) main.o GeneticClustering.o Object.o Individual.o Population.o Timer.o Utility.o Vector.o \
	-lrt -o GeneticClustering

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp
	
GeneticClustering.o: GeneticClustering.cpp
	$(CC) $(CFLAGS) GeneticClustering.cpp

Individual.o: Individual.cpp
	$(CC) $(CFLAGS) Individual.cpp

Object.o: Object.cpp
	$(CC) $(CFLAGS) Object.cpp

Population.o: Population.cpp
	$(CC) $(CFLAGS) Population.cpp

Timer.o: Timer.cpp
	$(CC) $(CFLAGS) Timer.cpp

Utility.o: Utility.cpp
	$(CC) $(CFLAGS) Utility.cpp

Vector.o: Vector.cpp
	$(CC) $(CFLAGS) Vector.cpp

clean:
	rm -rf *.o GeneticClustering
