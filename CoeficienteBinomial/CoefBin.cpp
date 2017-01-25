#include <iostream>
#include <stdlib.h>
#include <math.h>

using namespace std;

double BinCoef(int n, int k){
	int may=((n-k)>k)?(n-k):k;
	int men=((n-k)<k)?(n-k):k;
	int multi=1;
	int divi=1;
	for (int i=0;i<men;i++){
		multi*=n-i;
		divi*=men-i;
	}
	return (double)multi/(double)divi;
}

double Log_BinCoef(int n, int k, double base){
	
	int may=((n-k)>k)?(n-k):k;
	int men=((n-k)<k)?(n-k):k;
	double sumi=0.0,suma=0.0;
	for (int i=0;i<men;i++){
		sumi+=log(n-i)/log(base);
		suma-=log(men-i)/log(base);
	}
	return pow(base,sumi+suma);
}

int main(int argc, char *argv[]){
	int n=atoi(argv[1]);
        int k=atoi(argv[2]);
	double base=atof(argv[3]);
	
	double a=BinCoef(n,k);	
	double b=Log_BinCoef(n,k,base);

	cout<<a<<"\t"<<b<<endl;
	return 0;
}
