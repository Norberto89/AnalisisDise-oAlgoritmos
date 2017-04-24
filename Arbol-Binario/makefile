
CXXFLAGS = -std=c++11 -O3

all: Bin_Ar

clean:
	rm -rf *.o Grafos

Bin_Ar: main.o Arbol_Binario.o 
	g++ $(CXXFLAGS)  Arbol_Binario.o main.o -o Bin_Ar

main.o: main.cpp
	g++ -c $(CXXFLAGS) main.cpp	-o main.o
Arbol_Binario.o: Arbol_Binario.cpp
	g++ -c $(CXXFLAGS) Arbol_Binario.cpp -o Arbol_Binario.o
