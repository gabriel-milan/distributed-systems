all:
	g++ -g -I. -c -o consumer.o consumer.cpp
	g++ consumer.o -o consumer
	g++ -g -I. -c -o producer.o producer.cpp
	g++ producer.o -o producer
clean:
	rm -f consumer
	rm -f producer
	rm -f *.o
