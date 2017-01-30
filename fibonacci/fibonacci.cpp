#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace std;
int fibonacci_rec(int  n){
	if(n<=2){
		return 1;
	}else{
		return fibonacci_rec(n-1)+fibonacci_rec(n-2);
	}
}
int fibonacci_ite(int n){
	if(n<=2){
		return 1;
	}else{
		int fn1=1;
		int fn2=1;
		int fic;
		for(int i=3;i<n+1;i++){
			fic=fn1+fn2;
			fn1=fn2;
			fn2=fic;		
		}
		return fic;
	}
}

int main(int argv, char* argc[]){
	int n;
	n=atoi(argc[1]);
	int frec,fite;
	clock_t ini2=clock();
	fite=fibonacci_ite(n);
	clock_t fin2=clock();
	cout << "Iterativo: "<< fite<<endl;
	cout << "Tiempo: " << (double)(fin2-ini2)/(double)CLOCKS_PER_SEC << endl;
	clock_t ini1=clock();
	frec=fibonacci_rec(n);
	clock_t fin1=clock();
	cout<<"Recursivo: "<<frec<<endl;
	cout << "Tiempo: "<<(double) (fin1-ini1)/(double)CLOCKS_PER_SEC<<endl;
	return 0;
}
