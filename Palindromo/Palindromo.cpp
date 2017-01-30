#include <iostream>
#include <string>
#include <time.h>

using namespace std;

bool Palindromo_ite(string Pal){
	int inic=0, fin=Pal.size()-1;
	while (inic<fin){
		if(Pal[inic]!=Pal[fin]){
			return false;
		}else{
			inic++;
			fin--;
		}	
	}
	return true;
}

bool Palindromo_Rec(string Pal){
	int siz=Pal.size();
	if(siz<=1){
		return true;
	}else{
		if(Pal[0]!=Pal[siz-1]){
			return false;
		}else{
			return Palindromo_Rec(Pal.substr(1,siz-2));
		}
	}
}

int main(int argv,char* argc []){
	string Pal=argc[1];
	clock_t ini1=clock();
	cout << "Iterativo: "<< Palindromo_ite(Pal)<<endl;
	clock_t fin1=clock();
	cout << "Tiempo: " << (double)(fin1-ini1)/(double)CLOCKS_PER_SEC << endl;
	clock_t ini2=clock();
	cout << "Iterativo: "<< Palindromo_Rec(Pal)<<endl;
	clock_t fin2=clock();
	cout << "Tiempo: " << (double)(fin2-ini2)/(double)CLOCKS_PER_SEC << endl;
	return 0;
}
