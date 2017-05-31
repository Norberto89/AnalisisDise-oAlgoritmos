
CXXFLAGS = -std=c++11 -O3

all: Grafos

clean:
	rm -rf *.o Grafos

Grafos: main.o grafos.o leer_arc.o
	g++ $(CXXFLAGS)  grafos.o main.o leer_arc.o -o Grafos

main.o: main.cpp
	g++ -c $(CXXFLAGS) main.cpp	-o main.o
grafos.o: grafos.cpp
	g++ -c $(CXXFLAGS) grafos.cpp -o grafos.o
leer_arc.o: leer_arc.cpp
	g++ -c $(CXXFLAGS) leer_arc.cpp -o leer_arc.o
