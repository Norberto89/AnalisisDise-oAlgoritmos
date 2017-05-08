#include<iostream>
#include<fstream>
#include<random>
#include<set>
#include <stdlib.h>
#include <math.h>

using namespace std;

int generar_random(int a, int b, set<int> used){
	
	
	

	int n=rand()%(b-a+1) +a;
	while(used.find(n)!=used.end()){
		n=rand()%(b-a+1) +a;
	}
	return n;
}

int main(int argv,char* argc[]){
	srand(time(NULL));
	
	string pth = argc[1];
	int Nvars = atoi(argc[2]);
	int Ncond_Avg = atoi(argc[3]);
	set<int> Used_indic;
	
	
	ofstream ofile(pth.c_str(), ios::out);
	
	ofile << "%% Instance for three sat" << endl;
	ofile << "cnf " << Nvars << endl;
	

	default_random_engine generator;
  	
	normal_distribution<double> distributionN(Ncond_Avg, ceil(0.2*(double)Ncond_Avg));

	uniform_int_distribution<int> distributionU(1,ceil(0.1*(double)Nvars));
	

	for(int i=0;i<3; i++){
		int nCond=round(distributionN(generator));
		ofile << i << " " << nCond << endl;
		for(int j=0;j<nCond; j++){
			int nVars_perCond = distributionU(generator);
			for(int k=0;k<nVars_perCond; k++){
				int n=generar_random(1,Nvars,Used_indic);
				Used_indic.insert(n);
				ofile << n << " ";	
			}
			Used_indic.clear();
			ofile << "0" << endl;
		}
	}
	  
	
		
	
	ofile.close();
	return 0;
}
