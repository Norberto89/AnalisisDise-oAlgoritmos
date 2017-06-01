
#include <time.h>
#include <iostream>

#include "SSP.h"
#include "Heuristic.h"


using namespace std;


int main(int argc, char *argv[]){
    
	cout << argc << endl;
    
	if(argc!=3 ) {
		cout << "Error: " << argv[0] << " [Problem_File_Path]" << endl;
		exit(0);
	}
    
    string filePath=argv[1];
    int _ditr=atoi(argv[2]);
    //int max_ite = atoi(argv[2]);
    //double min_alfa = (double)atof(argv[3]);
    
    cout<<filePath<<endl;
    
    SSP *_SSP=new SSP(filePath);
    SubP *_SubProblem=new SubP(_SSP);
    SSP_Solution *S=new SSP_Solution(_SSP);
    
    
    Heuristic *L=new Lagrange(_SubProblem,_SSP, _ditr);
    
    double t0=clock();
    L->solve(S);
    t0=(double)(clock()-t0)/(double)CLOCKS_PER_SEC;
    cout<<"Total Time: "<<t0<<" s."<<endl;
    cout<<"Best Solution: "<<S->getcost()<<endl;

    
    delete L;
    delete S;
    delete _SubProblem;
    delete _SSP;
    
    cout<<"...done!"<<endl;
	
    return 0;
}
