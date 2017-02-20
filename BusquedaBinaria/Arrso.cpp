#include <stdlib.h>
#include <set>
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int nrec=0;

bool bus_bin(int elem, int *arr, int  inic, int fin){
	nrec++;
	int med = floor((inic+fin)/2);
	if(elem == arr[med] || elem == arr[inic] || elem == arr[fin]){
		return true;
	}else if( elem < arr[inic] || elem > arr[fin] || inic==med){
		return false;
	}else{
		if(elem > arr[med]){
			return bus_bin(elem,arr, med, fin);
		}else{
			return bus_bin(elem,arr,inic,med);
		}
	}
}

bool bus_bin_ite(int elem, int *arr, int siz, int *nite){
	int  inic = 0;
	*nite=0;
	int fin = siz-1;
	if(elem > arr[fin] || elem < arr[inic]){
		*nite+=1;
		return false;
	}else if(elem==arr[fin] || elem ==arr[inic]){
		*nite += 1;
		return true;
	}
	while(true){
		*nite+=1;
		int med = floor((fin+inic)/2);
		if(inic==med){
			return false;
		}else if(arr[med]==elem){
			return true;
		}else if(elem > arr[med]){
			inic=med;
		}else if(elem < arr[med]){
			fin=med;
		}
	}
}
bool bus_for(int elem, int *arr, int siz, int *nfor){
	*nfor=0;
	for(int i=0;i<siz;i++){
		*nfor+=1;
		if(arr[i]==elem){
			return true;
		}
	}
	return false;
}

int main(int argv,char* argc[]){
	int n=atoi(argc[1]);
	string temp_p=argc[2];
	int *arr=new int[n];
	int elem;
	set<int> seti;
	srand(time(NULL));
	arr[0]=rand() % 100;
	seti.insert(arr[0]);
	for(int i=1;i<n;i++){
		arr[i]=arr[i-1]+rand()%100+1;
		seti.insert(arr[i]);
	}	
	/*for(int i=0;i<n;i++){
		cout << arr[i] << "\t";
	}
	cout << endl;*/
	elem=rand()%(arr[n-1]+1);
	//cout << "Dame el elemento a buscar: "<<endl;
	//cin >> elem;
	cout << "elem: " << elem << endl;
	int nite;
	int nfor;
	
	ofstream outputf(temp_p.c_str(),ios::app);
	
	//cout << "Busqueda binaria recursiva: " << bus_bin(elem, arr, 0, n-1) <<endl;
	//cout << "Busqueda binaria iterativa: " <<  bus_bin_ite(elem,arr,n,&nite) << endl;
	//cout << "Busqueda uno por uno: " << bus_for(elem,arr,n,&nfor) << endl;
	//cout << "Pasos recursivos: " << nrec << endl;
	//cout << "Pasos iterativos: " << nite << endl;
	//cout << "Pasos uno por uno: " << nfor << endl;
	
	bool ind1=bus_bin(elem,arr,0,n-1);
	bool ind2=bus_bin_ite(elem,arr,n,&nite);
	bool ind3=bus_for(elem,arr, n, &nfor);
	
	outputf << ind1 << "," << nrec << "," << ind2 << "," << nite << "," << ind3 << "," << nfor << endl;
	//outputf << bus_bin(elem, arr, 0, n-1) <<"," << nrec <<"," << bus_bin_ite(elem,arr, n, &nite) <<"," << nite<<"," << bus_for(elem,arr, n, &nfor)<< "," << nfor << endl;
	//cout << "Busqueda binaria sobre clase set: "<< (seti.find(elem)==seti.end())?true:false <<endl;
	return 0;
}
