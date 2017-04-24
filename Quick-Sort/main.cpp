#include <iostream>
#include <list>
#include <fstream>
#include <string>
#include <math.h>

using namespace std;

void quick_sort(int a, int b, int *arr){
	int piv = floor((a+b)/2.0),oa=a, ob=b, tmpPi,pivot;
	int sw;
	pivot=arr[piv];

	
	
	if(a>=b){
		return;
	}
	while(ob>oa){
		ob=b;
		oa=a;
		while(arr[ob]>=pivot && ob>a){
			ob--;
		}while(arr[oa]<pivot && oa<b){
			oa++;
		}
		if(ob>oa){
			sw=arr[ob];
			arr[ob]=arr[oa];
			arr[oa]=sw;
			if(oa==piv){
				piv=ob;		
			}else if(ob==piv){
				piv=oa;
			}
		}
	}
	
		
	quick_sort(a,piv,arr);
	quick_sort(piv+1,b,arr);
}

int main(int argv, char* argc[]){
	string pth = argc[1];
	ifstream iFile(pth.c_str(),ios::in);
	int siz, *arr;

	iFile >> siz;
	arr = new int[siz];


	for(int i=0; i<siz; i++){
		iFile >> arr[i];
		cout << arr[i] << endl;
	}
	
	quick_sort(0,siz-1,arr);
	
	for(int i=0;i<siz;i++){
		cout << arr[i] << " ";
	}
	cout << endl;
	return 0;
}
