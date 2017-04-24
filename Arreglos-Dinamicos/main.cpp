#include <iostream>
#include <stdlib.h>

using namespace std;

int Aum_Num=0;
int Dis_Num=0;
int initial_size = 10;
int Max_Tam=initial_size;
float upper_limit = 0.8;
float lower_limit = 0.25;
int Cost;

void arreglo_dinamico(){
	int current_size = initial_size;
	int level = 0;
	srand(time(NULL));
	int count = 10000;
	for(int i=0; i < count; i++){
		int oper=rand()%2;
		if(oper==0){ // insert
			level++;
			if(level / (1.0 * current_size) > upper_limit){
				current_size *= 2;
				Cost += level;
				Aum_Num ++;
				if (current_size > Max_Tam){
					Max_Tam = current_size;	
				}
			}
		}else if(oper==1 && level > 0){
			level--;
			if(level / (1.0 * current_size) < lower_limit && current_size > initial_size) {
				current_size /= 2;
				Cost += level;
				Dis_Num ++;

			}
		}	
	}
	cout << Cost / (1.0 * count) << endl;
}


int main(int argv, char* argc[]){

	arreglo_dinamico();
	
	cout << Aum_Num << "\t" << Dis_Num << "\t" << Max_Tam << "\t" << Cost  << endl;
	
	return 0;
}
