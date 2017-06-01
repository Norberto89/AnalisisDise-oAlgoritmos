//
//  Constructor.h
//  Heur_SSP
//
//  Created by Vincent on 11/4/14.
//
//

#ifndef __Heur_SSP__Constructor__
#define __Heur_SSP__Constructor__

#include <stdio.h>
#include "SSP_Solution.h"
#include "SubP.h"
#include <math.h>
#include "CovProb.h"




class Heuristic{
    
protected:
    SubP *_SubProblem;
    SSP *_SSP;
    double **piRC;
    
    int _maxit;
    
public:
    
    Heuristic(SubP *SubProblem, SSP *SSP){
        _SubProblem=SubProblem;
        _SSP=SSP;
        piRC=new double*[_SSP->nbAct+2*_SSP->nbTask];
        for(int a=0;a<_SSP->nbAct+2*_SSP->nbTask;++a){
            piRC[a]=new double[_SSP->seqLength];
            for(int i=0;i<_SSP->seqLength;++i) piRC[a][i]=0.0;
        }
        
    };
    
    virtual ~Heuristic(){
        for(int a=0;a<_SSP->nbAct+2*_SSP->nbTask;++a) delete[] piRC[a];
        delete[] piRC;
        
        _SubProblem=NULL;
        _SSP=NULL;
    };
        
    virtual string getName()=0;
    
    virtual void solve(SSP_Solution *S)=0;
    
};


class Lagrange : public Heuristic{
    
    
    double **cov;
    double **u;
    double lambda;
    CovProb *CP;
    double OpGap;
    int max_ite;
    double min_alfa;
    int _distr;
    
public:
    
    Lagrange(SubP *SubProblem, SSP *SSP,int distr) : Heuristic(SubProblem,SSP){
	_distr = distr;
	max_ite = 75;
	min_alfa = 0.0001;
        cov=new double*[_SSP->nbAct+2*_SSP->nbTask];
        u=new double*[_SSP->nbAct+2*_SSP->nbTask];
        for (int a=0; a<_SSP->nbAct+2*_SSP->nbTask; ++a) {
            cov[a]=new double[_SSP->seqLength];
            u[a]=new double[_SSP->seqLength];
            
        }
        
        lambda=2;
        
        CP=new CovProb(_SSP);
        
        OpGap=1.0;
        
    };
    
    ~Lagrange(){
        
        for (int a=0; a<_SSP->nbAct+2*_SSP->nbTask; ++a){
            delete[] cov[a];
            delete[] u[a];
        }
        delete[] cov;
        delete[] u;
        delete CP;

    };
    
    double computeLagrangianCost(SSP_Solution *S){
        double cost=0.0;
        for (int i=0; i<_SSP->seqLength; ++i) {
            for(int e=0;e<_SSP->nbEmployees;++e){
                int a=S->getAssignedShifts(e, i);
                int aprev=-2;
                if(i>0) aprev=S->getAssignedShifts(e, i-1);
                if(a>=0 && a<_SSP->nbAct){
                    if(aprev>=0 && aprev!=a) cost+=_SSP->costTransit;
                }
            }
            
            for(int a=0;a<_SSP->nbAct;++a) {
                assert(_SSP->nbEmployees-_SSP->dAct[i][a]>=0);
//                double valu=_SSP->dAct[i][a];
//                double costu=(_SSP->ucAct[i][a]+u[a][i])*valu;
//                double valo=_SSP->nbEmployees-_SSP->dAct[i][a];
//                double costo=(_SSP->ocAct[i][a]-u[a][i])*valo;
//                
//                if(costu<0 && costu<costo){
//                    cost+=costu;
//                }
//                else if(costo<0 && costo<costu){
//                    cost+=costo;
//                }
                
                if(_SSP->ucAct[i][a]+u[a][i]<0) cost+=_SSP->dAct[i][a]*(_SSP->ucAct[i][a]);
                if(_SSP->ocAct[i][a]-u[a][i]<0) cost+=(_SSP->nbEmployees-_SSP->dAct[i][a])*(_SSP->ocAct[i][a]);
                cost+=u[a][i]*cov[a][i];
            }
        }
        
        return cost;
    }
    
    void updateCovering(SSP_Solution *S){
        //cov=Ax-b
        for (int i=0; i<_SSP->seqLength; ++i) {
            
            for(int a=0;a<_SSP->nbAct;++a){
                cov[a][i]=-_SSP->dAct[i][a];
                if(_SSP->ucAct[i][a]+u[a][i]<0) cov[a][i]+=_SSP->dAct[i][a];
                if(_SSP->ocAct[i][a]-u[a][i]<0) cov[a][i]-=(_SSP->nbEmployees-_SSP->dAct[i][a]);
            }
            
            for(int e=0;e<_SSP->nbEmployees;++e){
                int a = S->getAssignedShifts(e,i);
                if(a>=0 && a<_SSP->nbAct){
                    cov[a][i]+=1;
                }
            }
        }
    }
    
    void updateu(double Lcost, SSP_Solution *SBest){
        double N=0.0;
        for (int i=0; i<_SSP->seqLength; ++i) {
            for(int a=0;a<_SSP->nbAct;++a){
                N+=pow((double)(cov[a][i]),2.0);
            }
        }
        assert(N>0.01);
        double t=lambda*(SBest->getcost()-Lcost)/N;
        assert(t>=-0.01);
        
        for (int i=0; i<_SSP->seqLength; ++i) {
            for(int a=0;a<_SSP->nbAct;++a){
                u[a][i]+=t*(cov[a][i]);
                //u[a][i]=min((double)_SSP->ocAct[i][a],u[a][i]);
                //u[a][i]=max(-(double)_SSP->ucAct[i][a],u[a][i]);
                //u[a][i]=max(0.0,u[a][i]);
            }
        }
    }
    
    void solve(SSP_Solution *S){
        srand(time(NULL));
	
	default_random_engine generator;
	normal_distribution<double> distribution(100.0,20.0);
	uniform_int_distribution<int> distribution2(0,100);

        if(_distr==0){
        	for(int a=0;a<_SSP->nbAct+2*_SSP->nbTask;++a){
            		for(int i=0;i<_SSP->seqLength;++i) u[a][i]=0;
        	}
	}else if(_distr==1){
		for(int a=0;a<_SSP->nbAct+2*_SSP->nbTask;++a){
            		for(int i=0;i<_SSP->seqLength;++i) u[a][i]=rand();
        	}
	}else if (_distr==2){
		for(int a=0;a<_SSP->nbAct+2*_SSP->nbTask;++a){
            		for(int i=0;i<_SSP->seqLength;++i) u[a][i]= distribution(generator);
        	}		
	}else{
		for(int a=0;a<_SSP->nbAct+2*_SSP->nbTask;++a){
            		for(int i=0;i<_SSP->seqLength;++i) u[a][i]= distribution2(generator);
        	}
	}
//      for(int a=0;a<_SSP->nbAct;++a){
//          for(int i=0;i<_SSP->seqLength;++i) u[a][i]=-100*_SSP->dAct[i][a];
//      }
        
        SSP_Solution *Scurr=new SSP_Solution(S);
        SSP_Solution *SBest=new SSP_Solution(S);
        
        
        //Solving the lagrangian relaxation
        int count=0;
        lambda=2;
        double Lcost=-std::numeric_limits<double>::infinity();
        double Lcostmax=-std::numeric_limits<double>::infinity();
        int iter=0;
        double t0=clock();
        int *T=new int[_SSP->maxLengthPiece];
        while(lambda>min_alfa){
            
            iter++;
                        
            //Rebuilt Scurr
            for(int e=0;e<_SSP->nbEmployees;++e){
                for(int p=0;p<_SSP->nbPieces[e];++p){
                    _SubProblem->solveSubProblemWithDP(e, p, u, true, T);
                    for(int i=_SSP->pss[e][p];i<_SSP->pss[e][p]+_SSP->psl[e][p];++i){
                        Scurr->setAssignedShifts(e, i , T[i-_SSP->pss[e][p]]);
                    }
                    CP->addColumn(e, p, T);
                }
            }
            
          
            
            //Compute the covering
            updateCovering(Scurr);
            
            //Cost of feasible solution
            Scurr->Update();
            
            if(UpdateBestSolution(SBest, Scurr)){
                //count=0;
                //count=min(0,count);
            }
            
           
//            if(Scurr->getcost()<SBest->getcost()){
//                delete SBest;
//                SBest=new SSP_Solution(Scurr);
//            }
            
            //Update Lagrangian cost
            Lcost=computeLagrangianCost(Scurr);
            
            if(Lcost-Lcostmax<=0.01){
                count++;
            }
            else{
                Lcostmax=Lcost;
                count=0;
            }
            
            if(count>max_ite){
                count=0;
                lambda/=2;
            }
            
            cout<<setw(20)<<iter<<setw(20)<<SBest->getcost()<<setw(20)<<Scurr->getcost()<<setw(20)<<Lcost<<setw(20)<<Lcostmax<<setw(20)<<lambda<<endl;
            
            //Update u
            updateu(Lcostmax,SBest);

            
        }
        delete[] T;
        
        t0=(double)(clock()-t0)/(double)CLOCKS_PER_SEC;
        
        cout<<"Lagrangian Relaxation Time: "<<t0<<" s."<<endl;
        
        //int n=CP->filter(u );
        //cout<<"Number of Columns Removed: "<<n<<endl;
        
        double bestcost=2*SBest->getcost();
        double MIPobj=SBest->getcost();
        double gap=1.0;
        CP->setCutUp(MIPobj);
        CP->setCutLo(ceil(Lcostmax));
        while((bestcost>MIPobj /*&& gap>0.01*/) || gap>0.01){
            bestcost=MIPobj;
            CP->solve();
            MIPobj=CP->getObj();
            gap=CP->getGap();
        }
        
        for(int e=0;e<_SSP->nbEmployees;++e){
            for(int p=0;p<_SSP->nbPieces[e];++p){
                int *Shift=CP->getshift(e, p);
                for(int i=_SSP->pss[e][p];i<_SSP->pss[e][p]+_SSP->psl[e][p];++i){
                    S->setAssignedShifts(e, i , Shift[i-_SSP->pss[e][p]]);
                }
                
            }
        }
        S->Update();
        
        OpGap=(S->getcost()-Lcostmax)/Lcostmax;
        
        cout<<"Best Solution Found: "<<S->getcost()<<endl;
        cout<<"Optimality Gap: "<<OpGap<<endl;

        
        if(S->checkFeasibility()){
            cout<<"Solution is feasible"<<endl;
        }
        else {
            cout<<"Solution is NOT feasible"<<endl;
        }

        delete Scurr;
        delete SBest;
        
    };
    
    string getName() {return "Lagrange";}
    
    
    bool UpdateBestSolution(SSP_Solution *SBest, SSP_Solution *S){
        
        bool updated=false;
        
        set<int> sete;
        set<int> *setp=new set<int>[_SSP->nbEmployees];
        for(int e=0;e<_SSP->nbEmployees;++e){
            sete.insert(e);
            for(int p=0;p<_SSP->nbPieces[e];++p){
                setp[e].insert(p);
            }
        }
        
        double costBest=SBest->getcost();
        int *shift=new int[_SSP->seqLength];
        
        while(sete.size()!=0){
            
            int re=rand()%sete.size();
            set<int>::iterator ite=sete.begin();
            advance(ite,re);
            int es=*ite;
            
            
            int rp=rand()%setp[es].size();
            set<int>::iterator itp=setp[es].begin();
            advance(itp,rp);
            int ps=*itp;
            
            setp[es].erase(ps);
            if(setp[es].size()==0) sete.erase(es);
            
            bool update=false;
            for(int i=_SSP->pss[es][ps];i<_SSP->pss[es][ps]+_SSP->psl[es][ps];++i){
                shift[i]=SBest->getAssignedShifts(es, i);
                int a=S->getAssignedShifts(es, i);
                SBest->setAssignedShifts(es, i , a);
                if(a!=shift[i]) update=true;
            }
            
            if(!update) continue;
            
            SBest->Update();
            if(SBest->getcost()>costBest){
                for(int i=_SSP->pss[es][ps];i<_SSP->pss[es][ps]+_SSP->psl[es][ps];++i){
                    int a=shift[i];
                    SBest->setAssignedShifts(es, i , a);
                }
            }
            else{
                if(SBest->getcost()<costBest) updated=true;
                costBest=SBest->getcost();
                
                for(int e=0;e<_SSP->nbEmployees;++e){
                    for(int p=0;p<_SSP->nbPieces[e];++p){
                        if(_SSP->pss[es][ps]>=_SSP->pss[e][p] && _SSP->pss[es][ps]<_SSP->pss[e][p]+_SSP->psl[e][p]){
                            sete.insert(e);
                            setp[e].insert(p);
                        }
                        else if(_SSP->pss[es][ps]+_SSP->psl[es][ps]>=_SSP->pss[e][p] && _SSP->pss[es][ps]+_SSP->psl[es][ps]<_SSP->pss[e][p]+_SSP->psl[e][p]){
                            sete.insert(e);
                            setp[e].insert(p);
                        }
                    }
                }
            }
            
            
        }
        
        SBest->Update();
        
        delete[] shift;

        delete[] setp;
        
        return updated;
        
    }
    
};


#endif /* defined(__Heur_SSP__Constructor__) */
