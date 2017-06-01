//
// SubP.cpp
//
// Created by Vincent Boyer
//
// CIRRELT, Centre interuniversitaire de recherche sur les réseaux 
// d'entreprise, la logistique et le transport 
// Pavillon André-Aisenstadt, bureau 3520
// 2920, chemin de la Tour
// Montréal (Québec)  H3T 1J4  CANADA 
// Phone: (514) 343-7575 
// E-mail: v-boyer@laposte.net
//
//
//
// Janvier 2011
//

#include "SubP.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>    


using namespace std;


//Constructor & destructor
SubP::SubP(SSP *SSP){
	
	int e;
	
	
	initSCP(SSP);
	
	_symbols= new map<string,int>[_SSP->nbEmployees];
	_andNodes = new AndNode***[_SSP->nbEmployees];
	_gr = new GrammarGraph**[_SSP->nbEmployees];
	
	_terminalToAct= new map<int,int>*[_SSP->nbEmployees];
	
	for(e = 0; e < _SSP->nbEmployees; e++){
		//cout <<"SubProblem: " << e <<" ("<<_SSP->nbPieces[e]<<")"<<endl;
		createGrammar(e);
		initializeGrammarGraph(e);
		createTerminalToAct(e);
		
		//_andNodes[0][0][0]->id;

//		for (int p=0; p<_SSP->nbPieces[e]; ++p) {
//			cout << _gr[e][p]->nbNodes<<" ";
//		}
//		cout << endl;
		
	}
	
	//	exit(0);
	
}
SubP::~SubP(){cout<<"SubProblem Destroyed"<<endl;}

//Copie the data in the local variable _SSP
void SubP::initSCP(SSP *SCP){
	_SSP=SCP;
}

//Create the set of productions for emplyee e for each pieces p
void SubP::createGrammarQUENTIN(int e){
	int nbTerminals, nbNonTerminals;
	
	int nbActE = 0;
	
	string symbol;
	string symbolA;
	string symbolB;
	string symbolC;
	
	ostringstream as;
	ostringstream as2;
	ostringstream as3;
	ostringstream asA;
	ostringstream asB;
	ostringstream asC;
	
	//List of terminals
	//List of non-terminals
	//Productions
	
	
	/*nom des fichiers de sortie*/
	_grammarFile=new string[_SSP->nbPieces[e]];
	for (int p=0; p<_SSP->nbPieces[e]; ++p) {
		_grammarFile[p] = "Grammar/gr_";
		as.str(""); as << e;
		_grammarFile[p] += as.str();
		_grammarFile[p] += "_";
		as.str(""); as << p;
		_grammarFile[p] += as.str();
	}
	
	
	
	
	
	/*_symbols: contient le mapping des symbol des noeuds avec son numéro d'identification*/
	
	
	int nbTask=0;
	int *TaskSkillsId=new int[_SSP->nbTask];
	set<int>::iterator itSkills = _SSP->emplSkills[e].begin();
	while(itSkills != _SSP->emplSkills[e].end()){
		int t=*itSkills-_SSP->nbAct;
		if(*itSkills >= _SSP->nbAct){
			bool validtask=true;
			if (validtask) {
				TaskSkillsId[nbTask++]=t;
			}
		}
		itSkills++;
	}
	
	//Tasks could appear more than once in a shift...
	
	
	//creation de la grammaire
	ofstream *gram=new ofstream[_SSP->nbPieces[e]];
	for (int p=0; p<_SSP->nbPieces[e]; ++p){
		gram[p].open(_grammarFile[p].c_str(), ios::trunc);
		gram[p] << "# Terminals" << endl;
	}
	
	nbTerminals = 0;
	itSkills = _SSP->emplSkills[e].begin();
	while(itSkills != _SSP->emplSkills[e].end()){
		if(*itSkills < _SSP->nbAct){
			symbol.clear();symbol += "a_";as.str("");as << *itSkills;symbol += as.str(); 
			mapSymbol(symbol, e);
			for (int p=0; p<_SSP->nbPieces[e]; ++p)
				gram[p] << "# a_" << *itSkills << " "<< _symbols[e][symbol] << endl; 
			nbTerminals++;
			nbActE++;
		}
		itSkills++;
	}	
	//Taches
	for (int t=0;t<nbTask;++t){
		
		symbol.clear();symbol += "t_";as.str("");as << TaskSkillsId[t];symbol += as.str();
		mapSymbol(symbol, e);
		for (int p=0; p<_SSP->nbPieces[e]; ++p)
			gram[p] << "# t_" << TaskSkillsId[t] << " "<< _symbols[e][symbol] << endl;
		nbTerminals++;
		
		
	}
	
	
	
	nbNonTerminals = nbTerminals;
	mapSymbol("S", e);
	for (int p=0; p<_SSP->nbPieces[e]; ++p){
		gram[p] << "# Non-terminals" << endl;
		gram[p] << "# S " << _symbols[e]["S"] << endl; 
	}
	nbNonTerminals++;
	if (nbTask>0) {
		mapSymbol("BA", e);
		mapSymbol("T", e);
		mapSymbol("BAT", e);
		
		for (int p=0; p<_SSP->nbPieces[e]; ++p){
			gram[p] << "# BA " << _symbols[e]["BA"] << endl;
			gram[p] << "# T " << _symbols[e]["T"] << endl; 
			gram[p] << "# BAT " << _symbols[e]["BAT"] << endl; 
		}
		
		nbNonTerminals+=3;
		
		
	}
	
	
	itSkills = _SSP->emplSkills[e].begin();
	while(itSkills != _SSP->emplSkills[e].end()){
		if(*itSkills<_SSP->nbAct)
		{
			symbol.clear();symbol += "A_n_";as.str(""); as << *itSkills; symbol += as.str();
			mapSymbol(symbol, e);
			for (int p=0; p<_SSP->nbPieces[e]; ++p)
				gram[p] << "# A_n_" << *itSkills << " " << _symbols[e][symbol] << endl; 
			nbNonTerminals++;
			
			symbol.clear();symbol += "A_";as2.str(""); as2 << *itSkills; symbol += as2.str();
			mapSymbol(symbol, e);
			for (int p=0; p<_SSP->nbPieces[e]; ++p)	
				gram[p] << "# A_" << *itSkills << " "<< _symbols[e][symbol] << endl; 
			nbNonTerminals++;
			
			symbol.clear();symbol += "A_";as3.str(""); as3 << *itSkills; symbol += as3.str();symbol += "_2";
			mapSymbol(symbol, e);
			for (int p=0; p<_SSP->nbPieces[e]; ++p)
				gram [p]<< "# A_" << *itSkills << "_" << 2 << " "<< _symbols[e][symbol] << endl; 
			nbNonTerminals++;
			
		}
		itSkills++;
	}
	
	//Taches
	for (int t=0;t<nbTask;++t) {
		
		symbol.clear();symbol += "T_";as.str("");as << TaskSkillsId[t];symbol += as.str();
		mapSymbol(symbol, e);
		for (int p=0; p<_SSP->nbPieces[e]; ++p)
			gram[p] << "# "<<symbol<< " "<< _symbols[e][symbol] << endl;
		nbNonTerminals++;
		
	}
	for (int t=1;t<nbTask;++t) {
		//indice = nombre max de taches dans le sous-bloc de taches
		symbol.clear();symbol += "BT_";as.str("");as << t;symbol += as.str();
		mapSymbol(symbol, e);
		for (int p=0; p<_SSP->nbPieces[e]; ++p)
			gram[p] << "# "<<symbol<< " "<< _symbols[e][symbol] << endl;
		nbNonTerminals++;
	}
	
	//BATBA
	symbol.clear();symbol += "BATBA";
	mapSymbol(symbol, e);
	for (int p=0; p<_SSP->nbPieces[e]; ++p)
		gram[p] << "# "<<symbol<< " "<< _symbols[e][symbol] << endl;
	nbNonTerminals++;
	
	
	for (int p=0; p<_SSP->nbPieces[e]; ++p)
		gram[p] << endl;
	
	nbNonTerminals = nbNonTerminals - nbTerminals;
	
	//NbProductions
	int nbProductions =  nbActE * (5 + (nbActE - 1)*2); /*calcul a priori du nombre de production*/
	//cout << "NbActE= "<<nbActE<<endl;
	//cout << "Nb prod act= "<<nbProductions<<endl;
	if(nbTask>0){
		nbProductions+=(nbActE*3+4+3*nbTask); //Ajout des productions liées aux tâches
		//cout << "*Nb prod inter= "<<nbProductions<<endl;
		//cout << "nbTasks= "<<nbTask<<endl;
		if (nbTask>1){
			nbProductions+=3+nbTask+(nbTask-2)*(3+nbTask)+1;
		}
	}
	//cout<<"Nb prod act+tasks= "<<nbProductions<<endl;
	
	
	
	//NbTerminals NbNonTerminals NbProductions
	for (int p=0; p<_SSP->nbPieces[e]; ++p)
		gram[p] << nbTerminals << " " << nbNonTerminals << " " << nbProductions << endl;
	
	itSkills = _SSP->emplSkills[e].begin();
	while(itSkills != _SSP->emplSkills[e].end()){
		if(*itSkills<_SSP->nbAct){
			symbolB.clear(); symbolB += "A_"; asB.str(""); asB << *itSkills; symbolB += asB.str();
			symbolC.clear(); symbolC += "A_n_"; asC.str(""); asC << *itSkills; symbolC += asC.str();
			for (int p=0; p<_SSP->nbPieces[e]; ++p){
				gram[p] << "# S = A_"<<*itSkills << "A_n_" << *itSkills <<"[_segMinLength, _segMaxLength]" << endl;
				gram[p] << "15.0 " << _SSP->psl[e][p] << " " << _SSP->psl[e][p] << " 2 "<< _symbols[e]["S"] << " " << _symbols[e][symbolB]  << " " << _symbols[e][symbolC] << " " << 0 << " " << _SSP->psl[e][p] << endl;
			}
			
			if(_SSP->maxAct[*itSkills] >= _SSP->maxLengthP[e]){
				symbolB.clear(); symbolB += "A_"; asB.str(""); asB << *itSkills; symbolB += asB.str();
				symbolC.clear(); symbolC += "A_n_"; asC.str(""); asC << *itSkills; symbolC += asC.str();
				symbolB.clear(); symbolB += "A_"; asB.str(""); asB << *itSkills; symbolB += asB.str(); symbolB += "_2";
				for (int p=0; p<_SSP->nbPieces[e]; ++p){
					gram[p] << "# S = A_" << *itSkills << "_2A_" << *itSkills << "_2 [_segMinLength, _segMaxLength]" << endl;
					gram[p] << "0.0 " << _SSP->psl[e][p] << " " << _SSP->psl[e][p] << " 2 "<< _symbols[e]["S"] << " " << _symbols[e][symbolB]  << " " << _symbols[e][symbolB] << " " << 0 << " " << _SSP->psl[e][p] << endl;	
				}
			}
			
			//Def d'un bloc d'activitees
			if (nbTask>0) {
				symbolA.clear(); symbolA += "a_"; asA.str(""); asA << *itSkills; symbolA += asA.str();
				symbolB.clear(); symbolB += "A_"; asB.str(""); asB << *itSkills; symbolB += asB.str();
				symbolC.clear(); symbolC += "A_n_"; asC.str(""); asC << *itSkills; symbolC += asC.str();
				
				for (int p=0; p<_SSP->nbPieces[e]; ++p){
					gram[p] << "# BA = "<< symbolB << symbolC <<" | "<< symbolB<<"_2"<<symbolB<<"_2"<<" | "<< symbolA << endl;
					gram[p] << "15.0 0 9999 2 "<< _symbols[e]["BA"] << " " << _symbols[e][symbolB]  << " " << _symbols[e][symbolC] << " " << 0 << " " << _SSP->psl[e][p] << endl;
				}
				symbolB += "_2";
				for (int p=0; p<_SSP->nbPieces[e]; ++p){
					gram[p] << "0.0 0 9999 2 "<< _symbols[e]["BA"] << " " << _symbols[e][symbolB]  << " " << _symbols[e][symbolB] << " " << 0 << " " << _SSP->psl[e][p] << endl;
					gram[p] << "0.0 0 9999 1 "<< _symbols[e]["BA"] << " " << _symbols[e][symbolA]  << " " << 0 << " " << _SSP->psl[e][p] << endl;
				}
			}
			
			set<int>::iterator itSkillsp = _SSP->emplSkills[e].begin();
			while(itSkillsp != _SSP->emplSkills[e].end()){
				if(*itSkills != *itSkillsp && *itSkillsp<_SSP->nbAct){
					symbolA.clear(); symbolA += "A_n_"; asA.str(""); asA << *itSkillsp; symbolA += asA.str();
					symbolB.clear(); symbolB += "A_"; asB.str(""); asB << *itSkills; symbolB += asB.str();
					symbolC.clear(); symbolC += "A_n_"; asC.str(""); asC << *itSkills; symbolC += asC.str();
					for (int p=0; p<_SSP->nbPieces[e]; ++p){
						gram[p] << "# A_n_" << *itSkillsp << " = A_"<< *itSkills << "A_n_" << *itSkills << " | A_" << *itSkills << "_2A_" << *itSkills << "_2" << endl;
						gram[p] << "15.0 0 9999 2 "<< _symbols[e][symbolA] << " " << _symbols[e][symbolB]  << " " << _symbols[e][symbolC] << " " << 0 << " " << _SSP->psl[e][p] << endl;
					}
					symbolB.clear(); symbolB += "A_"; asB.str(""); asB << *itSkills; symbolB += asB.str(); symbolB += "_2";
					for (int p=0; p<_SSP->nbPieces[e]; ++p)
						gram[p] << "0.0 " << _SSP->minAct[*itSkills] << " " << _SSP->maxAct[*itSkills] << " 2 "<< _symbols[e][symbolA] << " " << _symbols[e][symbolB]  << " " << _symbols[e][symbolB] << " " << 0 << " " << _SSP->psl[e][p] << endl;
				}
				itSkillsp++;
			}
			
			symbolA.clear(); symbolA += "A_"; asA.str(""); asA << *itSkills; symbolA += asA.str();
			for (int p=0; p<_SSP->nbPieces[e]; ++p){
				gram[p] << "# A_" << *itSkills << " = A_" << *itSkills << "_2A_" << *itSkills << "_2 [_minAct,_maxAct]"<<endl;  
				gram[p] << "0.0 " << _SSP->minAct[*itSkills] << " " << _SSP->maxAct[*itSkills] << " 2 "<< _symbols[e][symbolA] << " " << _symbols[e][symbolB]  << " " << _symbols[e][symbolB] << " " << 0 << " " << _SSP->psl[e][p] << endl;
			}
			
			symbolA.clear(); symbolA += "A_"; asA.str(""); asA << *itSkills; symbolA += asA.str(); symbolA += "_2";
			symbolB.clear(); symbolB += "a_"; asB.str(""); asB << *itSkills; symbolB += asB.str();
			for (int p=0; p<_SSP->nbPieces[e]; ++p){
				gram[p] << "# A_" << *itSkills << "_2 = A_" << *itSkills << "_2A_" << *itSkills << "_2 | a_" << *itSkills << endl;
				gram[p] << "0.0 0 9999 2 "<< _symbols[e][symbolA] << " " << _symbols[e][symbolA]  << " " << _symbols[e][symbolA] << " " << 0 << " " << _SSP->psl[e][p] << endl;
				gram[p] << "0.0 0 9999 1 "<< _symbols[e][symbolA] << " " << _symbols[e][symbolB] << " 0 " << _SSP->psl[e][p] << endl;
				gram[p] << endl;
			}
		}
		
		itSkills++;
		
	}
	
	//Insertion des Taches parmie les activitées
	
	if (nbTask>0) {
		//S = BT_1 = BAT BA | BA T | T BA (| TT )
		symbolA.clear(); symbolA += "BAT";
		symbolB.clear(); symbolB += "BA"; 
		symbolC.clear(); symbolC += "T";
		
		
		for (int p=0; p<_SSP->nbPieces[e]; ++p){
			gram[p] << "# "<<symbolA <<" = "<< symbolB << symbolC <<  endl;
			gram[p] << "15.0 0 9999 2 "<< _symbols[e][symbolA] << " " << _symbols[e][symbolB]  << " " << _symbols[e][symbolC] << " " << 0 << " " << _SSP->psl[e][p] << endl;
			
			gram[p] << "# S = " << symbolA << symbolB << " | " << symbolB<<symbolC <<" | "<<symbolC<<symbolB<< endl;
			gram[p] << "15.0 " << _SSP->psl[e][p] << " " << _SSP->psl[e][p] << " 2 "<< _symbols[e]["S"] << " " << _symbols[e][symbolA]  << " " << _symbols[e][symbolB] << " " << 0 << " " << _SSP->psl[e][p] << endl;
			gram[p] << "15.0 " << _SSP->psl[e][p] << " " << _SSP->psl[e][p] << " 2 "<< _symbols[e]["S"] << " " << _symbols[e][symbolB]  << " " << _symbols[e][symbolC] << " " << 0 << " " << _SSP->psl[e][p] << endl;
			gram[p] << "15.0 " << _SSP->psl[e][p] << " " << _SSP->psl[e][p] << " 2 "<< _symbols[e]["S"] << " " << _symbols[e][symbolC]  << " " << _symbols[e][symbolB] << " " << 0 << " " << _SSP->psl[e][p] << endl;
		}
	}
	
	for (int p=0; p<_SSP->nbPieces[e]; ++p)
		gram[p] << endl;
	
	if (nbTask>1) {
		//S = BT_1 BT_(nbTask-1)
		
		symbolA.clear(); symbolA += "BT_1";
		symbolB.clear(); symbolB += "BT_";  asB.str(""); asB << nbTask-1; symbolB += asB.str();
		
		for (int p=0; p<_SSP->nbPieces[e]; ++p){
			gram[p] << "# S = " << symbolA << symbolB << endl;
			gram[p] << "15.0 " << _SSP->psl[e][p] << " " << _SSP->psl[e][p] << " 2 "<< _symbols[e]["S"] << " " << _symbols[e][symbolA]  << " " << _symbols[e][symbolB] << " " << 0 << " " << _SSP->psl[e][p] << endl;
		}
		
		//BT_1 = BAT BA | BA T | TT
		symbol.clear(); symbol += "BT_1";
		symbolA.clear(); symbolA += "BAT";
		symbolB.clear(); symbolB += "BA"; 
		symbolC.clear(); symbolC += "T"; 
		
		for (int p=0; p<_SSP->nbPieces[e]; ++p){
			gram[p] << "# "<<symbol<<" = "  << symbolB<<symbolC<< endl;
			gram[p] << "15.0 0 9999 2 "<< _symbols[e][symbol] << " "<< _symbols[e][symbolB]  << " " << _symbols[e][symbolC] << " " << 0 << " " << _SSP->psl[e][p] << endl;
		}
		
		
		for(int t=0; t<nbTask;++t){
			symbolC.clear(); symbolC += "T_"; asC.str(""); asC << TaskSkillsId[t]; symbolC += asC.str(); 
			for (int p=0; p<_SSP->nbPieces[e]; ++p){
				int startT=_SSP->wsTask[TaskSkillsId[t]]-_SSP->pss[e][p]; 
				if(startT<0) startT=0;
				int endT=_SSP->wsTask[TaskSkillsId[t]]+_SSP->wlTask[TaskSkillsId[t]]-_SSP->pss[e][p];
				if(endT<0) endT=0;			
				gram[p] << "# "<<symbol<<" = " << symbolC << symbolC << endl;
				gram[p] << "0.0 " << _SSP->lTask[TaskSkillsId[t]] << " " << _SSP->lTask[TaskSkillsId[t]] << " 2 "<< _symbols[e][symbol] << " " << _symbols[e][symbolC]<< " " << _symbols[e][symbolC]  << " " << startT <<" "<< endT<<endl;
			}
		}
		
		for (int p=0; p<_SSP->nbPieces[e]; ++p)
			gram[p] << endl;
		
		symbol.clear(); symbol += "BATBA";
		for (int p=0; p<_SSP->nbPieces[e]; ++p){
			gram[p] << "# "<<symbol<<" = " << symbolA << symbolB << endl;
			gram[p] << "15.0 0 9999 2 "<< _symbols[e][symbol] << " " << _symbols[e][symbolA]  << " " << _symbols[e][symbolB] << " " << 0 << " " << _SSP->psl[e][p] << endl;
		}
		
		for (int p=0; p<_SSP->nbPieces[e]; ++p)
			gram[p] << endl;
	}
	for (int p=0; p<_SSP->nbPieces[e]; ++p)
		gram[p] << endl;
	
	if (nbTask==2) {
		symbolA.clear(); symbolA += "BT_1";
		symbolB.clear(); symbolB += "BATBA";
		
		for (int p=0; p<_SSP->nbPieces[e]; ++p){
			gram[p] << "# S = " << symbolA << symbolB << endl;
			gram[p] << "15.0 " << _SSP->psl[e][p] << " " << _SSP->psl[e][p] << " 2 "<< _symbols[e]["S"] << " " << _symbols[e][symbolA]  << " " << _symbols[e][symbolB] << " " << 0 << " " << _SSP->psl[e][p] << endl;
		}
	}
	else{
		symbolA.clear(); symbolA += "BT_1";
		symbolB.clear(); symbolB += "BATBA";
		symbolC.clear(); symbolC += "BT_2";
		
		for (int p=0; p<_SSP->nbPieces[e]; ++p){
			gram[p] << "#"<<symbolC<<" = " << symbolA << symbolB << endl;
			gram[p] << "15.0 " << _SSP->psl[e][p] << " " << _SSP->psl[e][p] << " 2 "<< _symbols[e][symbolC] << " " << _symbols[e][symbolA]  << " " << _symbols[e][symbolB] << " " << 0 << " " << _SSP->psl[e][p] << endl;
		}
	}
	
	for (int p=0; p<_SSP->nbPieces[e]; ++p)
		gram[p] << endl;
	
	
	for (int t=1; t<nbTask-1; ++t) {
		//BT_(t+1)=BT_1 BT_t | BT_1
		symbol.clear(); symbol += "BT_"; as.str(""); as << t+1; symbol += as.str();
		symbolB.clear(); symbolB += "BT_1"; asB.str("");
		symbolC.clear(); symbolC += "BT_"; asC.str(""); asC << t; symbolC += asC.str();
		for (int p=0; p<_SSP->nbPieces[e]; ++p){
			gram[p] << "# "<<symbol<<" = " << symbolB << symbolC << endl;
			gram[p] << "15.0 0 9999 2 "<< _symbols[e][symbol] << " " << _symbols[e][symbolB]<< " " << _symbols[e][symbolC]  << " " << 0 << " " << _SSP->psl[e][p] << endl;	
		}
		
		symbolA.clear(); symbolA += "BAT";
		symbolB.clear(); symbolB += "BA"; 
		symbolC.clear(); symbolC += "T"; 
		
		for (int p=0; p<_SSP->nbPieces[e]; ++p){
			gram[p] << "# "<<symbol<<" = " << symbolA << symbolB << " | " << symbolB<<symbolC<< endl;
			gram[p] << "15.0 0 9999 2 "<< _symbols[e][symbol] << " " << _symbols[e][symbolA]  << " " << _symbols[e][symbolB] << " " << 0 << " " << _SSP->psl[e][p] << endl;
			gram[p] << "15.0 0 9999 2 "<< _symbols[e][symbol] << " " << _symbols[e][symbolB]  << " " << _symbols[e][symbolC] << " " << 0 << " " << _SSP->psl[e][p] << endl;
		}
		
		for(int t=0; t<nbTask;++t){
			symbolC.clear(); symbolC += "T_"; asC.str(""); asC << TaskSkillsId[t]; symbolC += asC.str(); 
			for (int p=0; p<_SSP->nbPieces[e]; ++p){
				int startT=_SSP->wsTask[TaskSkillsId[t]]-_SSP->pss[e][p]; 
				if(startT<0) startT=0;
				int endT=_SSP->wsTask[TaskSkillsId[t]]+_SSP->wlTask[TaskSkillsId[t]]-_SSP->pss[e][p];
				if(endT<0) endT=0;				
				gram[p] << "# "<<symbol<<" = " << symbolC << symbolC << endl;
				gram[p] << "0.0 " << _SSP->lTask[TaskSkillsId[t]] << " " << _SSP->lTask[TaskSkillsId[t]] << " 2 "<< _symbols[e][symbol] << " " << _symbols[e][symbolC]<< " " << _symbols[e][symbolC]  << " " << startT <<" "<< endT<<endl;
			}
			
		}
		
		for (int p=0; p<_SSP->nbPieces[e]; ++p)
			gram[p] << endl;
	}
	
	
	//Productions liées aux Taches T_k
	for(int t=0; t<nbTask;++t){
		
		symbolA.clear(); symbolA += "T";
		symbolB.clear(); symbolB += "t_"; asB.str(""); asB << TaskSkillsId[t]; symbolB += asB.str();
		symbolC.clear(); symbolC += "T_"; asC.str(""); asC << TaskSkillsId[t]; symbolC += asC.str(); 
		
		for (int p=0; p<_SSP->nbPieces[e]; ++p){
			gram[p] << "# "<< symbolA << "= "<<symbolC<<symbolC<< endl;
			
			int startT=_SSP->wsTask[TaskSkillsId[t]]-_SSP->pss[e][p]; 
			if(startT<0) startT=0;
			int endT=_SSP->wsTask[TaskSkillsId[t]]+_SSP->wlTask[TaskSkillsId[t]]-_SSP->pss[e][p];
			if(endT<0) endT=0;
			
			gram[p] << "0.0 " << _SSP->lTask[TaskSkillsId[t]] << " " << _SSP->lTask[TaskSkillsId[t]] << " 2 "<< _symbols[e][symbolA] << " " << _symbols[e][symbolC]<< " " << _symbols[e][symbolC]  << " " << startT <<" "<< endT<<endl;
			
			gram[p] << "# "<< symbolC << "= "<<symbolB<<" | "<<symbolC<<symbolC<< endl;
			
			gram[p] << "0.0 0 9999 1 "<< _symbols[e][symbolC] << " " << _symbols[e][symbolB]  << " " << 0 << " " << _SSP->psl[e][p] << endl;
			gram[p] << "0.0 0 9999 2 "<< _symbols[e][symbolC] << " " << _symbols[e][symbolC]  <<" " << _symbols[e][symbolC]  << " " << 0 << " " << _SSP->psl[e][p] << endl;
			
			gram[p]<<endl;
		}
		
	}
	
	for (int p=0; p<_SSP->nbPieces[e]; ++p)
		gram[p].close();
	
	
}


//With starting nodes st for tasks...
void SubP::createGrammar(int e){
	int nbTerminals, nbNonTerminals;
	
	int nbActE = 0;
	
	string symbol;
	string symbolA;
	string symbolB;
	string symbolC;
	
	ostringstream as;
	ostringstream as2;
	ostringstream as3;
	ostringstream asA;
	ostringstream asB;
	ostringstream asC;
	
	//List of terminals
	//List of non-terminals
	//Productions
	
	//cout<<"hola mundo"<<endl;
	/*nom des fichiers de sortie*/
	_grammarFile=new string[_SSP->nbPieces[e]];
	for (int p=0; p<_SSP->nbPieces[e]; ++p) {
		_grammarFile[p] = "Grammar/gr_";
		as.str(""); as << e;
		_grammarFile[p] += as.str();
		_grammarFile[p] += "_";
		as.str(""); as << p;
		_grammarFile[p] += as.str();
	}
	
	
	
	
	
	/*_symbols: contient le mapping des symbol des noeuds avec son numéro d'identification*/
	
	//Tasks could appear more than once in a shift...
	
	
	//creation de la grammaire
	ofstream *gram=new ofstream[_SSP->nbPieces[e]];
	for (int p=0; p<_SSP->nbPieces[e]; ++p){
		gram[p].open(_grammarFile[p].c_str(), ios::trunc);
		gram[p] << "# Terminals" << endl;
	}
	
	nbTerminals = 0;
	set<int>::iterator itSkills = _SSP->emplSkills[e].begin();
	while(itSkills != _SSP->emplSkills[e].end()){
		if(*itSkills < _SSP->nbAct){
			symbol.clear();symbol += "a_";as.str("");as << *itSkills;symbol += as.str(); 
			mapSymbol(symbol, e);
			for (int p=0; p<_SSP->nbPieces[e]; ++p)
				gram[p] << "# a_" << *itSkills << " "<< _symbols[e][symbol] << endl; 
			nbTerminals++;
			nbActE++;
		}
		else{
			symbol.clear();symbol += "t_";as.str("");as << *itSkills-_SSP->nbAct;symbol += as.str();
			mapSymbol(symbol, e);
			for (int p=0; p<_SSP->nbPieces[e]; ++p)
				gram[p] << "# t_" << *itSkills-_SSP->nbAct << " "<< _symbols[e][symbol] << endl;
			nbTerminals++;
			nbActE++;
			
			symbol.clear();symbol += "st_";as.str("");as << *itSkills-_SSP->nbAct;symbol += as.str();
			mapSymbol(symbol, e);
			for (int p=0; p<_SSP->nbPieces[e]; ++p)
				gram[p] << "# st_" << *itSkills-_SSP->nbAct << " "<< _symbols[e][symbol] << endl;
			nbTerminals++;
			//nbActE++;
		}
		itSkills++;
	}	
	
	
	
	
	nbNonTerminals = nbTerminals;
	mapSymbol("S", e);
	for (int p=0; p<_SSP->nbPieces[e]; ++p){
		gram[p] << "# Non-terminals" << endl;
		gram[p] << "# S " << _symbols[e]["S"] << endl; 
	}
	nbNonTerminals++;
	
	
	
	itSkills = _SSP->emplSkills[e].begin();
	while(itSkills != _SSP->emplSkills[e].end()){
		if(*itSkills<_SSP->nbAct)
		{
			symbol.clear();symbol += "A_n_";as.str(""); as << *itSkills; symbol += as.str();
			mapSymbol(symbol, e);
			for (int p=0; p<_SSP->nbPieces[e]; ++p)
				gram[p] << "# A_n_" << *itSkills << " " << _symbols[e][symbol] << endl; 
			nbNonTerminals++;
			
			symbol.clear();symbol += "A_";as2.str(""); as2 << *itSkills; symbol += as2.str();
			mapSymbol(symbol, e);
			for (int p=0; p<_SSP->nbPieces[e]; ++p)	
				gram[p] << "# A_" << *itSkills << " "<< _symbols[e][symbol] << endl; 
			nbNonTerminals++;
			
			symbol.clear();symbol += "A_";as3.str(""); as3 << *itSkills; symbol += as3.str();symbol += "_2";
			mapSymbol(symbol, e);
			for (int p=0; p<_SSP->nbPieces[e]; ++p)
				gram [p]<< "# A_" << *itSkills << "_" << 2 << " "<< _symbols[e][symbol] << endl; 
			nbNonTerminals++;
			
			symbol.clear();symbol += "A_";as3.str(""); as3 << *itSkills; symbol += as3.str();symbol += "_3";
			mapSymbol(symbol, e);
			for (int p=0; p<_SSP->nbPieces[e]; ++p)
				gram [p]<< "# A_" << *itSkills << "_" << 3 << " "<< _symbols[e][symbol] << endl; 
			nbNonTerminals++;
			
		}
		else{
			symbol.clear();symbol += "A_n_T";as.str(""); as << *itSkills-_SSP->nbAct; symbol += as.str();
			mapSymbol(symbol, e);
			for (int p=0; p<_SSP->nbPieces[e]; ++p)
				gram[p] << "# "<<symbol << " " << _symbols[e][symbol] << endl; 
			nbNonTerminals++;
			
			symbol.clear();symbol += "A_T";as2.str(""); as2 << *itSkills-_SSP->nbAct; symbol += as2.str();
			mapSymbol(symbol, e);
			for (int p=0; p<_SSP->nbPieces[e]; ++p)	
				gram[p] << "# "<<symbol << " "<< _symbols[e][symbol] << endl; 
			nbNonTerminals++;
			
			symbol.clear();symbol += "A_T";as3.str(""); as3 << *itSkills-_SSP->nbAct; symbol += as3.str();symbol += "_2";
			mapSymbol(symbol, e);
			for (int p=0; p<_SSP->nbPieces[e]; ++p)
				gram [p]<< "# "<<symbol << " "<< _symbols[e][symbol] << endl; 
			nbNonTerminals++;
			
			symbol.clear();symbol += "A_T";as3.str(""); as3 << *itSkills-_SSP->nbAct; symbol += as3.str();symbol += "_3";
			mapSymbol(symbol, e);
			for (int p=0; p<_SSP->nbPieces[e]; ++p)
				gram [p]<< "# "<<symbol << " "<< _symbols[e][symbol] << endl; 
			nbNonTerminals++;
			
		}
		itSkills++;
	}
	
	
	for (int p=0; p<_SSP->nbPieces[e]; ++p)
		gram[p] << endl;
	
	nbNonTerminals = nbNonTerminals - nbTerminals;
	
	
	
	
	
	//NbProductions
	int nbProductions =  nbActE * (5 + (nbActE - 1)*2); /*calcul a priori du nombre de production*/
	itSkills = _SSP->emplSkills[e].begin();
	while(itSkills != _SSP->emplSkills[e].end()){
		if(*itSkills<_SSP->nbAct){
			if(_SSP->maxAct[*itSkills] >= _SSP->maxLengthP[e]) ++nbProductions;
		}
		else 
			if(_SSP->lTask[*itSkills-_SSP->nbAct] >= _SSP->maxLengthP[e]) ++nbProductions;
		
		++itSkills;
	}
	
	//NbTerminals NbNonTerminals NbProductions
	for (int p=0; p<_SSP->nbPieces[e]; ++p)
		gram[p] << nbTerminals << " " << nbNonTerminals << " " << nbProductions << endl;
	
	//Ensemble des productions
	
	itSkills = _SSP->emplSkills[e].begin();
	while(itSkills != _SSP->emplSkills[e].end()){
		if(*itSkills<_SSP->nbAct){
			symbolB.clear(); symbolB += "A_"; asB.str(""); asB << *itSkills; symbolB += asB.str();
			symbolC.clear(); symbolC += "A_n_"; asC.str(""); asC << *itSkills; symbolC += asC.str();
			for (int p=0; p<_SSP->nbPieces[e]; ++p){
				gram[p] << "# S = A_"<<*itSkills << "A_n_" << *itSkills <<"[_segMinLength, _segMaxLength]" << endl;
				gram[p] << "15.0 " << _SSP->psl[e][p] << " " << _SSP->psl[e][p] << " 2 "<< _symbols[e]["S"] << " " << _symbols[e][symbolB]  << " " << _symbols[e][symbolC] << " " << 0 << " " << _SSP->psl[e][p] << endl;
			}
			
			if(_SSP->maxAct[*itSkills] >= _SSP->maxLengthP[e]){
				symbolB.clear(); symbolB += "A_"; asB.str(""); asB << *itSkills; symbolB += asB.str(); symbolB += "_2";
				symbolC.clear(); symbolC += "A_"; asC.str(""); asC << *itSkills; symbolC += asC.str(); symbolC += "_3";
				for (int p=0; p<_SSP->nbPieces[e]; ++p){
					gram[p] << "# S = "<<symbolB << symbolC << " [_segMinLength, _segMaxLength]" << endl;
					gram[p] << "0.0 " << _SSP->psl[e][p] << " " << _SSP->psl[e][p] << " 2 "<< _symbols[e]["S"] << " " << _symbols[e][symbolB]  << " " << _symbols[e][symbolC] << " " << 0 << " " << _SSP->psl[e][p] << endl;	
				}
			}
			
			
			set<int>::iterator itSkillsp = _SSP->emplSkills[e].begin();
			while(itSkillsp != _SSP->emplSkills[e].end()){
				if(*itSkills != *itSkillsp){
					if(*itSkillsp<_SSP->nbAct){
						symbolA.clear(); symbolA += "A_n_"; asA.str(""); asA << *itSkillsp; symbolA += asA.str();
						symbolB.clear(); symbolB += "A_"; asB.str(""); asB << *itSkills; symbolB += asB.str();
						symbolC.clear(); symbolC += "A_n_"; asC.str(""); asC << *itSkills; symbolC += asC.str();
						for (int p=0; p<_SSP->nbPieces[e]; ++p){
							gram[p] << "# A_n_" << *itSkillsp << " = A_"<< *itSkills << "A_n_" << *itSkills << " | A_" << *itSkills << "_2A_" << *itSkills << "_3" << endl;
							gram[p] << "15.0 0 9999 2 "<< _symbols[e][symbolA] << " " << _symbols[e][symbolB]  << " " << _symbols[e][symbolC] << " " << 0 << " " << _SSP->psl[e][p] << endl;
						}
						symbolB.clear(); symbolB += "A_"; asB.str(""); asB << *itSkills; symbolB += asB.str(); symbolB += "_2";
						symbolC.clear(); symbolC += "A_"; asC.str(""); asC << *itSkills; symbolC += asC.str(); symbolC += "_3";
						for (int p=0; p<_SSP->nbPieces[e]; ++p)
							gram[p] << "0.0 " << _SSP->minAct[*itSkills] << " " << _SSP->maxAct[*itSkills] << " 2 "<< _symbols[e][symbolA] << " " << _symbols[e][symbolB]  << " " << _symbols[e][symbolC] << " " << 0 << " " << _SSP->psl[e][p] << endl;
					}
					else{
						symbolA.clear(); symbolA += "A_n_T"; asA.str(""); asA << *itSkillsp-_SSP->nbAct; symbolA += asA.str();
						symbolB.clear(); symbolB += "A_"; asB.str(""); asB << *itSkills; symbolB += asB.str();
						symbolC.clear(); symbolC += "A_n_"; asC.str(""); asC << *itSkills; symbolC += asC.str();
						for (int p=0; p<_SSP->nbPieces[e]; ++p){
							gram[p] << "# A_n_T" << *itSkillsp-_SSP->nbAct << " = A_"<< *itSkills << "A_n_" << *itSkills << " | A_" << *itSkills << "_2A_" << *itSkills << "_3" << endl;
							gram[p] << "15.0 0 9999 2 "<< _symbols[e][symbolA] << " " << _symbols[e][symbolB]  << " " << _symbols[e][symbolC] << " " << 0 << " " << _SSP->psl[e][p] << endl;
						}
						symbolB.clear(); symbolB += "A_"; asB.str(""); asB << *itSkills; symbolB += asB.str(); symbolB += "_2";
						symbolC.clear(); symbolC += "A_"; asC.str(""); asC << *itSkills; symbolC += asC.str(); symbolC += "_3";
						for (int p=0; p<_SSP->nbPieces[e]; ++p)
							gram[p] << "0.0 " << _SSP->minAct[*itSkills] << " " << _SSP->maxAct[*itSkills] << " 2 "<< _symbols[e][symbolA] << " " << _symbols[e][symbolB]  << " " << _symbols[e][symbolC] << " " << 0 << " " << _SSP->psl[e][p] << endl;
						
					}
				}
				itSkillsp++;
			}
			
			symbolA.clear(); symbolA += "A_"; asA.str(""); asA << *itSkills; symbolA += asA.str();
			symbolB.clear(); symbolB += "A_"; asB.str(""); asB << *itSkills; symbolB += asB.str(); symbolB += "_2";
			symbolC.clear(); symbolC += "A_"; asC.str(""); asC << *itSkills; symbolC += asC.str(); symbolC += "_3";
			for (int p=0; p<_SSP->nbPieces[e]; ++p){
				gram[p] << "# A_" << *itSkills << " = A_" << *itSkills << "_2A_" << *itSkills << "_3 [_minAct,_maxAct]"<<endl;  
				gram[p] << "0.0 " << _SSP->minAct[*itSkills] << " " << _SSP->maxAct[*itSkills] << " 2 "<< _symbols[e][symbolA] << " " << _symbols[e][symbolB]  << " " << _symbols[e][symbolC] << " " << 0 << " " << _SSP->psl[e][p] << endl;
			}
			
			symbolA.clear(); symbolA += "A_"; asA.str(""); asA << *itSkills; symbolA += asA.str(); symbolA += "_2";
			symbolB.clear(); symbolB += "a_"; asB.str(""); asB << *itSkills; symbolB += asB.str();
			symbolC.clear(); symbolC += "A_"; asC.str(""); asC << *itSkills; symbolC += asC.str(); symbolC += "_3";
			for (int p=0; p<_SSP->nbPieces[e]; ++p){
				gram[p] << "# A_" << *itSkills << "_2 = A_" << *itSkills << "_2A_" << *itSkills << "_3 | a_" << *itSkills << endl;
				gram[p] << "0.0 0 9999 2 "<< _symbols[e][symbolA] << " " << _symbols[e][symbolA]  << " " << _symbols[e][symbolC] << " " << 0 << " " << _SSP->psl[e][p] << endl;
				gram[p] << "0.0 0 9999 1 "<< _symbols[e][symbolA] << " " << _symbols[e][symbolB] << " 0 " << _SSP->psl[e][p] << endl;
				
				gram[p] << "# A_" << *itSkills << "_3 = a_" << *itSkills << endl;
				gram[p] << "0.0 0 9999 1 "<< _symbols[e][symbolC] << " " << _symbols[e][symbolB] << " 0 " << _SSP->psl[e][p] << endl;
				
				gram[p] << endl;
			}
		}
		else {
			symbolB.clear(); symbolB += "A_T"; asB.str(""); asB << *itSkills-_SSP->nbAct; symbolB += asB.str();
			symbolC.clear(); symbolC += "A_n_T"; asC.str(""); asC << *itSkills-_SSP->nbAct; symbolC += asC.str();
			for (int p=0; p<_SSP->nbPieces[e]; ++p){
				gram[p] << "# S = A_T"<<*itSkills-_SSP->nbAct << "A_n_T" << *itSkills-_SSP->nbAct <<"[_segMinLength, _segMaxLength]" << endl;
				gram[p] << "15.0 " << _SSP->psl[e][p] << " " << _SSP->psl[e][p] << " 2 "<< _symbols[e]["S"] << " " << _symbols[e][symbolB]  << " " << _symbols[e][symbolC] << " " << 0 << " " << _SSP->psl[e][p] << endl;
			}
			
			if(_SSP->lTask[*itSkills-_SSP->nbAct] >= _SSP->maxLengthP[e]){
				symbolB.clear(); symbolB += "A_T"; asB.str(""); asB << *itSkills-_SSP->nbAct; symbolB += asB.str(); symbolB += "_2";
				symbolC.clear(); symbolC += "A_T"; asC.str(""); asC << *itSkills-_SSP->nbAct; symbolC += asC.str(); symbolC += "_3";
				
				for (int p=0; p<_SSP->nbPieces[e]; ++p){
					int startT=_SSP->wsTask[*itSkills-_SSP->nbAct]-_SSP->pss[e][p]; 
					if(startT<0) startT=0;
					int endT=_SSP->wsTask[*itSkills-_SSP->nbAct]+_SSP->wlTask[*itSkills-_SSP->nbAct]-_SSP->pss[e][p];
					if(endT<0) endT=0;
					gram[p] << "# S = A_T" << *itSkills-_SSP->nbAct << "_2A_T" << *itSkills-_SSP->nbAct << "_3 [_segMinLength, _segMaxLength]" << endl;
					gram[p] << "0.0 " << _SSP->lTask[*itSkills-_SSP->nbAct] << " " << _SSP->lTask[*itSkills-_SSP->nbAct] << " 2 "<< _symbols[e]["S"] << " " << _symbols[e][symbolB]  << " " << _symbols[e][symbolC] << " " << startT << " " << endT-startT << endl;	
				}
			}
			
			set<int>::iterator itSkillsp = _SSP->emplSkills[e].begin();
			while(itSkillsp != _SSP->emplSkills[e].end()){
				if(*itSkills != *itSkillsp){
					if(*itSkillsp<_SSP->nbAct){
						symbolA.clear(); symbolA += "A_n_"; asA.str(""); asA << *itSkillsp; symbolA += asA.str();
						symbolB.clear(); symbolB += "A_T"; asB.str(""); asB << *itSkills-_SSP->nbAct; symbolB += asB.str();
						symbolC.clear(); symbolC += "A_n_T"; asC.str(""); asC << *itSkills-_SSP->nbAct; symbolC += asC.str();
						for (int p=0; p<_SSP->nbPieces[e]; ++p){
							gram[p] << "# A_n_" << *itSkillsp << " = A_T"<< *itSkills-_SSP->nbAct << "A_n_T" << *itSkills-_SSP->nbAct << " | A_T" << *itSkills-_SSP->nbAct << "_2A_T" << *itSkills-_SSP->nbAct << "_3" << endl;
							gram[p] << "15.0 0 9999 2 "<< _symbols[e][symbolA] << " " << _symbols[e][symbolB]  << " " << _symbols[e][symbolC] << " " << 0 << " " << _SSP->psl[e][p] << endl;
						}
						symbolB.clear(); symbolB += "A_T"; asB.str(""); asB << *itSkills-_SSP->nbAct; symbolB += asB.str(); symbolB += "_2";
						symbolC.clear(); symbolC += "A_T"; asC.str(""); asC << *itSkills-_SSP->nbAct; symbolC += asC.str(); symbolC += "_3";
						for (int p=0; p<_SSP->nbPieces[e]; ++p){
							
							int startT=_SSP->wsTask[*itSkills-_SSP->nbAct]-_SSP->pss[e][p]; 
							if(startT<0) startT=0;
							int endT=_SSP->wsTask[*itSkills-_SSP->nbAct]+_SSP->wlTask[*itSkills-_SSP->nbAct]-_SSP->pss[e][p];
							if(endT<0) endT=0;
							gram[p] << "0.0 " <<  _SSP->lTask[*itSkills-_SSP->nbAct] << " " <<  _SSP->lTask[*itSkills-_SSP->nbAct] << " 2 "<< _symbols[e][symbolA] << " " << _symbols[e][symbolB]  << " " << _symbols[e][symbolC] << " " << startT << " " << endT-startT << endl;
							
						}
					}
					else{
						symbolA.clear(); symbolA += "A_n_T"; asA.str(""); asA << *itSkillsp-_SSP->nbAct; symbolA += asA.str();
						symbolB.clear(); symbolB += "A_T"; asB.str(""); asB << *itSkills-_SSP->nbAct; symbolB += asB.str();
						symbolC.clear(); symbolC += "A_n_T"; asC.str(""); asC << *itSkills-_SSP->nbAct; symbolC += asC.str();
						for (int p=0; p<_SSP->nbPieces[e]; ++p){
							gram[p] << "# A_n_T" << *itSkillsp-_SSP->nbAct << " = A_T"<< *itSkills-_SSP->nbAct << "A_n_T" << *itSkills-_SSP->nbAct << " | A_T" << *itSkills-_SSP->nbAct << "_2A_T" << *itSkills-_SSP->nbAct << "_3" << endl;
							gram[p] << "15.0 0 9999 2 "<< _symbols[e][symbolA] << " " << _symbols[e][symbolB]  << " " << _symbols[e][symbolC] << " " << 0 << " " << _SSP->psl[e][p] << endl;
						}
						symbolB.clear(); symbolB += "A_T"; asB.str(""); asB << *itSkills-_SSP->nbAct; symbolB += asB.str(); symbolB += "_2";
						symbolC.clear(); symbolC += "A_T"; asC.str(""); asC << *itSkills-_SSP->nbAct; symbolC += asC.str(); symbolC += "_3";
						for (int p=0; p<_SSP->nbPieces[e]; ++p){
							int startT=_SSP->wsTask[*itSkills-_SSP->nbAct]-_SSP->pss[e][p]; 
							if(startT<0) startT=0;
							int endT=_SSP->wsTask[*itSkills-_SSP->nbAct]+_SSP->wlTask[*itSkills-_SSP->nbAct]-_SSP->pss[e][p];
							if(endT<0) endT=0;
							gram[p] << "0.0 " << _SSP->lTask[*itSkills-_SSP->nbAct] << " " << _SSP->lTask[*itSkills-_SSP->nbAct] << " 2 "<< _symbols[e][symbolA] << " " << _symbols[e][symbolB]  << " " << _symbols[e][symbolC] << " " << startT << " " << endT-startT << endl;
						}
					}
				}
				itSkillsp++;
			}
			
			symbolA.clear(); symbolA += "A_T"; asA.str(""); asA << *itSkills-_SSP->nbAct; symbolA += asA.str();
			symbolB.clear(); symbolB += "A_T"; asB.str(""); asB << *itSkills-_SSP->nbAct; symbolB += asB.str(); symbolB += "_2";
			symbolC.clear(); symbolC += "A_T"; asC.str(""); asC << *itSkills-_SSP->nbAct; symbolC += asC.str(); symbolC += "_3";
			
			for (int p=0; p<_SSP->nbPieces[e]; ++p){
				int startT=_SSP->wsTask[*itSkills-_SSP->nbAct]-_SSP->pss[e][p]; 
				if(startT<0) startT=0;
				int endT=_SSP->wsTask[*itSkills-_SSP->nbAct]+_SSP->wlTask[*itSkills-_SSP->nbAct]-_SSP->pss[e][p];
				if(endT<0) endT=0;
				gram[p] << "# A_T" << *itSkills-_SSP->nbAct << " = A_T" << *itSkills-_SSP->nbAct << "_2A_T" << *itSkills-_SSP->nbAct << "_3 [_minAct,_maxAct]"<<endl;  
				gram[p] << "0.0 " << _SSP->lTask[*itSkills-_SSP->nbAct] << " " << _SSP->lTask[*itSkills-_SSP->nbAct] << " 2 "<< _symbols[e][symbolA] << " " << _symbols[e][symbolB]  << " " << _symbols[e][symbolC] << " " << startT << " " << endT-startT << endl;
			}
			
			symbolA.clear(); symbolA += "A_T"; asA.str(""); asA << *itSkills-_SSP->nbAct; symbolA += asA.str(); symbolA += "_2";
			symbolB.clear(); symbolB += "t_"; asB.str(""); asB << *itSkills-_SSP->nbAct; symbolB += asB.str();
			symbolC.clear(); symbolC += "A_T"; asC.str(""); asC << *itSkills-_SSP->nbAct; symbolC += asC.str(); symbolC += "_3";
			symbol.clear(); symbol += "st_"; as.str(""); as << *itSkills-_SSP->nbAct; symbol += as.str();
			for (int p=0; p<_SSP->nbPieces[e]; ++p){
				int startT=_SSP->wsTask[*itSkills-_SSP->nbAct]-_SSP->pss[e][p]; 
				if(startT<0) startT=0;
				int endT=_SSP->wsTask[*itSkills-_SSP->nbAct]+_SSP->wlTask[*itSkills-_SSP->nbAct]-_SSP->pss[e][p];
				if(endT<0) endT=0;
				gram[p] << "# A_T" << *itSkills-_SSP->nbAct << "_2 = A_T" << *itSkills-_SSP->nbAct << "_2A_T" << *itSkills-_SSP->nbAct << "_3 | st_" << *itSkills-_SSP->nbAct << endl;
				gram[p] << "0.0 0 "<<_SSP->lTask[*itSkills-_SSP->nbAct]<<" 2 "<< _symbols[e][symbolA] << " " << _symbols[e][symbolA]  << " " << _symbols[e][symbolC] << " " << startT << " " << endT-startT << endl;
				gram[p] << "0.0 1 1 1 "<< _symbols[e][symbolA] << " " << _symbols[e][symbol] << " " << startT << " " << endT-startT << endl;
				
				gram[p] << "# A_T" << *itSkills-_SSP->nbAct << "_3 = t_" << *itSkills-_SSP->nbAct << endl;
				gram[p] << "0.0 1 1 1 "<< _symbols[e][symbolC] << " " << _symbols[e][symbolB] << " " << startT << " " << endT-startT<< endl;
				
				gram[p] << endl;
				//gram[p] << "# A_T" << *itSkills-_SSP->nbAct << "_2 = A_T" << *itSkills-_SSP->nbAct << "_2A_T" << *itSkills-_SSP->nbAct << "_3 | st_" << *itSkills-_SSP->nbAct << endl;
				//				gram[p] << "0.0 0 9999 2 "<< _symbols[e][symbolA] << " " << _symbols[e][symbolA]  << " " << _symbols[e][symbolC] << " " << 0 << " " << _SSP->psl[e][p] << endl;
				//				gram[p] << "0.0 0 9999 1 "<< _symbols[e][symbolA] << " " << _symbols[e][symbol] << " 0 " << _SSP->psl[e][p] << endl;
				//				
				//				gram[p] << "# A_T" << *itSkills-_SSP->nbAct << "_3 = t_" << *itSkills-_SSP->nbAct << endl;
				//				gram[p] << "0.0 0 9999 1 "<< _symbols[e][symbolC] << " " << _symbols[e][symbolB] << " 0 " << _SSP->psl[e][p] << endl;
				//				gram[p] << endl;
			}
		}
		
		
		itSkills++;
		
	}
	

	
	for (int p=0; p<_SSP->nbPieces[e]; ++p)
		gram[p].close();
    
    delete[] gram;

	
	
}


//With starting nodes st for tasks...
//With breaks...
void SubP::createGrammar2(int e){
	int nbTerminals, nbNonTerminals;
	
	int nbActE = 0;
	
	string symbol;
	string symbolA;
	string symbolB;
	string symbolC;
	
	ostringstream as;
	ostringstream as2;
	ostringstream as3;
	ostringstream asA;
	ostringstream asB;
	ostringstream asC;
	
	//List of terminals
	//List of non-terminals
	//Productions
	
	
	/*nom des fichiers de sortie*/
	_grammarFile=new string[_SSP->nbPieces[e]];
	for (int p=0; p<_SSP->nbPieces[e]; ++p) {
		_grammarFile[p] = "Grammar/gr_";
		as.str(""); as << e;
		_grammarFile[p] += as.str();
		_grammarFile[p] += "_";
		as.str(""); as << p;
		_grammarFile[p] += as.str();
	}
	
	
	
	
	
	/*_symbols: contient le mapping des symbol des noeuds avec son numéro d'identification*/
	
	//Tasks could appear more than once in a shift...
	
	
	//creation de la grammaire
	ofstream *gram=new ofstream[_SSP->nbPieces[e]];
	for (int p=0; p<_SSP->nbPieces[e]; ++p){
		gram[p].open(_grammarFile[p].c_str(), ios::trunc);
		gram[p] << "# Terminals" << endl;
	}
	
	nbTerminals = 0;
	set<int>::iterator itSkills = _SSP->emplSkills[e].begin();
	while(itSkills != _SSP->emplSkills[e].end()){
		if(*itSkills<0){itSkills++; continue;}
		if(*itSkills < _SSP->nbAct){
			symbol.clear();symbol += "a_";as.str("");as << *itSkills;symbol += as.str(); 
			mapSymbol(symbol, e);
			for (int p=0; p<_SSP->nbPieces[e]; ++p)
				gram[p] << "# a_" << *itSkills << " "<< _symbols[e][symbol] << endl; 
			nbTerminals++;
			nbActE++;
		}
		else{
			symbol.clear();symbol += "t_";as.str("");as << *itSkills-_SSP->nbAct;symbol += as.str();
			mapSymbol(symbol, e);
			for (int p=0; p<_SSP->nbPieces[e]; ++p)
				gram[p] << "# t_" << *itSkills-_SSP->nbAct << " "<< _symbols[e][symbol] << endl;
			nbTerminals++;
			nbActE++;
			
			symbol.clear();symbol += "st_";as.str("");as << *itSkills-_SSP->nbAct;symbol += as.str();
			mapSymbol(symbol, e);
			for (int p=0; p<_SSP->nbPieces[e]; ++p)
				gram[p] << "# st_" << *itSkills-_SSP->nbAct << " "<< _symbols[e][symbol] << endl;
			nbTerminals++;
			//nbActE++;
		}
		itSkills++;
	}	
	symbol.clear();symbol += "b"; 
	mapSymbol(symbol, e);
	for (int p=0; p<_SSP->nbPieces[e]; ++p)
		gram[p] << "# "<<symbol << " "<< _symbols[e][symbol] << endl; 
	nbTerminals++;
	
	
	
	nbNonTerminals = nbTerminals;
	mapSymbol("S", e);
	for (int p=0; p<_SSP->nbPieces[e]; ++p){
		gram[p] << "# Non-terminals" << endl;
		gram[p] << "# S " << _symbols[e]["S"] << endl; 
	}
	nbNonTerminals++;
	
	
	
	itSkills = _SSP->emplSkills[e].begin();
	while(itSkills != _SSP->emplSkills[e].end()) {
		if(*itSkills<0){itSkills++; continue;}
		if(*itSkills<_SSP->nbAct)
		{
			symbol.clear();symbol += "A_n_";as.str(""); as << *itSkills; symbol += as.str();
			mapSymbol(symbol, e);
			for (int p=0; p<_SSP->nbPieces[e]; ++p)
				gram[p] << "# A_n_" << *itSkills << " " << _symbols[e][symbol] << endl; 
			nbNonTerminals++;
			
			symbol.clear();symbol += "A_";as2.str(""); as2 << *itSkills; symbol += as2.str();
			mapSymbol(symbol, e);
			for (int p=0; p<_SSP->nbPieces[e]; ++p)	
				gram[p] << "# A_" << *itSkills << " "<< _symbols[e][symbol] << endl; 
			nbNonTerminals++;
			
			symbol.clear();symbol += "A_";as3.str(""); as3 << *itSkills; symbol += as3.str();symbol += "_2";
			mapSymbol(symbol, e);
			for (int p=0; p<_SSP->nbPieces[e]; ++p)
				gram [p]<< "# A_" << *itSkills << "_" << 2 << " "<< _symbols[e][symbol] << endl; 
			nbNonTerminals++;
			
			symbol.clear();symbol += "A_";as3.str(""); as3 << *itSkills; symbol += as3.str();symbol += "_3";
			mapSymbol(symbol, e);
			for (int p=0; p<_SSP->nbPieces[e]; ++p)
				gram [p]<< "# A_" << *itSkills << "_" << 3 << " "<< _symbols[e][symbol] << endl; 
			nbNonTerminals++;
			
		}
		else{
			symbol.clear();symbol += "A_n_T";as.str(""); as << *itSkills-_SSP->nbAct; symbol += as.str();
			mapSymbol(symbol, e);
			for (int p=0; p<_SSP->nbPieces[e]; ++p)
				gram[p] << "# "<<symbol << " " << _symbols[e][symbol] << endl; 
			nbNonTerminals++;
			
			symbol.clear();symbol += "A_T";as2.str(""); as2 << *itSkills-_SSP->nbAct; symbol += as2.str();
			mapSymbol(symbol, e);
			for (int p=0; p<_SSP->nbPieces[e]; ++p)	
				gram[p] << "# "<<symbol << " "<< _symbols[e][symbol] << endl; 
			nbNonTerminals++;
			
			symbol.clear();symbol += "A_T";as3.str(""); as3 << *itSkills-_SSP->nbAct; symbol += as3.str();symbol += "_2";
			mapSymbol(symbol, e);
			for (int p=0; p<_SSP->nbPieces[e]; ++p)
				gram [p]<< "# "<<symbol << " "<< _symbols[e][symbol] << endl; 
			nbNonTerminals++;
			
			symbol.clear();symbol += "A_T";as3.str(""); as3 << *itSkills-_SSP->nbAct; symbol += as3.str();symbol += "_3";
			mapSymbol(symbol, e);
			for (int p=0; p<_SSP->nbPieces[e]; ++p)
				gram [p]<< "# "<<symbol << " "<< _symbols[e][symbol] << endl; 
			nbNonTerminals++;
			
		}
		itSkills++;
	}
	
	//for definition of the complete shift
	symbol.clear();symbol += "Sam";
	mapSymbol(symbol, e);
	for (int p=0; p<_SSP->nbPieces[e]; ++p)
		gram [p]<< "# "<<symbol << " "<< _symbols[e][symbol] << endl; 
	nbNonTerminals++;
	symbol.clear();symbol += "Spm";
	mapSymbol(symbol, e);
	for (int p=0; p<_SSP->nbPieces[e]; ++p)
		gram [p]<< "# "<<symbol << " "<< _symbols[e][symbol] << endl; 
	nbNonTerminals++;
	symbol.clear();symbol += "B";
	mapSymbol(symbol, e);
	for (int p=0; p<_SSP->nbPieces[e]; ++p)
		gram [p]<< "# "<<symbol << " "<< _symbols[e][symbol] << endl; 
	nbNonTerminals++;
	symbol.clear();symbol += "Bp";
	mapSymbol(symbol, e);
	for (int p=0; p<_SSP->nbPieces[e]; ++p)
		gram [p]<< "# "<<symbol << " "<< _symbols[e][symbol] << endl; 
	nbNonTerminals++;
	symbol.clear();symbol += "B_2";
	mapSymbol(symbol, e);
	for (int p=0; p<_SSP->nbPieces[e]; ++p)
		gram [p]<< "# "<<symbol << " "<< _symbols[e][symbol] << endl; 
	nbNonTerminals++;
//	symbol.clear();symbol += "B_3";
//	mapSymbol(symbol, e);
//	for (int p=0; p<_SSP->nbPieces[e]; ++p)
//		gram [p]<< "# "<<symbol << " "<< _symbols[e][symbol] << endl; 
//	nbNonTerminals++;
	symbol.clear();symbol += "A";
	mapSymbol(symbol, e);
	for (int p=0; p<_SSP->nbPieces[e]; ++p)
		gram [p]<< "# "<<symbol << " "<< _symbols[e][symbol] << endl; 
	nbNonTerminals++;
	symbol.clear();symbol += "Ab";
	mapSymbol(symbol, e);
	for (int p=0; p<_SSP->nbPieces[e]; ++p)
		gram [p]<< "# "<<symbol << " "<< _symbols[e][symbol] << endl; 
	nbNonTerminals++;
	
	
	for (int p=0; p<_SSP->nbPieces[e]; ++p)
		gram[p] << endl;
	
	nbNonTerminals = nbNonTerminals - nbTerminals;
	
	
	
	
	
	//NbProductions
	int nbProductions =  nbActE * (5 + (nbActE - 1)*2)+12; /*calcul a priori du nombre de production*/
	itSkills = _SSP->emplSkills[e].begin();
	while(itSkills != _SSP->emplSkills[e].end()) {
		if(*itSkills<0){itSkills++; continue;}
		if(*itSkills<_SSP->nbAct){
			if(_SSP->maxAct[*itSkills] >= _SSP->maxLengthP[e]) ++nbProductions;
		}
		else 
			if(_SSP->lTask[*itSkills-_SSP->nbAct] >= _SSP->maxLengthP[e]) ++nbProductions;
		
		++itSkills;
	}
	
	//si journée complete de 8h seulement
	//nbProductions-=2;
	
	//NbTerminals NbNonTerminals NbProductions
	for (int p=0; p<_SSP->nbPieces[e]; ++p)
		gram[p] << nbTerminals << " " << nbNonTerminals << " " << nbProductions << endl;
	
	//Ensemble des productions
	
	//for definition of the complete shift
	symbol.clear();symbol += "S";
	symbolA.clear(); symbolA += "Sam";
	symbolB.clear(); symbolB += "Spm";
	symbolC.clear(); symbolC += "B";
	for (int p=0; p<_SSP->nbPieces[e]; ++p){
		gram[p] << "#"<<symbol<<" = "<<symbolA<<symbolB<<" | "<<symbolA<<symbolC<<" | "<<symbolC<<"A"<<" | "<<"A"<<symbolB<<"[_segMinLength, _segMaxLength]" << endl;
		gram[p] << "0.0 " << _SSP->psl[e][p] << " " << _SSP->psl[e][p] << " 2 "<< _symbols[e]["S"] << " " << _symbols[e][symbolA]  << " " << _symbols[e][symbolB] << " " << 0 << " " << _SSP->psl[e][p] << endl;
		gram[p] << "0.0 " << _SSP->psl[e][p] << " " << _SSP->psl[e][p] << " 2 "<< _symbols[e]["S"] << " " << _symbols[e][symbolA]  << " " << _symbols[e][symbolC] << " " << 0 << " " << _SSP->psl[e][p] << endl;
		gram[p] << "0.0 " << _SSP->psl[e][p] << " " << _SSP->psl[e][p] << " 2 "<< _symbols[e]["S"] << " " << _symbols[e][symbolC]  << " " << _symbols[e]["A"] << " " << 0 << " " << _SSP->psl[e][p] << endl;
		gram[p] << "0.0 " << _SSP->psl[e][p] << " " << _SSP->psl[e][p] << " 2 "<< _symbols[e]["S"] << " " << _symbols[e]["A"]  << " " << _symbols[e][symbolB] << " " << 0 << " " << _SSP->psl[e][p] << endl;
	}
	for (int p=0; p<_SSP->nbPieces[e]; ++p)
		gram[p] << endl;
	
	//Sam
	symbol.clear();symbol += "Sam";
	symbolA.clear(); symbolA += "B";
	symbolB.clear(); symbolB += "A";
	for (int p=0; p<_SSP->nbPieces[e]; ++p){
		gram[p] << "#"<<symbol<<" = "<<symbolA<<symbolB<< endl;
		gram[p] << "0.0 " << 0 << " " << _SSP->psl[e][p] << " 2 "<< _symbols[e][symbol] << " " << _symbols[e][symbolA]  << " " << _symbols[e][symbolB] << " " << 0 << " " << _SSP->psl[e][p] << endl;
	}
	for (int p=0; p<_SSP->nbPieces[e]; ++p)
		gram[p] << endl;
	
	//Spm
	symbol.clear();symbol += "Spm";
	symbolA.clear(); symbolA += "Bp";
	symbolB.clear(); symbolB += "A";
	symbolC.clear(); symbolC += "Ab";
	for (int p=0; p<_SSP->nbPieces[e]; ++p){
		gram[p] << "#"<<symbol<<" = "<<symbolA<<symbolB<<" | "<<symbolA<<symbolC<< endl;
		gram[p] << "0.0 " << 0 << " " << _SSP->psl[e][p] << " 2 "<< _symbols[e][symbol] << " " << _symbols[e][symbolA]  << " " << _symbols[e][symbolB] << " " << 0 << " " << _SSP->psl[e][p] << endl;
		gram[p] << "0.0 " << 0 << " " << _SSP->psl[e][p] << " 2 "<< _symbols[e][symbol] << " " << _symbols[e][symbolA]  << " " << _symbols[e][symbolC] << " " << 0 << " " << _SSP->psl[e][p] << endl;
	}
	symbol.clear();symbol += "Ab";
	symbolA.clear(); symbolA += "A";
	symbolB.clear(); symbolB += "B";
	for (int p=0; p<_SSP->nbPieces[e]; ++p){
		gram[p] << "#"<<symbol<<" = "<<symbolA<<symbolB<< endl;
		gram[p] << "0.0 " << 0 << " " << _SSP->psl[e][p] << " 2 "<< _symbols[e][symbol] << " " << _symbols[e][symbolA]  << " " << _symbols[e][symbolB] << " " << 0 << " " << _SSP->psl[e][p] << endl;
	}
	for (int p=0; p<_SSP->nbPieces[e]; ++p)
		gram[p] << endl;
	
	//breaks
	symbol.clear();symbol += "B";
	symbolA.clear(); symbolA += "B_2";
	//symbolB.clear(); symbolB += "B_3";
	symbolC.clear(); symbolC += "b";
	for (int p=0; p<_SSP->nbPieces[e]; ++p){
		gram[p] << "#"<<symbol<<" = "<<symbolA<<symbol<<" | " <<symbolC<< endl;
		gram[p] << "0.0 " << 0 << " " << _SSP->psl[e][p] << " 2 "<< _symbols[e][symbol] << " " << _symbols[e][symbolA]  << " " << _symbols[e][symbol] << " " << 0 << " " << _SSP->psl[e][p] << endl;
		gram[p] << "0.0 " << 0 << " " << _SSP->psl[e][p] << " 1 "<< _symbols[e][symbol] << " " << _symbols[e][symbolC]  << " " << 0 << " " << _SSP->psl[e][p] << endl;
//		gram[p] << "#"<<symbolB<<" = "<<symbolA<<symbolB<<" | "<<symbolC<< endl;
//		gram[p] << "0.0 " << 0 << " " << _SSP->psl[e][p] << " 2 "<< _symbols[e][symbolB] << " " << _symbols[e][symbolA]  << " " << _symbols[e][symbolB] << " " << 0 << " " << _SSP->psl[e][p] << endl;
//		gram[p] << "0.0 " << 0 << " " << _SSP->psl[e][p] << " 1 "<< _symbols[e][symbolB] << " " << _symbols[e][symbolC]  << " " << 0 << " " << _SSP->psl[e][p] << endl;
		gram[p] << "#"<<symbolA<<" = "<<symbolC<< endl;
		gram[p] << "0.0 " << 0 << " " << _SSP->psl[e][p] << " 1 "<< _symbols[e][symbolA] << " " << _symbols[e][symbolC]  << " " << 0 << " " << _SSP->psl[e][p] << endl;
	}
	//Pause entre Sam et Spm de 1h 
	symbol.clear();symbol += "Bp";
	symbolA.clear(); symbolA += "B_2";
	symbolB.clear(); symbolB += "B";
	for (int p=0; p<_SSP->nbPieces[e]; ++p){
		gram[p] << "#"<<symbol<<" = "<<symbolA<<symbolB<< endl;
		gram[p] << "0.0 " << "4" << " " << "4" << " 2 "<< _symbols[e][symbol] << " " << _symbols[e][symbolA]  << " " << _symbols[e][symbolB] << " " << 0 << " " << _SSP->psl[e][p] << endl;
	}
	for (int p=0; p<_SSP->nbPieces[e]; ++p)
		gram[p] << endl;
	
	itSkills = _SSP->emplSkills[e].begin();
	while(itSkills != _SSP->emplSkills[e].end()){
		if(*itSkills<0){itSkills++; continue;}
		if(*itSkills<_SSP->nbAct){
			symbolB.clear(); symbolB += "A_"; asB.str(""); asB << *itSkills; symbolB += asB.str();
			symbolC.clear(); symbolC += "A_n_"; asC.str(""); asC << *itSkills; symbolC += asC.str();
			for (int p=0; p<_SSP->nbPieces[e]; ++p){
				gram[p] << "# A = A_"<<*itSkills << "A_n_" << *itSkills <<"[_segMinLength, _segMaxLength]" << endl;
				gram[p] << "15.0 16 16 2 "<< _symbols[e]["A"] << " " << _symbols[e][symbolB]  << " " << _symbols[e][symbolC] << " " << 0 << " " << _SSP->psl[e][p] << endl;
			}
			
			if(_SSP->maxAct[*itSkills] >= _SSP->maxLengthP[e]){
				symbolB.clear(); symbolB += "A_"; asB.str(""); asB << *itSkills; symbolB += asB.str(); symbolB += "_2";
				symbolC.clear(); symbolC += "A_"; asC.str(""); asC << *itSkills; symbolC += asC.str(); symbolC += "_3";
				for (int p=0; p<_SSP->nbPieces[e]; ++p){
					gram[p] << "# A = "<<symbolB << symbolC << " [_segMinLength, _segMaxLength]" << endl;
					gram[p] << "0.0 16 16 2 "<< _symbols[e]["A"] << " " << _symbols[e][symbolB]  << " " << _symbols[e][symbolC] << " " << 0 << " " << _SSP->psl[e][p] << endl;	
				}
			}
			
			
			set<int>::iterator itSkillsp = _SSP->emplSkills[e].begin();
			while(itSkillsp != _SSP->emplSkills[e].end()){
				if(*itSkillsp<0){itSkillsp++; continue;}
				if(*itSkills != *itSkillsp){
					if(*itSkillsp<_SSP->nbAct){
						symbolA.clear(); symbolA += "A_n_"; asA.str(""); asA << *itSkillsp; symbolA += asA.str();
						symbolB.clear(); symbolB += "A_"; asB.str(""); asB << *itSkills; symbolB += asB.str();
						symbolC.clear(); symbolC += "A_n_"; asC.str(""); asC << *itSkills; symbolC += asC.str();
						for (int p=0; p<_SSP->nbPieces[e]; ++p){
							gram[p] << "# A_n_" << *itSkillsp << " = A_"<< *itSkills << "A_n_" << *itSkills << " | A_" << *itSkills << "_2A_" << *itSkills << "_3" << endl;
							gram[p] << "15.0 0 9999 2 "<< _symbols[e][symbolA] << " " << _symbols[e][symbolB]  << " " << _symbols[e][symbolC] << " " << 0 << " " << _SSP->psl[e][p] << endl;
						}
						symbolB.clear(); symbolB += "A_"; asB.str(""); asB << *itSkills; symbolB += asB.str(); symbolB += "_2";
						symbolC.clear(); symbolC += "A_"; asC.str(""); asC << *itSkills; symbolC += asC.str(); symbolC += "_3";
						for (int p=0; p<_SSP->nbPieces[e]; ++p)
							gram[p] << "0.0 " << _SSP->minAct[*itSkills] << " " << _SSP->maxAct[*itSkills] << " 2 "<< _symbols[e][symbolA] << " " << _symbols[e][symbolB]  << " " << _symbols[e][symbolC] << " " << 0 << " " << _SSP->psl[e][p] << endl;
					}
					else{
						symbolA.clear(); symbolA += "A_n_T"; asA.str(""); asA << *itSkillsp-_SSP->nbAct; symbolA += asA.str();
						symbolB.clear(); symbolB += "A_"; asB.str(""); asB << *itSkills; symbolB += asB.str();
						symbolC.clear(); symbolC += "A_n_"; asC.str(""); asC << *itSkills; symbolC += asC.str();
						for (int p=0; p<_SSP->nbPieces[e]; ++p){
							gram[p] << "# A_n_T" << *itSkillsp-_SSP->nbAct << " = A_"<< *itSkills << "A_n_" << *itSkills << " | A_" << *itSkills << "_2A_" << *itSkills << "_3" << endl;
							gram[p] << "15.0 0 9999 2 "<< _symbols[e][symbolA] << " " << _symbols[e][symbolB]  << " " << _symbols[e][symbolC] << " " << 0 << " " << _SSP->psl[e][p] << endl;
						}
						symbolB.clear(); symbolB += "A_"; asB.str(""); asB << *itSkills; symbolB += asB.str(); symbolB += "_2";
						symbolC.clear(); symbolC += "A_"; asC.str(""); asC << *itSkills; symbolC += asC.str(); symbolC += "_3";
						for (int p=0; p<_SSP->nbPieces[e]; ++p)
							gram[p] << "0.0 " << _SSP->minAct[*itSkills] << " " << _SSP->maxAct[*itSkills] << " 2 "<< _symbols[e][symbolA] << " " << _symbols[e][symbolB]  << " " << _symbols[e][symbolC] << " " << 0 << " " << _SSP->psl[e][p] << endl;
						
					}
				}
				itSkillsp++;
			}
			
			symbolA.clear(); symbolA += "A_"; asA.str(""); asA << *itSkills; symbolA += asA.str();
			symbolB.clear(); symbolB += "A_"; asB.str(""); asB << *itSkills; symbolB += asB.str(); symbolB += "_2";
			symbolC.clear(); symbolC += "A_"; asC.str(""); asC << *itSkills; symbolC += asC.str(); symbolC += "_3";
			for (int p=0; p<_SSP->nbPieces[e]; ++p){
				gram[p] << "# A_" << *itSkills << " = A_" << *itSkills << "_2A_" << *itSkills << "_3 [_minAct,_maxAct]"<<endl;  
				gram[p] << "0.0 " << _SSP->minAct[*itSkills] << " " << _SSP->maxAct[*itSkills] << " 2 "<< _symbols[e][symbolA] << " " << _symbols[e][symbolB]  << " " << _symbols[e][symbolC] << " " << 0 << " " << _SSP->psl[e][p] << endl;
			}
			
			symbolA.clear(); symbolA += "A_"; asA.str(""); asA << *itSkills; symbolA += asA.str(); symbolA += "_2";
			symbolB.clear(); symbolB += "a_"; asB.str(""); asB << *itSkills; symbolB += asB.str();
			symbolC.clear(); symbolC += "A_"; asC.str(""); asC << *itSkills; symbolC += asC.str(); symbolC += "_3";
			for (int p=0; p<_SSP->nbPieces[e]; ++p){
				gram[p] << "# A_" << *itSkills << "_2 = A_" << *itSkills << "_2A_" << *itSkills << "_3 | a_" << *itSkills << endl;
				gram[p] << "0.0 0 9999 2 "<< _symbols[e][symbolA] << " " << _symbols[e][symbolA]  << " " << _symbols[e][symbolC] << " " << 0 << " " << _SSP->psl[e][p] << endl;
				gram[p] << "0.0 0 9999 1 "<< _symbols[e][symbolA] << " " << _symbols[e][symbolB] << " 0 " << _SSP->psl[e][p] << endl;
				
				gram[p] << "# A_" << *itSkills << "_3 = a_" << *itSkills << endl;
				gram[p] << "0.0 0 9999 1 "<< _symbols[e][symbolC] << " " << _symbols[e][symbolB] << " 0 " << _SSP->psl[e][p] << endl;
				
				gram[p] << endl;
			}
		}
		else {
			symbolB.clear(); symbolB += "A_T"; asB.str(""); asB << *itSkills-_SSP->nbAct; symbolB += asB.str();
			symbolC.clear(); symbolC += "A_n_T"; asC.str(""); asC << *itSkills-_SSP->nbAct; symbolC += asC.str();
			for (int p=0; p<_SSP->nbPieces[e]; ++p){
				gram[p] << "# A = A_T"<<*itSkills-_SSP->nbAct << "A_n_T" << *itSkills-_SSP->nbAct <<"[_segMinLength, _segMaxLength]" << endl;
				gram[p] << "15.0 16 16 2 "<< _symbols[e]["A"] << " " << _symbols[e][symbolB]  << " " << _symbols[e][symbolC] << " " << 0 << " " << _SSP->psl[e][p] << endl;
			}
			
			if(_SSP->lTask[*itSkills-_SSP->nbAct] >= _SSP->maxLengthP[e]){
				symbolB.clear(); symbolB += "A_T"; asB.str(""); asB << *itSkills-_SSP->nbAct; symbolB += asB.str(); symbolB += "_2";
				symbolC.clear(); symbolC += "A_T"; asC.str(""); asC << *itSkills-_SSP->nbAct; symbolC += asC.str(); symbolC += "_3";
				
				for (int p=0; p<_SSP->nbPieces[e]; ++p){
					int startT=_SSP->wsTask[*itSkills-_SSP->nbAct]-_SSP->pss[e][p]; 
					if(startT<0) startT=0;
					int endT=_SSP->wsTask[*itSkills-_SSP->nbAct]+_SSP->wlTask[*itSkills-_SSP->nbAct]-_SSP->pss[e][p];
					if(endT<0) endT=0;
					gram[p] << "# A = A_T" << *itSkills-_SSP->nbAct << "_2A_T" << *itSkills-_SSP->nbAct << "_3 [_segMinLength, _segMaxLength]" << endl;
					gram[p] << "0.0 16 16 2 "<< _symbols[e]["A"] << " " << _symbols[e][symbolB]  << " " << _symbols[e][symbolC] << " " << startT << " " << endT-startT << endl;	
				}
			}
			
			set<int>::iterator itSkillsp = _SSP->emplSkills[e].begin();
			while(itSkillsp != _SSP->emplSkills[e].end()) {
				if(*itSkillsp<0){itSkillsp++; continue;}
				if(*itSkills != *itSkillsp){
					if(*itSkillsp<_SSP->nbAct){
						symbolA.clear(); symbolA += "A_n_"; asA.str(""); asA << *itSkillsp; symbolA += asA.str();
						symbolB.clear(); symbolB += "A_T"; asB.str(""); asB << *itSkills-_SSP->nbAct; symbolB += asB.str();
						symbolC.clear(); symbolC += "A_n_T"; asC.str(""); asC << *itSkills-_SSP->nbAct; symbolC += asC.str();
						for (int p=0; p<_SSP->nbPieces[e]; ++p){
							gram[p] << "# A_n_" << *itSkillsp << " = A_T"<< *itSkills-_SSP->nbAct << "A_n_T" << *itSkills-_SSP->nbAct << " | A_T" << *itSkills-_SSP->nbAct << "_2A_T" << *itSkills-_SSP->nbAct << "_3" << endl;
							gram[p] << "15.0 0 9999 2 "<< _symbols[e][symbolA] << " " << _symbols[e][symbolB]  << " " << _symbols[e][symbolC] << " " << 0 << " " << _SSP->psl[e][p] << endl;
						}
						symbolB.clear(); symbolB += "A_T"; asB.str(""); asB << *itSkills-_SSP->nbAct; symbolB += asB.str(); symbolB += "_2";
						symbolC.clear(); symbolC += "A_T"; asC.str(""); asC << *itSkills-_SSP->nbAct; symbolC += asC.str(); symbolC += "_3";
						for (int p=0; p<_SSP->nbPieces[e]; ++p){
							
							int startT=_SSP->wsTask[*itSkills-_SSP->nbAct]-_SSP->pss[e][p]; 
							if(startT<0) startT=0;
							int endT=_SSP->wsTask[*itSkills-_SSP->nbAct]+_SSP->wlTask[*itSkills-_SSP->nbAct]-_SSP->pss[e][p];
							if(endT<0) endT=0;
							gram[p] << "0.0 " <<  _SSP->lTask[*itSkills-_SSP->nbAct] << " " <<  _SSP->lTask[*itSkills-_SSP->nbAct] << " 2 "<< _symbols[e][symbolA] << " " << _symbols[e][symbolB]  << " " << _symbols[e][symbolC] << " " << startT << " " << endT-startT << endl;
							
						}
					}
					else{
						symbolA.clear(); symbolA += "A_n_T"; asA.str(""); asA << *itSkillsp-_SSP->nbAct; symbolA += asA.str();
						symbolB.clear(); symbolB += "A_T"; asB.str(""); asB << *itSkills-_SSP->nbAct; symbolB += asB.str();
						symbolC.clear(); symbolC += "A_n_T"; asC.str(""); asC << *itSkills-_SSP->nbAct; symbolC += asC.str();
						for (int p=0; p<_SSP->nbPieces[e]; ++p){
							gram[p] << "# A_n_T" << *itSkillsp-_SSP->nbAct << " = A_T"<< *itSkills-_SSP->nbAct << "A_n_T" << *itSkills-_SSP->nbAct << " | A_T" << *itSkills-_SSP->nbAct << "_2A_T" << *itSkills-_SSP->nbAct << "_3" << endl;
							gram[p] << "15.0 0 9999 2 "<< _symbols[e][symbolA] << " " << _symbols[e][symbolB]  << " " << _symbols[e][symbolC] << " " << 0 << " " << _SSP->psl[e][p] << endl;
						}
						symbolB.clear(); symbolB += "A_T"; asB.str(""); asB << *itSkills-_SSP->nbAct; symbolB += asB.str(); symbolB += "_2";
						symbolC.clear(); symbolC += "A_T"; asC.str(""); asC << *itSkills-_SSP->nbAct; symbolC += asC.str(); symbolC += "_3";
						for (int p=0; p<_SSP->nbPieces[e]; ++p){
							int startT=_SSP->wsTask[*itSkills-_SSP->nbAct]-_SSP->pss[e][p]; 
							if(startT<0) startT=0;
							int endT=_SSP->wsTask[*itSkills-_SSP->nbAct]+_SSP->wlTask[*itSkills-_SSP->nbAct]-_SSP->pss[e][p];
							if(endT<0) endT=0;
							gram[p] << "0.0 " << _SSP->lTask[*itSkills-_SSP->nbAct] << " " << _SSP->lTask[*itSkills-_SSP->nbAct] << " 2 "<< _symbols[e][symbolA] << " " << _symbols[e][symbolB]  << " " << _symbols[e][symbolC] << " " << startT << " " << endT-startT << endl;
						}
					}
				}
				itSkillsp++;
			}
			
			symbolA.clear(); symbolA += "A_T"; asA.str(""); asA << *itSkills-_SSP->nbAct; symbolA += asA.str();
			symbolB.clear(); symbolB += "A_T"; asB.str(""); asB << *itSkills-_SSP->nbAct; symbolB += asB.str(); symbolB += "_2";
			symbolC.clear(); symbolC += "A_T"; asC.str(""); asC << *itSkills-_SSP->nbAct; symbolC += asC.str(); symbolC += "_3";
			
			for (int p=0; p<_SSP->nbPieces[e]; ++p){
				int startT=_SSP->wsTask[*itSkills-_SSP->nbAct]-_SSP->pss[e][p]; 
				if(startT<0) startT=0;
				int endT=_SSP->wsTask[*itSkills-_SSP->nbAct]+_SSP->wlTask[*itSkills-_SSP->nbAct]-_SSP->pss[e][p];
				if(endT<0) endT=0;
				gram[p] << "# A_T" << *itSkills-_SSP->nbAct << " = A_T" << *itSkills-_SSP->nbAct << "_2A_T" << *itSkills-_SSP->nbAct << "_3 [_minAct,_maxAct]"<<endl;  
				gram[p] << "0.0 " << _SSP->lTask[*itSkills-_SSP->nbAct] << " " << _SSP->lTask[*itSkills-_SSP->nbAct] << " 2 "<< _symbols[e][symbolA] << " " << _symbols[e][symbolB]  << " " << _symbols[e][symbolC] << " " << startT << " " << endT-startT << endl;
			}
			
			symbolA.clear(); symbolA += "A_T"; asA.str(""); asA << *itSkills-_SSP->nbAct; symbolA += asA.str(); symbolA += "_2";
			symbolB.clear(); symbolB += "t_"; asB.str(""); asB << *itSkills-_SSP->nbAct; symbolB += asB.str();
			symbolC.clear(); symbolC += "A_T"; asC.str(""); asC << *itSkills-_SSP->nbAct; symbolC += asC.str(); symbolC += "_3";
			symbol.clear(); symbol += "st_"; as.str(""); as << *itSkills-_SSP->nbAct; symbol += as.str();
			for (int p=0; p<_SSP->nbPieces[e]; ++p){
				int startT=_SSP->wsTask[*itSkills-_SSP->nbAct]-_SSP->pss[e][p]; 
				if(startT<0) startT=0;
				int endT=_SSP->wsTask[*itSkills-_SSP->nbAct]+_SSP->wlTask[*itSkills-_SSP->nbAct]-_SSP->pss[e][p];
				if(endT<0) endT=0;
				gram[p] << "# A_T" << *itSkills-_SSP->nbAct << "_2 = A_T" << *itSkills-_SSP->nbAct << "_2A_T" << *itSkills-_SSP->nbAct << "_3 | st_" << *itSkills-_SSP->nbAct << endl;
				gram[p] << "0.0 0 "<<_SSP->lTask[*itSkills-_SSP->nbAct]<<" 2 "<< _symbols[e][symbolA] << " " << _symbols[e][symbolA]  << " " << _symbols[e][symbolC] << " " << startT << " " << endT-startT << endl;
				gram[p] << "0.0 1 1 1 "<< _symbols[e][symbolA] << " " << _symbols[e][symbol] << " " << startT << " " << endT-startT << endl;
				
				gram[p] << "# A_T" << *itSkills-_SSP->nbAct << "_3 = t_" << *itSkills-_SSP->nbAct << endl;
				gram[p] << "0.0 1 1 1 "<< _symbols[e][symbolC] << " " << _symbols[e][symbolB] << " " << startT << " " << endT-startT<< endl;
				
				gram[p] << endl;
				//gram[p] << "# A_T" << *itSkills-_SSP->nbAct << "_2 = A_T" << *itSkills-_SSP->nbAct << "_2A_T" << *itSkills-_SSP->nbAct << "_3 | st_" << *itSkills-_SSP->nbAct << endl;
				//				gram[p] << "0.0 0 9999 2 "<< _symbols[e][symbolA] << " " << _symbols[e][symbolA]  << " " << _symbols[e][symbolC] << " " << 0 << " " << _SSP->psl[e][p] << endl;
				//				gram[p] << "0.0 0 9999 1 "<< _symbols[e][symbolA] << " " << _symbols[e][symbol] << " 0 " << _SSP->psl[e][p] << endl;
				//				
				//				gram[p] << "# A_T" << *itSkills-_SSP->nbAct << "_3 = t_" << *itSkills-_SSP->nbAct << endl;
				//				gram[p] << "0.0 0 9999 1 "<< _symbols[e][symbolC] << " " << _symbols[e][symbolB] << " 0 " << _SSP->psl[e][p] << endl;
				//				gram[p] << endl;
			}
		}
		
		
		itSkills++;
		
	}
	
	
	
	for (int p=0; p<_SSP->nbPieces[e]; ++p)
		gram[p].close();
	
	
}


// From grammar e, p, initialize grammar graph
void SubP::initializeGrammarGraph(int e){
	
	_andNodes[e] = new AndNode**[_SSP->nbPieces[e]];
	_gr[e] = new GrammarGraph*[_SSP->nbPieces[e]];	
	for (int p=0; p<_SSP->nbPieces[e]; ++p) {
		
		ifstream grammarStream(_grammarFile[p].c_str(), ios::in); 
		if(grammarStream == NULL){cout << "Incorrect shift file path: " << _grammarFile[p] << endl; 
			exit(1);
		}
		
		GrammarWTW* g = readGrammar(grammarStream); 
		_gr[e][p] = constructCYKGraph(_SSP->psl[e][p], *g, NULL); 
		
		grammarStream.close();
		remove(_grammarFile[p].c_str());
		
		//initialize andNode structure
		_andNodes[e][p] = new AndNode*[_gr[e][p]->nbNodes - _gr[e][p]->nbOrNodes];
		
		for(int id = 0; id < _gr[e][p]->nbNodes - _gr[e][p]->nbOrNodes; id++){
			_andNodes[e][p][id] = NULL;
		}	
		
		for (int i = 0; i < _gr[e][p]->nbOrNodes; i++) { //parcours des orNodes
			OrNode* n = _gr[e][p]->postOrderNodes[i];
			assert(n->id == i); 
			if (n->nbChildren == 0) { 	
				for (int j = 0; j < n->nbAncestors; j++){
					if(_andNodes[e][p][n->ancestors[j]->id - _gr[e][p]->nbOrNodes] == NULL){
						AndNode* newAndNodeL = n->ancestors[j]; 
						_andNodes[e][p][n->ancestors[j]->id - _gr[e][p]->nbOrNodes] = newAndNodeL;
					}
				}			
			}
			else 
			{ 	// n est un noeud dans l'arbre
				
				// pas la racine
				if (n->nbAncestors > 0)	{
					// somme des parents 
					for (int j = 0; j < n->nbAncestors; j++){
						if(_andNodes[e][p][n->ancestors[j]->id - _gr[e][p]->nbOrNodes] == NULL){
							AndNode* newAndNode = n->ancestors[j]; 
							_andNodes[e][p][n->ancestors[j]->id - _gr[e][p]->nbOrNodes] = newAndNode;
						}
					}
				}
			}
		}
	}
	
}

bool SubP::mapSymbol(string symbol, int e){
	int n=(int)_symbols[e].size();
	if(_symbols[e].find(symbol)==_symbols[e].end()){
		_symbols[e][symbol] = n;
		return true;
	}
	return false;
	
}

//crée les noeuds terminaux liés aux activités
void SubP::createTerminalToAct(int e){
	
	string symbol;
	_terminalToAct[e] = new map<int,int>[_SSP->nbPieces[e]];
	for (int p=0; p<_SSP->nbPieces[e]; ++p) {
		for(int idO = 0; idO < _gr[e][p]->nbOrNodes; idO++){ 
			OrNode* n = _gr[e][p]->postOrderNodes[idO]; 
			assert(n->id == idO);
			
			if(!(n->nbChildren > 0)){ 
				set<int>:: iterator itSkills = _SSP->emplSkills[e].begin();
				while(itSkills != _SSP->emplSkills[e].end()){
					int offset=0;
					if(*itSkills<0){
						symbol.clear();	symbol += "b"; 
					}
					else if(*itSkills<_SSP->nbAct){
						symbol.clear();	symbol += "a_"; ostringstream as; as << *itSkills; symbol += as.str();
					}
					else{
						//symbol.clear();	symbol += "t_"; ostringstream ass; ass << *itSkills-_SSP->nbAct; symbol += ass.str();
						
						//starting task
						symbol.clear();	symbol += "t_"; ostringstream ass; ass << *itSkills-_SSP->nbAct; symbol += ass.str();
						if(_symbols[e][symbol] != n->leaf.terminal){
							symbol.clear();	symbol += "st_"; ass.str("") ;ass << *itSkills-_SSP->nbAct; symbol += ass.str();
							offset=_SSP->nbTask;
						}
					}
						
					if(_symbols[e][symbol] == n->leaf.terminal){
						if(_terminalToAct[e][p].find(n->leaf.terminal) == _terminalToAct[e][p].end()){
							_terminalToAct[e][p][n->leaf.terminal] = *itSkills+offset;
							//cout << "loooooool: "<<symbol<<" "<< n->leaf.terminal<<" "<<_terminalToAct[p][n->leaf.terminal]<<endl;
						}
					}
					
					itSkills++;	
				}
				
			}
		}
	}
}

int SubP::solveSubProblemWithDP(int e, int p, double **piRC, double dualEmplWork, set<int> *cuts, int **shift, int maxNumberOfColumns){
	//cout << "**In solveSubProblemWithDP " << endl;
	
	
	double columnCost = 0.0;
	double cost;
	double min;
	int idMin;
	
	//we assume that the first or nodes are leaves	
	
	GrammarGraph **gr=_gr[e];
	
	double *costAnd=new double[gr[p]->nbNodes - gr[p]->nbOrNodes]; 	
	double *costOr=new double[gr[p]->nbOrNodes]; 	
	int *idChildOr=new int[gr[p]->nbOrNodes];
	set<int> *setidChildOr=new set<int>[gr[p]->nbOrNodes];
	
	//we assume that the first or nodes are leaves
	for(int idO = 0; idO < gr[p]->nbOrNodes; idO++){ 
		OrNode* n = gr[p]->postOrderNodes[idO]; 
		assert(n->id == idO);
		
		costOr[idO] = 0.0;
		if(!(n->nbChildren > 0)){ //leaf
			//cout << n->nbChildren<<" ";
			//Tests if belongs to a cut
			int a=_terminalToAct[e][p][n->leaf.terminal];
			if(a>=_SSP->nbAct+_SSP->nbTask) a-=_SSP->nbTask;
			if(cuts[n->leaf.position].find(a)!=cuts[n->leaf.position].end()){
				//this node is forbidden
				//if(_terminalToAct[e][p][n->leaf.terminal]>=_SSP->nbAct+_SSP->nbTask || _terminalToAct[e][p][n->leaf.terminal]<_SSP->nbAct)
					costOr[idO]=std::numeric_limits<double>::infinity();
			}
			else{
				a=_terminalToAct[e][p][n->leaf.terminal];
				if(a>=_SSP->nbAct+_SSP->nbTask)
					costOr[idO]=-piRC[a-_SSP->nbTask][n->leaf.position + _SSP->pss[e][p]];
				else if(a>=_SSP->nbAct)
					costOr[idO]=0.0;
				else if(a>=0)
					costOr[idO]=-piRC[a][n->leaf.position + _SSP->pss[e][p]];
				else costOr[idO]=0.0;
				
			}
			//			if(cuts[n->leaf.position].find(_terminalToAct[e][p][n->leaf.terminal])!=cuts[n->leaf.position].end()){
			//				costOr[idO]=std::numeric_limits<double>::infinity();
			//			}
			//			else{
			//				int a=_terminalToAct[e][p][n->leaf.terminal];
			//				costOr[idO]=-piRC[a][n->leaf.position + _SSP->pss[e][p]];
			//			}
			
			//cout << "\t"<<idO<<"->cut: ornode a="<<_terminalToAct[e][p][n->leaf.terminal]<<"("<<n->leaf.terminal<<")"<<" pos="<<n->position/*+_SSP->pss[e][p]*/<<" cost="<<costOr[idO]<<endl;
			
		}
		else{
			//cout << n->nbChildren<<" ";
			//Calculate the cost of the children to choose the min
			//if the or node is equal to true
			// at least one of its children is equal to true
			min=std::numeric_limits<double>::infinity();
			//min=1000000;
			idMin=-1;
			for (int idA = 0; idA < n->nbChildren; idA++){ 
				AndNode* nA = n->inner.children[idA];
                cost = 0.0;//nA->production->cost;
				
				
				if(nA->leftChild != NULL){
					assert(nA->leftChild->id < n->id);
					cost += costOr[nA->leftChild->id];
				}
				if(nA->rightChild != NULL){
					assert(nA->rightChild->id < n->id);
					cost += costOr[nA->rightChild->id];
				}
				
				
				costAnd[nA->id - gr[p]->nbOrNodes] = cost;
				
				if(cost < min){
					min = cost;
					idMin = nA->id - gr[p]->nbOrNodes;
					setidChildOr[n->id].clear();
				}
				else if(cost-min<=TOLERANCE) setidChildOr[n->id].insert(nA->id - gr[p]->nbOrNodes);
				
			}
			costOr[n->id] = min;
			idChildOr[n->id] = idMin;
			//if(setidChildOr[n->id].size()>0){cout<<"jiji"<<endl; getchar();}
			//cout << "\t"<<idO<<"->***: ornode a="<<_terminalToAct[e][p][n->leaf.terminal]<<"("<<n->leaf.terminal<<")"<<" pos="<<n->position/*+_SSP->pss[e][p]*/<<" cost="<<costOr[idO]<<endl;
			
		}
		
		
		
	}
	//cout << endl;
	//getchar();
	
	OrNode *root = gr[p]->root; 
	columnCost += costOr[root->id] - dualEmplWork;
	
	if (columnCost>=+TOLERANCE) {
		return 0;
	}
	
	//construct schedule and return column
	
	int k=0;
	stack<int> stackSched;
	
	double mincost=-1;
	while (mincost<+TOLERANCE && k  < maxNumberOfColumns) {
		mincost=TOLERANCE;
		int id=-1;
		for(int i = 0; i < root->nbChildren; i++){
			if(costAnd[root->inner.children[i]->id - gr[p]->nbOrNodes] - dualEmplWork < mincost){
				mincost=costAnd[root->inner.children[i]->id - gr[p]->nbOrNodes] - dualEmplWork;
				id=i;
			}
		}
		
		if (mincost<+TOLERANCE) {
			//cout <<"("<<k<<")"<< root->inner.children[id]->id<<" ";
			
			assert(stackSched.empty());
			stackSched.push(root->inner.children[id]->id - gr[p]->nbOrNodes);
			
			
			//double sost
			int lastchoiceId=-1;
			while(!stackSched.empty()){
				int currentAndId = stackSched.top(); 
				stackSched.pop();
				AndNode* currentAndNode = _andNodes[e][p][currentAndId]; 
				assert(currentAndNode != NULL);
				
				if(currentAndNode->leftChild->nbChildren > 0){ //not a leaf
					
					assert(currentAndNode->rightChild != NULL);
					
					stackSched.push(idChildOr[currentAndNode->rightChild->id]);
					if(setidChildOr[currentAndNode->rightChild->id].size()>0) lastchoiceId=currentAndNode->rightChild->id;
					
					stackSched.push(idChildOr[currentAndNode->leftChild->id]);
					if(setidChildOr[currentAndNode->leftChild->id].size()>0) lastchoiceId=currentAndNode->leftChild->id;
					
				}
				else{
					OrNode* n = currentAndNode->leftChild;
					if(_terminalToAct[e][p].find(n->leaf.terminal) != _terminalToAct[e][p].end()){
						//shift[k][n->leaf.position] = _terminalToAct[e][p][n->leaf.terminal];
						int a=_terminalToAct[e][p][n->leaf.terminal];
						if(a>=_SSP->nbAct+_SSP->nbTask) a-=_SSP->nbTask;
						shift[k][n->leaf.position] = a;
					}
					
				}
				
			}
//			if(lastchoiceId>=0){
//				//cout << "jiji"<<endl;
//				set<int>::iterator it=setidChildOr[lastchoiceId].begin();
//				idChildOr[lastchoiceId]=*it;
//				setidChildOr[lastchoiceId].erase(it);
//			}
//			else 
				costAnd[root->inner.children[id]->id - gr[p]->nbOrNodes]=dualEmplWork;
			
			//			double costalgo=mincost;
			//			double costT=0.0; 
			//			double cost=0.0;
			//			for (int t=0; t<_SSP->psl[e][p]; ++t) if(shift[k][t]>=0){
			//				if(t>0 && shift[k][t]!=shift[k][t-1] && shift[k][t-1]>=0) costT+=_SSP->costTransit;
			//				if(shift[k][t]<_SSP->nbAct)
			//					cost-=piRC[shift[k][t]][t+_SSP->pss[e][p]];
			//				else{
			//					if(t==0 || (shift[k][t-1]!=shift[k][t])) 
			//						cost-=piRC[shift[k][t]][t+_SSP->pss[e][p]];
			//				}
			//				assert(shift[k][t]<_SSP->nbTask+_SSP->nbAct && shift[k][t]>=0);
			//			}
			//			cost+=costT;
			//			//cout << "("<<cost<<" - "<< costT<<" - "<<dualEmplWork<<") ";
			//			cost-=dualEmplWork;
			//			double ab=(costalgo-cost>=0)?costalgo-cost:cost-costalgo;
			//			if(ab>TOLERANCE){
			//				cout<<e<<" "<<p<<" ("<<costalgo<<"-"<<cost<<"->"<<costT<<"): ";
			//				for (int t=0; t<_SSP->psl[e][p]; ++t){
			//					int a=shift[k][t];
			//					if (a<_SSP->nbAct) cout<<a;
			//					else cout<<"T"<<a-_SSP->nbAct;
			//					cout << ".";
			//				}
			//				cout << endl;
			//				getchar();
			//			}
			
			++k;
		}
	}
	//cout << endl;
	
	return k;
	
	
	
	
	
}


int SubP::solveSubProblemWithDP(int e, int p, double **piRC, double dualEmplWork, int **shift, int maxNumberOfColumns){
    //cout << "**In solveSubProblemWithDP " << endl;
    
    
    double columnCost = 0.0;
    double cost;
    double min;
    int idMin;
    
    //we assume that the first or nodes are leaves
    
    GrammarGraph **gr=_gr[e];
    
    double *costAnd=new double[gr[p]->nbNodes - gr[p]->nbOrNodes];
    double *costOr=new double[gr[p]->nbOrNodes];
    int *idChildOr=new int[gr[p]->nbOrNodes];
    
    //we assume that the first or nodes are leaves
    for(int idO = 0; idO < gr[p]->nbOrNodes; idO++){
        OrNode* n = gr[p]->postOrderNodes[idO];
        assert(n->id == idO);
        
        costOr[idO] = 0.0;
        if(!(n->nbChildren > 0)){ //leaf
            //cout << n->nbChildren<<" ";
            //Tests if belongs to a cut
            int a=_terminalToAct[e][p][n->leaf.terminal];
            
            if(a>=_SSP->nbAct+_SSP->nbTask)
                costOr[idO]=-piRC[a-_SSP->nbTask][n->leaf.position + _SSP->pss[e][p]];
            else if(a>=_SSP->nbAct)
                costOr[idO]=0.0;
            else if(a>=0)
                costOr[idO]=-piRC[a][n->leaf.position + _SSP->pss[e][p]];
            else costOr[idO]=0.0;
            
        }
        else{
            //cout << n->nbChildren<<" ";
            //Calculate the cost of the children to choose the min
            //if the or node is equal to true
            // at least one of its children is equal to true
            min=std::numeric_limits<double>::infinity();
            idMin=-1;
            for (int idA = 0; idA < n->nbChildren; idA++){
                AndNode* nA = n->inner.children[idA];
                cost = 0.0;//nA->production->cost;
                
                
                if(nA->leftChild != NULL){
                    assert(nA->leftChild->id < n->id);
                    cost += costOr[nA->leftChild->id];
                }
                if(nA->rightChild != NULL){
                    assert(nA->rightChild->id < n->id);
                    cost += costOr[nA->rightChild->id];
                }
                
                
                costAnd[nA->id - gr[p]->nbOrNodes] = cost;
                
                if(cost < min){
                    min = cost;
                    idMin = nA->id - gr[p]->nbOrNodes;
                }
                else if (cost-min<0.01){
                    double r=(rand()%1000)/1000.0;
                    double prob=0.5;
                    if(r<prob){
                        min = cost;
                        idMin = nA->id - gr[p]->nbOrNodes;
                    }
                }
                
            }
            costOr[n->id] = min;
            idChildOr[n->id] = idMin;
            
            
        }
        
        
        
    }
    
    OrNode *root = gr[p]->root;
    columnCost += costOr[root->id] - dualEmplWork;
    
//    if (columnCost>=-TOLERANCE) {
//        return 0;
//    }
    
    //construct schedule and return column
    set<int> forbiddenmove;
    int k=0;
    stack<int> stackSched;
    
    double mincost=-1;
    while (forbiddenmove.size()<root->nbChildren && k  < maxNumberOfColumns) {
        mincost=std::numeric_limits<double>::infinity();
        int id=-1;
        for(int i = 0; i < root->nbChildren; i++){
            if(forbiddenmove.find(i)!=forbiddenmove.end()) continue;
            if(costAnd[root->inner.children[i]->id - gr[p]->nbOrNodes] - dualEmplWork < mincost){
                mincost=costAnd[root->inner.children[i]->id - gr[p]->nbOrNodes] - dualEmplWork;
                id=i;
            }
        }
        
        forbiddenmove.insert(id);
        assert(id>=0);
        
        if (/*mincost<=TOLERANCE*/true) {
            
            assert(stackSched.empty());
            stackSched.push(root->inner.children[id]->id - gr[p]->nbOrNodes);
            
            while(!stackSched.empty()){
                int currentAndId = stackSched.top();
                stackSched.pop();
                AndNode* currentAndNode = _andNodes[e][p][currentAndId];
                assert(currentAndNode != NULL);
                
                if(currentAndNode->leftChild->nbChildren > 0){ //not a leaf
                    
                    assert(currentAndNode->rightChild != NULL);
                    
                    stackSched.push(idChildOr[currentAndNode->rightChild->id]);
                    
                    stackSched.push(idChildOr[currentAndNode->leftChild->id]);
                    
                }
                else{
                    OrNode* n = currentAndNode->leftChild;
                    if(_terminalToAct[e][p].find(n->leaf.terminal) != _terminalToAct[e][p].end()){
                        //shift[k][n->leaf.position] = _terminalToAct[e][p][n->leaf.terminal];
                        int a=_terminalToAct[e][p][n->leaf.terminal];
                        if(a>=_SSP->nbAct+_SSP->nbTask) a-=_SSP->nbTask;
                        shift[k][n->leaf.position] = a;
                    }
                    
                }
                
            }
          
            //costAnd[root->inner.children[id]->id - gr[p]->nbOrNodes]=50000000;//10*dualEmplWork;
            
            
            ++k;
        }
    }

    delete[] costAnd;
    delete[] costOr;
    delete[] idChildOr;
    return k;
    
    
    
    
    
}

int SubP::solveSubProblemWithDPWorst(int e, int p, double **piRC, double dualEmplWork, int **shift, int maxNumberOfColumns){
    //cout << "**In solveSubProblemWithDP " << endl;
    
    
    double columnCost = 0.0;
    double cost;
    double min;
    int idMin;
    
    //we assume that the first or nodes are leaves
    
    GrammarGraph **gr=_gr[e];
    
    double *costAnd=new double[gr[p]->nbNodes - gr[p]->nbOrNodes];
    double *costOr=new double[gr[p]->nbOrNodes];
    int *idChildOr=new int[gr[p]->nbOrNodes];
    
    //we assume that the first or nodes are leaves
    for(int idO = 0; idO < gr[p]->nbOrNodes; idO++){
        OrNode* n = gr[p]->postOrderNodes[idO];
        assert(n->id == idO);
        
        costOr[idO] = 0.0;
        if(!(n->nbChildren > 0)){ //leaf
            //cout << n->nbChildren<<" ";
            //Tests if belongs to a cut
            int a=_terminalToAct[e][p][n->leaf.terminal];
            
            if(a>=_SSP->nbAct+_SSP->nbTask)
                costOr[idO]=piRC[a-_SSP->nbTask][n->leaf.position + _SSP->pss[e][p]];
            else if(a>=_SSP->nbAct)
                costOr[idO]=0.0;
            else if(a>=0)
                costOr[idO]=piRC[a][n->leaf.position + _SSP->pss[e][p]];
            else costOr[idO]=0.0;
            
        }
        else{
            //cout << n->nbChildren<<" ";
            //Calculate the cost of the children to choose the min
            //if the or node is equal to true
            // at least one of its children is equal to true
            min=std::numeric_limits<double>::infinity();
            idMin=-1;
            for (int idA = 0; idA < n->nbChildren; idA++){
                AndNode* nA = n->inner.children[idA];
                cost = -nA->production->cost;
                
                
                if(nA->leftChild != NULL){
                    assert(nA->leftChild->id < n->id);
                    cost += costOr[nA->leftChild->id];
                }
                if(nA->rightChild != NULL){
                    assert(nA->rightChild->id < n->id);
                    cost += costOr[nA->rightChild->id];
                }
                
                
                costAnd[nA->id - gr[p]->nbOrNodes] = cost;
                
                if(cost < min){
                    min = cost;
                    idMin = nA->id - gr[p]->nbOrNodes;
                }
                else if (cost-min<0.01){
                    double r=(rand()%1000)/1000.0;
                    double prob=0.5;
                    if(r<prob){
                        min = cost;
                        idMin = nA->id - gr[p]->nbOrNodes;
                    }
                }
                
            }
            costOr[n->id] = min;
            idChildOr[n->id] = idMin;
            
            
        }
        
        
        
    }
    
    OrNode *root = gr[p]->root;
    columnCost += costOr[root->id] - dualEmplWork;
    
    //    if (columnCost>=-TOLERANCE) {
    //        return 0;
    //    }
    
    //construct schedule and return column
    set<int> forbiddenmove;
    int k=0;
    stack<int> stackSched;
    
    double mincost=-1;
    while (forbiddenmove.size()<root->nbChildren && k  < maxNumberOfColumns) {
        mincost=std::numeric_limits<double>::infinity();
        int id=-1;
        for(int i = 0; i < root->nbChildren; i++){
            if(forbiddenmove.find(i)!=forbiddenmove.end()) continue;
            if(costAnd[root->inner.children[i]->id - gr[p]->nbOrNodes] - dualEmplWork < mincost){
                mincost=costAnd[root->inner.children[i]->id - gr[p]->nbOrNodes] - dualEmplWork;
                id=i;
            }
        }
        
        forbiddenmove.insert(id);
        assert(id>=0);
        
        if (/*mincost<=TOLERANCE*/true) {
            
            assert(stackSched.empty());
            stackSched.push(root->inner.children[id]->id - gr[p]->nbOrNodes);
            
            while(!stackSched.empty()){
                int currentAndId = stackSched.top();
                stackSched.pop();
                AndNode* currentAndNode = _andNodes[e][p][currentAndId];
                assert(currentAndNode != NULL);
                
                if(currentAndNode->leftChild->nbChildren > 0){ //not a leaf
                    
                    assert(currentAndNode->rightChild != NULL);
                    
                    stackSched.push(idChildOr[currentAndNode->rightChild->id]);
                    
                    stackSched.push(idChildOr[currentAndNode->leftChild->id]);
                    
                }
                else{
                    OrNode* n = currentAndNode->leftChild;
                    if(_terminalToAct[e][p].find(n->leaf.terminal) != _terminalToAct[e][p].end()){
                        //shift[k][n->leaf.position] = _terminalToAct[e][p][n->leaf.terminal];
                        int a=_terminalToAct[e][p][n->leaf.terminal];
                        if(a>=_SSP->nbAct+_SSP->nbTask) a-=_SSP->nbTask;
                        shift[k][n->leaf.position] = a;
                    }
                    
                }
                
            }
            
            //costAnd[root->inner.children[id]->id - gr[p]->nbOrNodes]=50000000;//10*dualEmplWork;
            
            
            ++k;
        }
    }
    
    delete[] costAnd;
    delete[] costOr;
    delete[] idChildOr;
    return k;
    
    
    
    
    
}

int SubP::solveSubProblemWithDPRand(int e, int p, int **shift, int maxNumberOfColumns){
    //cout << "**In solveSubProblemWithDP " << endl;
    
    
    double columnCost = 0.0;
    double cost;
    double min;
    int idMin;
    
    //we assume that the first or nodes are leaves
    
    GrammarGraph **gr=_gr[e];
    
    double *costAnd=new double[gr[p]->nbNodes - gr[p]->nbOrNodes];
    double *costOr=new double[gr[p]->nbOrNodes];
    int *idChildOr=new int[gr[p]->nbOrNodes];
    
    //we assume that the first or nodes are leaves
    for(int idO = 0; idO < gr[p]->nbOrNodes; idO++){
        OrNode* n = gr[p]->postOrderNodes[idO];
        assert(n->id == idO);
        
        costOr[idO] = 0.0;
        if(!(n->nbChildren > 0)){ //leaf
            
        }
        else{
            //cout << n->nbChildren<<" ";
            //Calculate the cost of the children to choose the min
            //if the or node is equal to true
            // at least one of its children is equal to true
            min=std::numeric_limits<double>::infinity();
            idMin=-1;
            for (int idA = 0; idA < n->nbChildren; idA++){
                AndNode* nA = n->inner.children[idA];
                cost = 0.0;
                
                
                if(nA->leftChild != NULL){
                    assert(nA->leftChild->id < n->id);
                    cost += costOr[nA->leftChild->id];
                }
                if(nA->rightChild != NULL){
                    assert(nA->rightChild->id < n->id);
                    cost += costOr[nA->rightChild->id];
                }
                
                
                costAnd[nA->id - gr[p]->nbOrNodes] = cost;
                
                if(cost < min){
                    min = cost;
                    idMin = nA->id - gr[p]->nbOrNodes;
                }
                else if (cost-min<0.01){
                    double r=(rand()%1000)/1000.0;
                    double prob=0.5;
                    if(r<prob){
                        min = cost;
                        idMin = nA->id - gr[p]->nbOrNodes;
                    }
                }
                
            }
            costOr[n->id] = min;
            idChildOr[n->id] = idMin;
            
            
        }
        
        
        
    }
    
    OrNode *root = gr[p]->root;
    columnCost += costOr[root->id] ;
    
    //    if (columnCost>=-TOLERANCE) {
    //        return 0;
    //    }
    
    //construct schedule and return column
    set<int> forbiddenmove;
    int k=0;
    stack<int> stackSched;
    
    double mincost=-1;
    while (forbiddenmove.size()<root->nbChildren && k  < maxNumberOfColumns) {
        mincost=std::numeric_limits<double>::infinity();
        int id=-1;
        
        do{
            id=rand()%root->nbChildren;
        }while(forbiddenmove.find(id)!=forbiddenmove.end());
        
        
        forbiddenmove.insert(id);
        assert(id>=0);
        
        if (/*mincost<=TOLERANCE*/true) {
            
            assert(stackSched.empty());
            stackSched.push(root->inner.children[id]->id - gr[p]->nbOrNodes);
            
            while(!stackSched.empty()){
                int currentAndId = stackSched.top();
                stackSched.pop();
                AndNode* currentAndNode = _andNodes[e][p][currentAndId];
                assert(currentAndNode != NULL);
                
                if(currentAndNode->leftChild->nbChildren > 0){ //not a leaf
                    
                    assert(currentAndNode->rightChild != NULL);
                    
                    stackSched.push(idChildOr[currentAndNode->rightChild->id]);
                    
                    stackSched.push(idChildOr[currentAndNode->leftChild->id]);
                    
                }
                else{
                    OrNode* n = currentAndNode->leftChild;
                    if(_terminalToAct[e][p].find(n->leaf.terminal) != _terminalToAct[e][p].end()){
                        //shift[k][n->leaf.position] = _terminalToAct[e][p][n->leaf.terminal];
                        int a=_terminalToAct[e][p][n->leaf.terminal];
                        if(a>=_SSP->nbAct+_SSP->nbTask) a-=_SSP->nbTask;
                        shift[k][n->leaf.position] = a;
                    }
                    
                }
                
            }
            
            //costAnd[root->inner.children[id]->id - gr[p]->nbOrNodes]=50000000;//10*dualEmplWork;
            
            
            ++k;
        }
    }
    
    delete[] costAnd;
    delete[] costOr;
    delete[] idChildOr;
    return k;
    
    
    
    
    
}

//Solve subproblem, one columns and without cuts maximizing the assigned preferences

double SubP::solveSubProblemWithDP(int e, int p, double **piRC, bool costtransit, int *shift){
	//cout << "**In solveSubProblemWithDP " << endl;
	
	

	
	//we assume that the first or nodes are leaves
	
	GrammarGraph **gr=_gr[e];
	
	double *costAnd=new double[gr[p]->nbNodes - gr[p]->nbOrNodes];
	double *costOr=new double[gr[p]->nbOrNodes];
	int *idChildOr=new int[gr[p]->nbOrNodes];
	
	//we assume that the first or nodes are leaves
	for(int idO = 0; idO < gr[p]->nbOrNodes; idO++){
		OrNode* n = gr[p]->postOrderNodes[idO];
		assert(n->id == idO);
		
		costOr[idO] = 0.0;
		if(!(n->nbChildren > 0)){ //leaf
            if(piRC==NULL) continue;
			int a=_terminalToAct[e][p][n->leaf.terminal];
            if(a>=_SSP->nbAct+_SSP->nbTask)
                costOr[idO]=0.0;//-piRC[a-_SSP->nbTask][n->leaf.position + _SSP->pss[e][p]];
            else if(a>=_SSP->nbAct)
                costOr[idO]=0.0;
            else if(a>=0)
                costOr[idO]=piRC[a][n->leaf.position + _SSP->pss[e][p]];
            else costOr[idO]=0.0;
		}
		else{
			//cout << n->nbChildren<<" ";
			//Calculate the cost of the children to choose the min
			//if the or node is equal to true
			// at least one of its children is equal to true
			double min=std::numeric_limits<double>::infinity();
			int idMin=-1;
			for (int idA = 0; idA < n->nbChildren; idA++){
				AndNode* nA = n->inner.children[idA];
                double cost =0.0;
                if(costtransit) cost+=nA->production->cost;
				
				
				if(nA->leftChild != NULL){
					//assert(nA->leftChild->id < n->id);
					cost += costOr[nA->leftChild->id];
				}
				if(nA->rightChild != NULL){
					//assert(nA->rightChild->id < n->id);
					cost += costOr[nA->rightChild->id];
				}
				
				
				costAnd[nA->id - gr[p]->nbOrNodes] = cost;
				
				if(cost < min){
					min = cost;
					idMin = nA->id - gr[p]->nbOrNodes;
				}
                else if (cost-min<0.01){
                    double r=(rand()%1000)/1000.0;
                    if(r<0.5){
                        min = cost;
                        idMin = nA->id - gr[p]->nbOrNodes;
                    }
                }
				
			}

			costOr[n->id] = min;
			idChildOr[n->id] = idMin;
			
		}
		
		
		
	}
		
	OrNode *root = gr[p]->root;
	
	stack<int> stackSched;
	
	stackSched.push(idChildOr[root->id]);
    
    
    while(!stackSched.empty()){
        int currentAndId = stackSched.top();
        stackSched.pop();
        AndNode* currentAndNode = _andNodes[e][p][currentAndId];
        assert(currentAndNode != NULL);
        
        if(currentAndNode->leftChild->nbChildren > 0){ //not a leaf
            
            assert(currentAndNode->rightChild != NULL);
            
            stackSched.push(idChildOr[currentAndNode->rightChild->id]);
            
            stackSched.push(idChildOr[currentAndNode->leftChild->id]);
            
        }
        else{
            OrNode* n = currentAndNode->leftChild;
            //if(_terminalToAct[e][p].find(n->leaf.terminal) != _terminalToAct[e][p].end()){
                int a=_terminalToAct[e][p][n->leaf.terminal];
                if(a>=_SSP->nbAct+_SSP->nbTask) a-=_SSP->nbTask;
                shift[n->leaf.position] = a;
            //}
            
        }
        
    }
    double costtoreturn=costOr[root->id];
    
    delete[] costAnd;
	delete[] costOr;
	delete[] idChildOr;
    gr=NULL;
	
	return costtoreturn;
}


//solve subproblem, one columns and without cuts considering the demmand of each activity and the under and overcovering costs
int SubP::solveSubProblemWithDP(int e, int p, int **piRC, double dualEmplWork, int *shift, int **ucost,int **ocost){
	//cout << "**In solveSubProblemWithDP " << endl;
	
	
	double columnCost = 0.0;
	double cost;
	double min;
	int idMin;
	
	GrammarGraph **gr=_gr[e];
	

	double *costAnd=new double[gr[p]->nbNodes - gr[p]->nbOrNodes]; 	
	double *costOr=new double[gr[p]->nbOrNodes]; 	
	int *idChildOr=new int[gr[p]->nbOrNodes];
	
	
	//we assume that the first or nodes are leaves
	for(int idO = 0; idO < gr[p]->nbOrNodes; idO++){ 
		OrNode* n = gr[p]->postOrderNodes[idO]; 
		assert(n->id == idO);
		
		costOr[idO] = 0;
		if(!(n->nbChildren > 0)){ //leaf
			
			int a=_terminalToAct[e][p][n->leaf.terminal];
			if(a>=_SSP->nbAct+_SSP->nbTask){
				costOr[idO]=-piRC[n->leaf.position + _SSP->pss[e][p]][a-_SSP->nbTask];
			}else if(a>=_SSP->nbAct)
				costOr[idO]=0.0;
			else if(a>=0){
				if(piRC[n->leaf.position+_SSP->pss[e][p]][a]>0){
					costOr[idO]=-piRC[n->leaf.position + _SSP->pss[e][p]][a]*ucost[n->leaf.position + _SSP->pss[e][p]][a];
				}else{
					costOr[idO]=-piRC[n->leaf.position + _SSP->pss[e][p]][a]*ocost[n->leaf.position + _SSP->pss[e][p]][a];
				}
			}else costOr[idO]=0.0;
			
		}
		else{
			//Calculate the cost of the children to choose the min
			//if the or node is equal to true
			// at least one of its children is equal to true
			bool first = true;
			min = 100000;
            idMin=-1;
			for (int idA = 0; idA < n->nbChildren; idA++){ 
				AndNode* nA = n->inner.children[idA];
				cost = nA->production->cost;
				
				if(nA->leftChild != NULL){
					assert(nA->leftChild->id < n->id);
					cost += costOr[nA->leftChild->id];
				}
				if(nA->rightChild != NULL){
					assert(nA->rightChild->id < n->id);
					cost += costOr[nA->rightChild->id];
				}
				
				
				costAnd[nA->id - gr[p]->nbOrNodes] = cost;
				if(first == true){
					min = cost;
					idMin = nA->id - gr[p]->nbOrNodes;
					first = false;
				}
				else{
					if(cost < min){
						min = cost;
						idMin = nA->id - gr[p]->nbOrNodes;
					}
				}
				
			}
			costOr[n->id] = min;
			idChildOr[n->id] = idMin;
			//cout << "\t"<<idO<<"->***: ornode a="<<_terminalToAct[e][p][n->leaf.terminal]<<"("<<n->leaf.terminal<<")"<<" pos="<<n->position/*+_SSP->pss[e][p]*/<<" cost="<<costOr[idO]<<endl;
			
		}
		
		
		
	}
	
	OrNode *root = gr[p]->root; 
	columnCost += costOr[root->id] - dualEmplWork;
	
	
	
	stack<int> stackSched;
	stackSched.push(idChildOr[root->id]);
	
	
	while(!stackSched.empty()){
		int currentAndId = stackSched.top(); 
		stackSched.pop();
		AndNode* currentAndNode = _andNodes[e][p][currentAndId]; 
		assert(currentAndNode != NULL);
		if(currentAndNode->leftChild->nbChildren > 0){ //not a leaf
			stackSched.push(idChildOr[currentAndNode->leftChild->id]);
			assert(currentAndNode->rightChild != NULL);
			int id = idChildOr[currentAndNode->rightChild->id];
			stackSched.push(id);
		}
		else{
			OrNode* n = currentAndNode->leftChild;
			if(_terminalToAct[e][p].find(n->leaf.terminal) != _terminalToAct[e][p].end()){
				//shift[0][n->leaf.position] = _terminalToAct[e][p][n->leaf.terminal];
				int a=_terminalToAct[e][p][n->leaf.terminal];
				if(a>=_SSP->nbAct+_SSP->nbTask) a-=_SSP->nbTask;
				
				shift[n->leaf.position] = a;
			}
			
		}
	}
	
	
	delete[] costAnd; 	
	delete[] costOr; 	
	delete[] idChildOr;
    gr=NULL;
	
	return 1;
}


