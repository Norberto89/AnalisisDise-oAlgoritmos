#include <stdlib.h>
#include <stdio.h>
#include <iostream>

using namespace std;

void unir(int *arr,int *arrL, int *arrR, int a, int m, int b){
	int count1=0, count2=0, count=a;
	int siz1, siz2;
	

	for(int i=a;i<=b;i++){
		if(i<=m){
			arrL[count1]=arr[i];
			count1++;
		}else{
			arrR[count2]=arr[i];
			count2++;
		}
	}
	siz1=count1;
	siz2=count2;
	count1=0;
	count2=0;
	while(count1<siz1 && count2<siz2){
		
		if(arrL[count1]<= arrR[count2]){
			arr[count]=arrL[count1];
			count1++;
		}else{	
			arr[count]=arrR[count2];
			count2++;
		}
				
		
		
		count++;
	}
	
	while(count1 < siz1){
		arr[count]=arrL[count1];
		count++;
		count1++;
	}
	while(count2<siz2){
		arr[count] = arrR[count2];
		count++;
		count2++;
	}
}

void merge_sort(int *arr, int *arrL, int *arrR, int a, int b){
	//cout<< a <<" " << b << endl;
	if(a<b){
		int m=(a+b)/2;
		merge_sort(arr,arrL, arrR,a,m);
		merge_sort(arr,arrL, arrR,m+1,b);
		
		unir(arr,arrL, arrR,a,m,b);
	}
	
}



int main(int argv, char* argc[]){
	
	srand(time(NULL));
	int n = atoi(argc[1]);
	int *arr = new int[n];
	int *arrL = new int[n];
	int *arrR = new int[n];
	cout << "Arreglo: " << endl;
	for(int i=0;i<n;i++){
		arr[i] = rand()%1500;
		cout << arr[i] << "\t";
	}
	cout << endl;	
	
	
	merge_sort(arr,arrL, arrR,0,n-1);
	
	cout << "Arreglo Ordenado: " << endl;

	for(int i=0;i<n;i++){
		cout << arr[i] << "\t";
	}
	cout << endl;


	return 0;
}
