all:
	g++ -g -I. -c -o spinlock.o spinlock.cpp 
	g++ -pthread  spinlock.o -o spinlock
	g++ -g -I. -c -o generateData.o generateData.cpp 
	g++ -pthread  generateData.o -o generateData
	  
clean:
	rm -f spinlock    rm -f generateData
	rm -f *.o

