//
//  CovProb.h
//  Heur_SSP
//
//  Created by Vincent on 4/3/15.
//
//

#ifndef __Heur_SSP__CovProb__
#define __Heur_SSP__CovProb__

#include <stdio.h>
#include "SSP_Solution.h"
#include <ilconcert/iloenv.h>
#include <ilcplex/ilocplex.h>
#include <set>



class column{
    int *_shift;
    int _e;
    int _p;
    int _l;
    int _k;
    string _S;
public:
    column(int e, int p, int pos, int l, int *T){
        _e=e;
        _p=p;
        _l=l;
        _k=pos;
        _shift=new int[_l];
        ostringstream label;
        for(int i=0;i<_l;++i){
            int a=T[i];
            _shift[i]=a;
            if(a<10) label<<"0";
            label<<a;
        }
        _S=label.str();
    }
    
    column(column const &rhs){
        _e=rhs.gete();
        _p=rhs.getp();
        _l=rhs.getl();
        _k=rhs.getk();
        
        int *T=rhs.getShift();
        _shift=new int[_l];
        ostringstream label;
        for(int i=0;i<_l;++i){
            int a=T[i];
            _shift[i]=a;
            if(a<10) label<<"0";
            label<<a;
        }
        _S=label.str();
    }
    
    ~column(){
        delete[] _shift;
    }
    
    bool operator<(const column& rhs) const{
        //return _k<rhs.getk();
        string Srhs=rhs.getS();
        if(_S.compare(Srhs)<0) return true;
        else return false;
            
    }
    
    bool operator==(const column& rhs) const{
        if(_S.compare(rhs.getS())==0) return true;
        else return false;
    }
    
    int *getShift() const {return _shift;}
    int getk() const {return _k;}
    int gete() const {return _e;}
    int getp() const {return _p;}
    int getl() const {return _l;}
    string getS() const {return _S;}
    
    
};


class CovProb{
    
    SSP *_SSP;
    
    set<column>** setShifts;
    IloEnv _env;
    IloModel _model;
    IloCplex _cplex;
    IloObjective _objective;
    
    IloArray<IloNumVarArray> _u;
    IloArray<IloNumVarArray> _o;
    
    IloArray<IloNumVarArray> _a;

    
    IloArray< IloArray<IloNumVarArray> > _x;
    
    IloArray<IloRangeArray> _ConstCov;
    IloArray<IloRangeArray> _ConstCovShift;
    
    
    
public:
    
    CovProb(SSP *SSP){
        _SSP=SSP;
        _model=IloModel(_env);
        _cplex=IloCplex(_model);
        
        _cplex.setParam(IloCplex::TiLim , 300);
        _cplex.setParam(IloCplex::EpGap , 0.01);
        _cplex.setParam(IloCplex::Threads , 1);
	//_cplex.setParam(IloCplex::getNcuts);
	//_cplex.setParam(IloCplex:: ZeroHalfCuts, 2);
	//_cplex.setParam(IloCplex::MIRCuts, 1);
	//_cplex.setParam(IloCplex::LiftProjCuts, 1);

        _cplex.setParam(IloCplex::MIPEmphasis , IloCplex::MIPEmphasisFeasibility );
        //_cplex.setParam(IloCplex::NumericalEmphasis , false );
        //_cplex.setParam(IloCplex::Probe , 3 );


        
        setShifts=new set<column>*[_SSP->nbEmployees];
        for(int e=0;e<_SSP->nbEmployees;++e ){
            setShifts[e]=new set<column>[_SSP->nbPieces[e]];
        }
        
        
        
        //Variables
        _u=IloArray<IloNumVarArray>(_env, _SSP->seqLength);
        _o=IloArray<IloNumVarArray>(_env, _SSP->seqLength);

        for(int i=0;i<_SSP->seqLength;++i){
            _u[i]=IloNumVarArray(_env);
            _o[i]=IloNumVarArray(_env);
            for(int a=0;a<_SSP->nbAct;++a){
                ostringstream labelu;
                labelu << "u_" << i << "_" << a;
                _u[i].add(IloNumVar(_env,0,IloInfinity,ILOFLOAT,labelu.str().c_str()));
                ostringstream labelo;
                labelo << "o_" << i << "_" << a;
                _o[i].add(IloNumVar(_env,0,IloInfinity,ILOFLOAT,labelo.str().c_str()));
                
            }
        }
        
        _x=IloArray<IloArray<IloNumVarArray> >(_env, _SSP->nbEmployees );
        for(int e=0;e<_SSP->nbEmployees;++e ){
            _x[e]=IloArray<IloNumVarArray>(_env, _SSP->nbPieces[e] );
            for(int p=0;p<_SSP->nbPieces[e];++p){
                _x[e][p]=IloNumVarArray(_env);
            }
        }
        
        _a=IloArray<IloNumVarArray>(_env, _SSP->nbEmployees);
        
        for(int e=0;e<_SSP->nbEmployees;++e ){
            _a[e]=IloNumVarArray(_env);
            for(int p=0;p<_SSP->nbPieces[e];++p){
                ostringstream label;
                label << "a_" << e << "_" << p;
                _a[e].add(IloNumVar(_env,0,IloInfinity,ILOFLOAT,label.str().c_str()));
            }
        }
        
        //Objective
        _objective=IloAdd(_model, IloMinimize(_env,0));
        for(int i=0;i<_SSP->seqLength;++i){
            for(int a=0;a<_SSP->nbAct;++a){
                _objective.setLinearCoef(_u[i][a], _SSP->ucAct[i][a]);
                _objective.setLinearCoef(_o[i][a], _SSP->ocAct[i][a]);
            }
        }
        for(int e=0;e<_SSP->nbEmployees;++e ){
            for(int p=0;p<_SSP->nbPieces[e];++p){
                _objective.setLinearCoef(_a[e][p], 1000000);
            }
        }
        
        
        //Constraints Cov
        _ConstCov=IloArray<IloRangeArray>(_env,_SSP->seqLength);
        for(int i=0;i<_SSP->seqLength;++i){
            _ConstCov[i]=IloRangeArray(_env);
            for(int a=0;a<_SSP->nbAct;++a){
                IloExpr expr(_env);
                expr+=_u[i][a]-_o[i][a];
                
                ostringstream label;
                label << "Cov_" << i<<"_"<<a;
                
                _ConstCov[i].add(IloRange(_env, _SSP->dAct[i][a], expr,_SSP->dAct[i][a],label.str().c_str()));
                
                expr.end();
            }
            _model.add(_ConstCov[i]);

        }
        
        //Constraints CovShifts
        _ConstCovShift=IloArray<IloRangeArray>(_env,_SSP->nbEmployees);
        for(int e=0;e<_SSP->nbEmployees;++e ){
            _ConstCovShift[e]=IloRangeArray(_env);
            for(int p=0;p<_SSP->nbPieces[e];++p){
                IloExpr expr(_env);
                //expr+=_a[e][p];
                ostringstream label;
                label << "CovShifts_" << e<<"_"<<p;
                
                _ConstCovShift[e].add(IloRange(_env, 1, expr,1,label.str().c_str()));
            }
            _model.add(_ConstCovShift[e]);
        }
        
        
       
        
    }
    
    ~CovProb(){
        
        
        
        
        for(int e=0;e<_SSP->nbEmployees;++e ){
            delete[] setShifts[e];
        }
        delete[] setShifts;
        

        
        _model.end();
        _cplex.end();
        
        _SSP=NULL;
    }
    
    bool addColumn(int e, int p, int *shift){
        
        int k=(int)setShifts[e][p].size();
        
        column C(e,p,k,_SSP->psl[e][p],shift);
        
//        if(setShifts[e][p].find(C)!=setShifts[e][p].end()){
//            //cout<<"Column removed"<<endl;
//            return ;
//        }
        
        size_t s=setShifts[e][p].size();
        setShifts[e][p].insert(C);
        
        if(s==setShifts[e][p].size()) return false;

                
        IloNumColumn col(_env);
        
        //Objective
        double cost=0;
        for (int t=0; t<_SSP->psl[e][p]; ++t) {
            int a=shift[t];
            if (a>=0 && a<_SSP->nbAct) {
                cost+=_SSP->cAct[t+_SSP->pss[e][p]][a];
                //assert(_SSP->cAct[t+_SSP->pss[e][p]][a]==0);
            }
            else if(a>=_SSP->nbAct+_SSP->nbTask){
                cost+=_SSP->cTask[a-_SSP->nbAct-_SSP->nbTask];
                a-=_SSP->nbTask;
            }
            else if(a<0) a=-1;
            
            if(t>0 && shift[t-1]>=0 && shift[t-1]!=a) cost+=_SSP->costTransit;
        }
        
        col+=_objective( cost);
        
        //CovConst
        for(int i=0;i<_SSP->seqLength;++i){
            for(int a=0;a<_SSP->nbAct;++a){
                if(i<_SSP->pss[e][p] || i>=_SSP->pss[e][p]+_SSP->psl[e][p]){
                    col+=_ConstCov[i][a](0.0);
                }
                else{
                    if(a==shift[i-_SSP->pss[e][p]]){
                        col+=_ConstCov[i][a](1.0);
                    }
                    else{
                        col+=_ConstCov[i][a](0.0);
                    }
                                
                            
                }
            }
        }
        
        //Constraints CovShifts
        for(int ee=0;ee<_SSP->nbEmployees;++ee ){
            for(int pp=0;pp<_SSP->nbPieces[e];++pp){
                if(ee==e && pp==p){
                    col+=_ConstCovShift[e][p](1.0);
                }
                else{
                    col+=_ConstCovShift[e][p](0.0);
                }
            }
        }
        
        ostringstream label;
        label << "x_" << e << "_" << p<<"_"<<k;
        _x[e][p].add(IloNumVar(col,0,1.0,ILOINT,label.str().c_str()));
        col.end();
        
        return true;
        
    }
    
    void addColumn2(int e, int p, int *shift){
        
        int k=(int)setShifts[e][p].size();
        
        column C(e,p,k,_SSP->psl[e][p],shift);
        
        size_t s=setShifts[e][p].size();
        setShifts[e][p].insert(C);
        
        if(s==setShifts[e][p].size()) return ;
        
        ostringstream label;
        label << "x_" << e << "_" << p<<"_"<<k;
        _x[e][p].add(IloNumVar(_env,0,1,ILOINT,label.str().c_str()));
        
        //Objective
        double cost=0;
        for (int t=0; t<_SSP->psl[e][p]; ++t) {
            int a=shift[t];
            if (a>=0 && a<_SSP->nbAct) {
                cost+=_SSP->cAct[t+_SSP->pss[e][p]][a];
            }
            else if(a>=_SSP->nbAct+_SSP->nbTask){
                cost+=_SSP->cTask[a-_SSP->nbAct-_SSP->nbTask];
                a-=_SSP->nbTask;
            }
            else if(a<0) a=-1;

            if(t>0 && shift[t-1]>=0 && shift[t-1]!=a) cost+=_SSP->costTransit;
        }
        
        _objective.setLinearCoef(_x[e][p][k],cost);
        
        //CovConst
        for(int i=_SSP->pss[e][p];i<_SSP->pss[e][p]+_SSP->psl[e][p];++i){
            int a=shift[i-_SSP->pss[e][p]];
            _ConstCov[i][a].setLinearCoef(_x[e][p][k], 1.0);
        }

        //Constraints CovShifts
        _ConstCovShift[e][p].setLinearCoef(_x[e][p][k], 1.0);
        
    }
    
    void exportmodel(string name){
        cout<<"exporting model: "<<name<<endl;
        _cplex.exportModel(name.c_str());
    }
    
    void solve(){
        _cplex.solve();
    }
    
    void setCutUp(double UB){
        _cplex.setParam(IloCplex::CutUp , UB);
    }
    void setCutLo(double LB){
        _cplex.setParam(IloCplex::CutLo , LB);
    }
    
    double getvalx(int e, int p, int k){
        return _cplex.getValue(_x[e][p][k]);
    }
    
    int *getshift(int e, int p){
        
        for(set<column>::iterator it=setShifts[e][p].begin();it!=setShifts[e][p].end();++it ){
            int k=it->getk();
            if(getvalx(e,p,k)>0.5){
                return it->getShift();
            }
        }
        
//        for(int k=0;k<setShifts[e][p].size();++k){
//            if(getvalx(e,p,k)>0.5){
//                set<column>::iterator it=setShifts[e][p].begin();
//                advance(it, k);
//                return it->getShift();
//            }
//        }
        return NULL;
    }
    
    double getObj(){
        return _cplex.getObjValue();
    }
    
    double getGap(){
        return _cplex.getMIPRelativeGap();
    }
    
    int filter(double **u){
        int count=0;
        for(int e=0;e<_SSP->nbEmployees;++e){
            for(int p=0;p<_SSP->nbPieces[e];++p ){
                for(set<column>::iterator it=setShifts[e][p].begin();it!=setShifts[e][p].end();++it ){
                    int *shift=it->getShift();
                    double cost=0;
                    for (int t=0; t<_SSP->psl[e][p]; ++t) {
                        int a=shift[t];
                        if (a>=0 && a<_SSP->nbAct) {
                            cost+=u[a][t+_SSP->pss[e][p]];
                        }
                        if(a>=0){
                            if(t>0 && shift[t-1]>=0 && shift[t-1]!=a) cost+=_SSP->costTransit;
                        }
                    }
                    if(cost>0.001){
                        int k=it->getk();
                        _x[e][p][k].setUB(0.0);
                        count++;
                    }
                }
            }
            
        }
        return count;
    }
    
    
    
    
    
    
};

#endif /* defined(__Heur_SSP__CovProb__) */
