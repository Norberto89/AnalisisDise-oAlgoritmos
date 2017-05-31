
CXXFLAGS = -std=c++11 -O3

all: sat

clean:
	rm -rf *.o Sat

sat: sat.o data_sat.o 
	g++ $(CXXFLAGS)  data_sat.o sat.o  -o sat

sat.o: sat.cpp
	g++ -c $(CXXFLAGS) sat.cpp	-o sat.o
data_sat.o: data_sat.cpp
	g++ -c $(CXXFLAGS) data_sat.cpp -o data_sat.o
