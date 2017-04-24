#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdlib.h>

using namespace std;

int main(int argv, char* argc[]){
	string file_pth = argc[1];
	cout << file_pth << endl;
	int a=atoi(argc[2]);
	int b=atoi(argc[3]);
	string temp;
	ifstream fil(file_pth.c_str(), ios::in);
	int nvert;
	int mina, maxa, mint;
	int xtemp, ytemp;
	int *arr;
	bool arc=false;
	if(!fil.good()){
		cout << "No se puede abrir el archivo: " << file_pth << endl;
		exit(0);
	}	
	
	while(!fil.eof()){		
		if(arc){
			fil >> xtemp;
			fil >> ytemp;
			mint = (arr[xtemp]<arr[ytemp])?arr[xtemp]:arr[ytemp];
			mina = (xtemp<ytemp)?xtemp:ytemp;
			maxa = (xtemp>ytemp)?xtemp:ytemp;
			mina =(mina < mint)?mina:mint;
			cout<< xtemp << " "<<ytemp <<endl;
			arr[xtemp]= mina;
			arr[ytemp]= mina;
			//For, para grafos no dirigidos.
			for(int i=0;i<nvert;i++){
				if(arr[i]==maxa){
					arr[i]=mina;
				}
			}
			
			if(arr[a]==arr[b]){
				cout << "Conectados" << endl;
				return 0;
			}	
		}else{
			fil>>temp;
			if(temp == "Vertices"){
			fil >> nvert;
			arr = new int [nvert];
			for(int i=0;i<nvert;i++){
				arr[i] = nvert+1;
			}
			}else if(temp == "Arcos"){
				arc = true; 
			}
		}
	
	}
	
	cout << "No conectados" << endl;
	return 0;
}
