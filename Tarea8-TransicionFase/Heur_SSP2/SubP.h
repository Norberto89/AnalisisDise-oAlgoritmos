//
// SubP.h
//
// Created by Vincent Boyer
//
// CIRRELT, Centre interuniversitaire de recherche sur les r�seaux 
// d'entreprise, la logistique et le transport 
// Pavillon Andr�-Aisenstadt, bureau 3520
// 2920, chemin de la Tour
// Montr�al (Qu�bec)  H3T 1J4  CANADA 
// Phone: (514) 343-7575 
// E-mail: v-boyer@laposte.net
//
//
//
// Janvier 2011
//


#ifndef _SubP_
#define _SubP_

#include <fstream>
#include <cstring>
#include <vector>
#include <set>
#include <string>
#include <sstream>
#include <map>
#include <stack>


//#include	<ilconcert/iloenv.h>
//#include	<ilcplex/ilocplex.h>

#include "GrammarWTW.h"
#include "SSP.h"

#define TOLERANCE 0.001

using namespace std;



class SubP
{
private:
	//SCP
	SSP *_SSP;
	
	//Grammarbp
	string *_grammarFile;		//Name of the .txt files with all the productions fo the grammar
	//GrammarGraph*** _gr;		//Set of the grammars (per employee and per shift)
	map<string,int>* _symbols;	//Mapping of the symbols in the grammar with their index
	//AndNode**** _andNodes;		//_andNodes[e][id] give the AndNode associated to id
	//map<int,int>** _terminalToAct; //Link the id of a terminal node with its corresponding activities/tasks

	
	void initSCP(SSP *SCP);
	
	void createGrammarQUENTIN(int e);
	
	void createGrammar2(int e);
	
	void createGrammar(int e);
	
	void initializeGrammarGraph(int e);
	
	bool mapSymbol(string symbol, int e);
	
	void createTerminalToAct(int e);
	
	
public:

	GrammarGraph*** _gr;
	AndNode**** _andNodes;							//_andNodes[e][pieza][id] donne le AndNode associ� � cet id
	//	double _costAndNodes;
	
	
	//constructor & destructor
	SubP (SSP *SCP);
	~SubP();
	
	AndNode* getAndNode(int e, int p, int id){
		return _andNodes[e][p][id];
	}
	map<int,int>** _terminalToAct;
	int solveSubProblemWithDP(int e, int p, double **piRC, double dualEmplWork, set<int> *cuts, int **shift, int maxNumberOfColumns);
    
    int solveSubProblemWithDP(int e, int p, double **piRC, double dualEmplWork, int **shift, int maxNumberOfColumns);
    
    int solveSubProblemWithDPWorst(int e, int p, double **piRC, double dualEmplWork, int **shift, int maxNumberOfColumns);

    int solveSubProblemWithDPRand(int e, int p, int **shift, int maxNumberOfColumns);


	
	double solveSubProblemWithDP(int e, int p, double **piRC, bool costtransit, int *shift);

	int solveSubProblemWithDP(int e, int p, int **piRC, double dualEmplWork, int *shift, int **ucost,int **ocost);
	
};//class SubP

#endif
