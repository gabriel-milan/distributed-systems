all:
	g++ -g -I. -c -o client.o client.cpp
	g++ -lpthread client.o -o client
clean:
	rm -f client
	rm -f *.o
