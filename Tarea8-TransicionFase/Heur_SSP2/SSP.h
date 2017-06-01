//
//  SSP.h
//  Heur_SSP
//
//  Created by Vincent on 9/26/14.
//
//

#ifndef __Heur_SSP__SSP__
#define __Heur_SSP__SSP__

#include <iostream>
#include <fstream>

#include <string>
#include <set>
#include <assert.h>

using namespace std;


class SSP{
public:
	//Employees
	int seqLength;				//Sequence length
	int nbEmployees;			//Nomber of employees
	set<int>* emplSkills;		//emplActs[e] list of skills for which employee e is competent
	int * maxLengthP;			//Max length of a shift for each employee
	int ** pss;					//Starting time of each shift for each employee
	int ** psl;					//Length of each shift for each employee
	int * nbPieces;				//Number of shifts per employee
	int nbPiecesMax;
	int maxLengthPiece;
	
	//Activities
	int costTransit;
	int nbAct;					//Number of activities
	int *minAct;				//Min Length of each activities
	int *maxAct;				//Max Length of each activities
	int **dAct;					//Demand for each activities for each periods
	int **ocAct;				//Ovec covering costs
	int **ucAct;				//Undercovering costs
	int **cAct;					//Costs
	
	//Tasks
	int nbTask;					//Number of tasks
	int *dTask;					//Demand for each activities for each periods
	int *ocTask;				//Ovec covering costs
	int *ucTask;				//Undercovering costs
	int *cTask;					//Costs
	int *lTask;					//Length of a task
	int *wsTask;				//Starting window
	int *wlTask;				//Legth window
	set<int> *Precedences;		//Precedences[a] = set of tasks that should be performed before task a
    
    SSP(string temp);
    
    ~SSP();
    
    
};

#endif /* defined(__Heur_SSP__SSP__) */
