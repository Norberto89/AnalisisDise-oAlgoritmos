#include <iostream>
#include <random>
#include <fstream>


using namespace std;



int main(int argv, char* argc[]){
		int nAct, nEmp, maxDAct;
		int nPer=64;
		double m1,m2,s;
		
		int *dAct=new int [nPer];
		int **dActs;
		string f_pth = argc[1];
		nEmp = atoi(argc[2]);
		nAct = atoi(argc[3]);
		maxDAct = atoi(argc[4]);
		
		int nmax1=0, nmax2=0;
		
		dActs = new int* [nPer];
		for(int i=0;i<nPer;i++){
				dAct[i]=0;
				dActs[i]=new int[nAct];
				for(int j=0;j<nAct;j++){
					dActs[i][j]=0;
				}
		}
		int nexpr=100000;
		m1 = nPer/4.0;
		m2 = 3.0*nPer/4.0;
		
		s = nPer/8.0;
		
		default_random_engine generator;
		normal_distribution<double> distribution(m1,s);
		normal_distribution<double> distribution2(m2,s);

		for(int i = 0; i < nexpr; i++){
				double n1 = distribution(generator);
				double n2 = distribution2(generator);
				if(n1>=0 && n1<nPer/2.0){
						dAct[(int)n1]++;
						if(dAct[(int)n1]>nmax1){
							nmax1=dAct[(int)n1];
						}
				}
				if(n2>=nPer/2.0  && n2<nPer){
						dAct[(int)n2]++;
						if(dAct[(int)n2]>nmax2){
							nmax2=dAct[(int)n2];
						}
				}
		}
		//cout << nmax1 << " " << nmax2 << endl;
		
		
		for(int i=0;i<nPer; i++){
			if(i<nPer/2.0){ 
				dAct[i]=(int)((double)maxDAct*(double)dAct[i]/(double)nmax1);
				//cout << dAct[i] << endl;
			}else{
				dAct[i]=(int)((double)maxDAct*(double)dAct[i]/(double)nmax2);
				//cout << dAct[i] << endl;
			}
		}
		
		
		int ind, dA;
		uniform_int_distribution<int> distributionU(0,nAct-1);
		for(int i=0;i<nPer;i++){
			dA = 0;
			while(dA!=dAct[i]){
				ind = distributionU(generator);
				dActs[i][ind]++;
				dA++;
			}						
		}
		
		ofstream ofile(f_pth.c_str(),ios::out);
		ofile << "Pers" << endl;
		ofile << nPer << endl;
		cout << "# de empleados: " << nEmp << endl;
		ofile << "Empl" << endl;
		ofile << nEmp << endl;
		cout << "# de actividades: " << nAct << endl;
		ofile << "Acts" << endl;
		ofile << nAct << endl;
		
		cout << "Demandas: " << endl;
		ofile << "Dem" << endl;
		for(int i=0;i<nAct;i++){
			for(int j=0; j<nPer;j++){
				cout << dActs[j][i] << "\t" ;
				ofile << dActs[j][i] << " ";
			}
			cout << endl;
			ofile << endl;
		}
		
		ofile.close();
		
		
		return 0;
}
