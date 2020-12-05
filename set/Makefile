output: main.o set.o
	g++ -o output main.o set.o
	del *.o

main.o: main.cpp
	g++ -c main.cpp

set.o: set.cpp set.h
	g++ -c set.cpp

