/* Autores: Pamela Jocelyn Palomo Martínez
		Norberto Alejandro Hernández Leandro*/
#include<iostream>
#include <fstream>
#include <stdlib.h>
#include <math.h>

using namespace std;

int main(int argv, char* argc[]){
	srand(time(NULL));
	string pth = argc[1];	
	int n = atoi(argc[2]);
	ofstream ofile(pth.c_str(),ios::out);
	ofile<<n<< endl;
	double *x,*y;
	x=new double[n];
	y=new double [n];
	double dist;
	for(int i=0;i<n;i++){
		x[i] = (double)rand()/(double)RAND_MAX;
		y[i] = (double)rand()/(double)RAND_MAX;
	}
	for(int i=0;i<n;i++){
		for(int j=i+1;j<n;j++){
			dist= sqrt(pow(x[i]-x[j],2)+pow(y[i]-y[j],2));	
			ofile << i << " " << j << " "<< dist << " "<< endl;
		}
	}
	
	
	
	return 0;
}
