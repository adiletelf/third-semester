output: main.o bvector.o bmatrix.o
	g++ -o output main.o bvector.o bmatrix.o
	del *.o

bvector.o: bvector.cpp bvector.h
	g++ -c bvector.cpp

bmatrix.o: bmatrix.cpp bmatrix.h
	g++ -c bmatrix.cpp


