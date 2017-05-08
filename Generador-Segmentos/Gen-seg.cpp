/* Autores: Pamela Jocelyn Palomo Martínez
		Norberto Alejandro Hernández Leandro*/
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>

using namespace std;

double generar_ale(){
	
	return (double)rand()/(double)RAND_MAX;
}

int main(int argv, char* argc[]){
	int n = atoi(argc[1]);
	string pth = argc[2];
	srand(time(NULL));
	
	ofstream ofile(pth.c_str(), ios::out);	
	
	//ofile << n << endl;		

	for(int i=0;i<n;i++){
			
		ofile << generar_ale() << " " << generar_ale() << " " << generar_ale() << " " << generar_ale() << endl;
	}


	ofile.close();

	return 0;
}
