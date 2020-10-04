all:
	g++ -g -I. -c -o sendSignal.o sendSignal.cpp
	g++ sendSignal.o -o sendSignal
	g++ -g -I. -c -o receiveSignal.o receiveSignal.cpp
	g++ receiveSignal.o -o receiveSignal
clean:
	rm -f sendSignal
	rm -f receiveSignal
	rm -f *.o
