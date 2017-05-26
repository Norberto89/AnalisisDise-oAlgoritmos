#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace std;

int main(int argv, char* argc[]){
	
	string tmp_pth = argc[1],tmp;
	int dist = atoi(argc[2]);
	int mues = atoi(argc[3]);
	string name = argc[4];
	double time;
	

	ifstream ifile(tmp_pth.c_str(), ios::in);
	ofstream ofile("Resultados.csv", ios::app);
	
	if(mues==1){
		ofile << name <<",";	
	}


	while(!ifile.eof()){
		ifile >> tmp;
		if(ifile.eof()){
			break;
		}
		if(tmp=="Time:"){
			ifile >> time;
			ofile<<time << ",";
			break;
		}
		
	}
	if(mues==10){
		ofile << endl;
	}
	

	ifile.close();
	ofile.close();	

	return 0;
}
