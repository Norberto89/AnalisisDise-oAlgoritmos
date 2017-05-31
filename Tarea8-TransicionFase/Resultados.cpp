#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace std;

int main(int argv, char* argc[]){
	
	string tmp_pth = argc[1],tmp;
	int max_ite = atoi(argc[2]);
	double min_alfa = (double) atof(argc[3]),gap;

	ifstream ifile(tmp_pth.c_str(), ios::in);
	ofstream ofile("Resultados.csv", ios::app);
	
	ofile << max_ite << "," << min_alfa << ",";

	while(!ifile.eof()){
		ifile >> tmp;
		if(ifile.eof()){
			break;
		}
		if(tmp=="Gap:"){
			ifile >> gap;
			ofile << gap << endl;
		}
		
	}
	

	ifile.close();
	ofile.close();	

	return 0;
}
