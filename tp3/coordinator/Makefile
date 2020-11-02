all:
	g++ -g -I. -c -o coordinator.o coordinator.cpp
	g++ -lpthread coordinator.o -o coordinator
clean:
	rm -f coordinator
	rm -f *.o
