//
//  SSP_Solution.h
//  Heur_SSP
//
//  Created by Vincent on 11/4/14.
//
//

#ifndef __Heur_SSP__SSP_Solution__
#define __Heur_SSP__SSP_Solution__

#include <stdio.h>
#include "SSP.h"
#include "SubP.h"
#include <iomanip>


class SSP_Solution{
    
    const int destroyCode=-1000;
    
    double cost;
    int totalover;
    int totalunder;
    int nTransit;
    int* delta;
    int ** AssignedShifts;
    SSP *_SSP;
    
public:
    
    SSP_Solution(SSP *SSP){
        _SSP=SSP;
        AssignedShifts=new int*[_SSP->nbEmployees];
        for(int e=0;e<_SSP->nbEmployees;++e){
            AssignedShifts[e]=new int[_SSP->seqLength];
            
            for (int i=0; i<_SSP->seqLength; ++i) {
                AssignedShifts[e][i]=-2; //Nothing assigned
            }
        }
        cost=1000000;
        
        delta=new int[_SSP->nbAct];
        
        Update();

    }
    
    SSP_Solution(SSP_Solution *S){
        _SSP=S->getSSP();
        AssignedShifts=new int*[_SSP->nbEmployees];
        for(int e=0;e<_SSP->nbEmployees;++e){
            AssignedShifts[e]=new int[_SSP->seqLength];
            
            for (int i=0; i<_SSP->seqLength; ++i) {
                AssignedShifts[e][i]=S->getAssignedShifts(e , i);
            }
        }
        cost=S->getcost();
        totalover=S->getOverCovering();
        totalunder=S->getUnderCovering();
        nTransit=S->getnTransit();
        
        delta=new int[_SSP->nbAct];
    }
    
    ~SSP_Solution(){
        for(int e=0;e<_SSP->nbEmployees;++e){
            delete[] AssignedShifts[e];
        }
        delete[] AssignedShifts;        
        _SSP=NULL;
        
        delete[] delta;
        
        
    }
    
    double getcost() {return cost;}
    int getAssignedShifts(int e, int i) {return AssignedShifts[e][i];}
    int* getAssignedShifts(int e) {return AssignedShifts[e];}

    void setAssignedShifts(int e, int i, int a) {AssignedShifts[e][i]=a;}
    
    void destroy(int e, int i) {AssignedShifts[e][i]=destroyCode; cost=1000000;}
    bool isdestroyed(int e, int i) {if(AssignedShifts[e][i]==destroyCode) return true; else return false;}

    int getUnderCovering(){return totalunder;}
    int getOverCovering(){return totalover;}
    int getnTransit() {return nTransit;}

    SSP* getSSP() {return _SSP;};
    
    void Update(){
        
    
        //Compute the cost
        
        nTransit=0;
        totalover=0;
        totalunder=0;
        cost=0;

        for (int i=0; i<_SSP->seqLength; ++i) {
            
            for(int a=0;a<_SSP->nbAct;++a) delta[a]=_SSP->dAct[i][a];
            
            for(int e=0;e<_SSP->nbEmployees;++e){
                int a = AssignedShifts[e][i];
                if(a>=0 && a<_SSP->nbAct){
                    delta[a]-=1;
                }
                
            }
            
            for(int a=0;a<_SSP->nbAct;++a){
                if(delta[a]>0){
                    cost+=_SSP->ucAct[i][a]*delta[a];
                    totalunder+=delta[a];
                }
                else{
                    cost-=_SSP->ocAct[i][a]*delta[a];
                    totalover-=delta[a];
                }
            }
        }
        
        
        
        for(int e=0;e<_SSP->nbEmployees;++e){
            for(int p=0;p<_SSP->nbPieces[e];++p){
                for(int i=_SSP->pss[e][p];i<_SSP->pss[e][p]+_SSP->psl[e][p];++i){
                    if(i>_SSP->pss[e][p] && AssignedShifts[e][i]!=AssignedShifts[e][i-1]) nTransit++;
                }
            }
        }
        
        cost+=nTransit*_SSP->costTransit;
        
        
        
        
        
    }
    
    void print(){
        cout<<"cost= "<<cost<<endl;
        for(int e=0;e<_SSP->nbEmployees;++e){
            cout<<setw(3)<<e<<" : ";
            
            for(int i=0;i<_SSP->seqLength;++i){
                int a = AssignedShifts[e][i];
                if(a>=0) cout<<setw(3)<<a;
                else cout<<setw(3)<<"_";
            }
            cout<<endl;
            
        }
        
        cout<<"Activities"<<endl;
        for(int a=0;a<_SSP->nbAct;++a){
            cout<<setw(3)<<a<<" : ";
            for (int i=0; i<_SSP->seqLength; ++i) {
                
                int cov=_SSP->dAct[i][a];
                
                for(int e=0;e<_SSP->nbEmployees;++e) if(a==AssignedShifts[e][i]){
                    if(a>=0 && a<_SSP->nbAct){
                        cov-=1;
                    }
                }
                cout<<setw(3)<<cov;
                
            }
            cout<<setw(3)<<_SSP->minAct[a]<<setw(3)<<_SSP->maxAct[a]<<endl;
        }
        
        cout<<"Demands"<<endl;
        for(int a=0;a<_SSP->nbAct;++a){
            cout<<setw(3)<<a<<" : ";
            for (int i=0; i<_SSP->seqLength; ++i) {
                cout<<setw(3)<<_SSP->dAct[i][a];
                
            }
            cout<<setw(3)<<_SSP->minAct[a]<<setw(3)<<_SSP->maxAct[a]<<endl;
        }
        
        cout<<"Min/MaxAct"<<endl;
        for(int a=0;a<_SSP->nbAct;++a){
            cout<<setw(3)<<a<<" : ";
            cout<<setw(3)<<_SSP->minAct[a]<<setw(3)<<_SSP->maxAct[a]<<endl;
        }
        
        cout<<"Skills"<<endl;
        for(int e=0;e<_SSP->nbEmployees;++e){
            cout<<setw(3)<<e<<" : ";
            for(set<int>::iterator it=_SSP->emplSkills[e].begin();it!=_SSP->emplSkills[e].end();++it){
                int a=*it;
                cout<<setw(3)<<a;
            }
            cout<<endl;
        }
    }
    
    void printshift(int e, int p){
        
        cout<<setw(3)<<e<<setw(3)<<p<<setw(3)<<": ";
        
        for(int i=_SSP->pss[e][p];i<_SSP->pss[e][p]+_SSP->psl[e][p];++i){
            int a = AssignedShifts[e][i];
            if(a>=0) cout<<setw(4)<<a;
            else cout<<setw(4)<<" ";
        }
        cout<<endl;
        
        cout<<setw(3)<<" "<<setw(3)<<p<<setw(3)<<" ";
        for(int i=_SSP->pss[e][p];i<_SSP->pss[e][p]+_SSP->psl[e][p];++i){
            cout<<setw(4)<<i;
        }
        cout<<endl;

        
    }
    
    
    bool checkFeasibility(){
        
        for(int e=0;e<_SSP->nbEmployees;++e){
            for(int p=0;p<_SSP->nbPieces[e];++p){
                int l=0;
                for(int i=_SSP->pss[e][p];i<_SSP->pss[e][p]+_SSP->psl[e][p];++i){
                    int a=AssignedShifts[e][i];
                    if(_SSP->emplSkills[e].find(a)==_SSP->emplSkills[e].end()){
                        cout<<"Employee "<<e<<" cannot do activity "<<a<<" at position "<<i<<endl;
                        return false;
                    }
                    if(a<0){
                        cout<<"Employee "<<e<<" is doind nothing at position "<<i<<endl;
                        return false;
                    }
                    if((i>_SSP->pss[e][p] && a!=AssignedShifts[e][i-1])){
                        a=AssignedShifts[e][i-1];

                        if(l<_SSP->minAct[a] || l>_SSP->maxAct[a]){
                            cout<<"Employee "<<e<<" is doing activity "<<a<<" at position "<<i<<" with length "<<l<<" ["<<_SSP->minAct[a]<<", "<<_SSP->maxAct[a]<<"]"<<endl;
                            printshift(e , p);
                            return false;
                        }
                        else l=0;
                    }
                    if(i==_SSP->pss[e][p]+_SSP->psl[e][p]-1){
                        l++;
                        if(l<_SSP->minAct[a] || l>_SSP->maxAct[a]){
                            cout<<"Employee "<<e<<" is doing activity "<<a<<" at position "<<i<<" with length "<<l<<" ["<<_SSP->minAct[a]<<", "<<_SSP->maxAct[a]<<"]"<<endl;
                            printshift(e , p);
                            return false;
                        }
                        else l=0;
                    }
                    l++;
                }
            }
        }
        
        
        return true;
    }
    
    
    
};

#endif /* defined(__Heur_SSP__SSP_Solution__) */
