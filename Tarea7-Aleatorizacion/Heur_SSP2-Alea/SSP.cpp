//
//  SSP.cpp
//  Heur_SSP
//
//  Created by Vincent on 9/26/14.
//
//

#include "SSP.h"


SSP::SSP(string temp){
    //Lee datos :P
	int a,i,e,s,p;
	int tempNbSkills;
	int skill;
	bool primary;
	//cout <<"In  readInputQUENTIN " << endl;
	
	string tempString;
	
	ifstream inputDemand(temp.c_str(), ios::in);
	if(!	inputDemand.good()){cout << "Incorrect shift file path: " <<temp<< endl;
		
	}
	
	inputDemand >> seqLength;
	inputDemand >> tempString;
	if(tempString == "0"){
		primary = false;
	}
	else{
		primary = true;
	}
	
	costTransit=15;
	
	inputDemand >> tempString;
	assert(tempString == "EMPL");
	inputDemand >> nbEmployees; assert(nbEmployees > 0);
	emplSkills = new set<int>[nbEmployees];
	maxLengthP = new int[nbEmployees];
	
	if(primary){
		pss = new int*[nbEmployees];
		psl = new int*[nbEmployees];
		nbPieces = new int[nbEmployees];
		nbPiecesMax = 0;
		maxLengthPiece = 0;
		for(e = 0; e < nbEmployees; e++){
			maxLengthP[e] = 0;
			inputDemand >> nbPieces[e];
			if(nbPieces[e] > nbPiecesMax){
				nbPiecesMax = nbPieces[e];
			}
			pss[e] = new int[nbPieces[e]];
			psl[e] = new int[nbPieces[e]];
			for(p = 0; p < nbPieces[e]; p++){
				inputDemand >> pss[e][p];
				inputDemand >> psl[e][p];
				if(psl[e][p] > maxLengthP[e]){
					maxLengthP[e] = psl[e][p];
				}
			}
			inputDemand >> tempNbSkills;
			for(s = 0; s < tempNbSkills; s++){
				inputDemand >> skill;
				emplSkills[e].insert(skill);
			}
			//emplSkills[e].insert(-1); //breaks
			
			if(maxLengthP[e] > maxLengthPiece){
				maxLengthPiece = maxLengthP[e];
			}
			
		}
	}
	
	//Activities
	inputDemand >> tempString;
	assert(tempString == "ACT");
	inputDemand >> nbAct; assert(nbAct > 0);
	minAct = new int[nbAct];
	maxAct = new int[nbAct];
	dAct = new int*[seqLength];
	ocAct = new int*[seqLength];
	ucAct = new int*[seqLength];
	cAct = new int*[seqLength];
	for(i = 0; i < seqLength; i++){
		dAct[i] = new int[nbAct];
		ocAct[i] = new int[nbAct];
		ucAct[i] = new int[nbAct];
		cAct[i] = new int[nbAct];
		for(a = 0; a < nbAct; a++){
			cAct[i][a] = 0;
		}
	}
	for(a = 0; a < nbAct; a++){
		inputDemand >> minAct[a];
		inputDemand >> maxAct[a];
		//cout << a <<": ";
		for(i = 0; i < seqLength; i++){
			inputDemand >> dAct[i][a];
			assert(dAct[i][a]>=0);
			//cout << dAct[i][a]<< " ";
		}
		//cout << endl;
		for(i = 0; i < seqLength; i++)	inputDemand >> ocAct[i][a];
		
		for(i = 0; i < seqLength; i++) inputDemand >> ucAct[i][a];
		
//		for(i = 0; i < seqLength; i++){
//			assert(ocAct[i][a]>=0 && ucAct[i][a]>=0);
//			ocAct[i][a]*=15;
//			ucAct[i][a]*=15;
//		}
	}
    //cout<<ocAct[0][0]<<endl;
    //getchar();
	
	//Tasks
	inputDemand >> tempString;
	if (tempString=="TASK") {
		assert(tempString == "TASK");
		inputDemand >> nbTask; assert(nbTask > 0);
		lTask = new int[nbTask];
		dTask = new int[nbTask];
		ocTask = new int[nbTask];
		ucTask = new int[nbTask];
		cTask = new int[nbTask];
		wsTask = new int[nbTask];
		wlTask = new int[nbTask];
		for(int t = 0; t < nbTask; t++){
			cTask[t] = 0;
		}
		
		Precedences=new set<int>[nbTask];
		for(int t = 0; t < nbTask; t++){ //cout <<t << ": ";
			inputDemand >> lTask[t]; //borne sur longueur activitÈ
			inputDemand >> ocTask[t];
			inputDemand >> ucTask[t];
//			ucTask[t]*=15;
//			ocTask[t]*=15;
            
			inputDemand >> wsTask[t];
			inputDemand >> wlTask[t];
			
			int nPrec;
			inputDemand >> nPrec;
			for (int i=0; i<nPrec; ++i) {
				int tt;
				inputDemand >> tt;
				Precedences[t].insert(tt);
			}
			
			dTask[t]=1; //pour le moment...
		}
	}
	else nbTask=0;
//        
//        cout <<"out readInputQUENTIN " << endl;
//        cout << endl;
//        
//        
//        cout <<"________________________________________________________________________"<<endl;
//        
//        cout << "Input File: "<<temp<<endl<<endl;
//        cout << "Sequence Length= "<<seqLength<<endl;
//        cout << "Number of employees: "<<nbEmployees<<endl;
//        cout << "Number of activities: "<<nbAct<<endl;
//        cout << "Number of tasks: "<<nbTask<<endl;
//        cout << endl;
//        
//        cout << "Employees:"<<endl;
//        for(int e=0;e<nbEmployees;++e){
//            cout << "\t"<<e<<"\ts="<<nbPieces[e]<<"\tskills={ ";
//            set<int>::iterator itSkills = emplSkills[e].begin();
//            while(itSkills != emplSkills[e].end()){
//                if(*itSkills >= nbAct) cout << "T"<<*itSkills-nbAct<<" ";
//                else cout<<*itSkills<<" ";
//                itSkills++;
//            }
//            cout <<"}"<<endl;
//        }
//        
//        cout << "Activities:"<<endl;
//        for(int a=0;a<nbAct;++a){
//            cout << "\t"<<a<<"\tl=["<<minAct[a]<<", "<<maxAct[a]<<"]"<<endl;
//        }
//        
//        cout << "Tasks:"<<endl;
//        for(int t=0;t<nbTask;++t){
//            cout << "\t T"<<t<<"\tl="<<lTask[t]<<"\td="<<dTask[t]<<"\tw=["<<wsTask[t]<<", "<<wsTask[t]+wlTask[t]<<"]"<<endl;
//            if (Precedences[t].size()>0) {
//                cout << "\t P: ";
//                for (set<int>::iterator it=Precedences[t].begin(); it!=Precedences[t].end(); ++it) {
//                    cout << "T"<<*it-nbAct<< " ";
//                }
//                cout << endl;
//            }
//        }
//        
//        cout <<"________________________________________________________________________"<<endl;
    
}

SSP::~SSP(){
    delete[] emplSkills;
	delete[] maxLengthP;
    
}
