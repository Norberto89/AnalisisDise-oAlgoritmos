#include <iostream>
#include <string>
#include <stdlib.h>
#include <fstream>
#include <sstream>

using namespace std;

int main(int argv, char* argc[]){
	int n=atoi(argc[1]);
	int ins=atoi(argc[2]);
	srand(time(NULL));

	for( int i=0; i<ins; i++){
		stringstream lin;
		lin << "Arreglos/Arreglo_";
		lin << n;
		lin << "_";
		lin << i;
		lin << ".txt";
		string pth= lin.str(); 	
	
		ofstream ofile(pth.c_str(),ios::out);
		ofile << n << endl;
		for(int j=0;j<n;j++){
			int a=rand()%1000;
			ofile << a << " ";
		}
		
	}
	
}
