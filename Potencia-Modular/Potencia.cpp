#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <time.h>

using namespace std;

int main(int argc, char *argv[]){
	int base = atoi(argv[1]);
	int poten = atoi(argv[2]);
	int modul = atoi(argv[3]);
	int num = base;
	int result = 1;
	cout << "Usando funcion pow: " << endl;
	clock_t ti1 = clock();
	cout << (int)pow(base,poten)%modul << endl;
	clock_t tf1 = clock();
	cout << "Tiempo: " << (double)(tf1-ti1)/(double)CLOCKS_PER_SEC <<endl;
	clock_t ti2 = clock();
	while (poten>0){
		if(poten%2 == 1){
			result *= num;
			result = result%modul;
		}
		poten = poten>>1;
		num *= num;
		num = num%modul;
	}
	clock_t tf2 = clock();

	cout << "Exponenciacion rapida: " << endl;
	cout << result << endl;
	cout<<ti2<<"  "<<tf2<<endl;	
	cout << "Tiempo: " << (double)(tf2-ti2)/(double)CLOCKS_PER_SEC <<endl;

	return 0;
}
