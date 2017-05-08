/* Autores: Pamela Jocelyn Palomo Martínez
		Norberto Alejandro Hernández Leandro*/

#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <fstream>
#include <string>

using namespace std;

struct point{
	double x;
	double y;
	int s_key;
	int indic;
	int s_key_int_i;
	int s_key_int_d;
	bool operator==(const point& rhs) const{
		return x==rhs.x;
    	}

	bool operator<(const point& rhs) const{
		return x<rhs.x; 
	}
};

struct less_than_key
{
    inline bool operator() (const point& struct1, const point& struct2)
    {
        return (struct1.y < struct2.y);
    }
};

struct equal_than_key
{
    inline bool operator() (const point& struct1, const point& struct2)
    {
        return (struct1.s_key == struct2.s_key);
    }
};


struct segment{
	point p1;
	point p2;
};


int main(int argv, char* argc[]){
	string pth = argc[1];
	ifstream ifile(pth.c_str(), ios::in);
	point pinic, pfin, pto,pto2, ptoint;
	segment s;
	double m1, m2;
	vector<segment> segmentos;
	vector<point> puntos;
	vector<point> puntosy;

	double x,y;
	while (!ifile.eof()){
		ifile >> x;
		ifile >> y;
		if(ifile.eof()){
			break;
		}		
		
		
		pinic.x = (x<y)?x:y;
		pinic.y = (x>y)?x:y;
		cout << pinic.x << " " << pinic.y << endl;
		pinic.indic = 0;	
		ifile >> x;
		ifile >> y;
		
		pfin.x = (x<y)?x:y;
		pfin.y = (x>y)?x:y;
		cout << pfin.x << " " <<pfin.y << endl;
		pfin.indic = 1;	
		
	
		s.p1=pinic;
		s.p2=pfin;
		segmentos.push_back(s);
	}
	
	for(int i=0; i<segmentos.size(); i++){
		segmentos[i].p1.s_key = i;
		segmentos[i].p2.s_key = i;
		puntos.push_back(segmentos[i].p1); 
		puntos.push_back(segmentos[i].p2); 
	}
	sort(puntos.begin(),puntos.end());
	
	while(!puntos.empty()){
		double x,y;
		pto=*puntos.begin();
		puntos.erase(puntos.begin());

		if(pto.indic == 0){
			puntosy.push_back(pto);
			puntosy.push_back(segmentos[pto.s_key].p2); 
			sort(puntosy.begin(),puntosy.end(),less_than_key());
			if(pto.y!=puntosy.front().y){
				vector<point>::iterator it;
				it = find(puntosy.begin(),puntosy.end(),pto);
				--it;
				pto2=*it;
				m1 = (segmentos[pto.s_key].p2.y-segmentos[pto.s_key].p1.y)/(segmentos[pto.s_key].p2.x-segmentos[pto.s_key].p1.x);
				m2 = (segmentos[pto2.s_key].p2.y-segmentos[pto2.s_key].p1.y)/(segmentos[pto2.s_key].p2.x-segmentos[pto2.s_key].p1.x);
				
				x = (m1*segmentos[pto.s_key].p1.x-m2*segmentos[pto2.s_key].p1.x+segmentos[pto2.s_key].p1.y-segmentos[pto.s_key].p1.y)/(m1-m2);
				y = m1*x-m1*segmentos[pto.s_key].p1.x +segmentos[pto.s_key].p1.y;
				ptoint.x = x;
				ptoint.y = y;
				ptoint.indic = 3;
				ptoint.s_key_int_i = pto2.s_key;
				ptoint.s_key_int_d = pto.s_key;
				if(x > pto.x){
					puntos.push_back(ptoint);
					sort(puntos.begin(),puntos.end());
				}
			}
			if(pto.y!=puntosy.back().y){
				vector<point>::iterator it;
				it = find(puntosy.begin(),puntosy.end(),pto);
				++it;
				pto2=*it;
				m1 = (segmentos[pto.s_key].p2.y-segmentos[pto.s_key].p1.y)/(segmentos[pto.s_key].p2.x-segmentos[pto.s_key].p1.x);
				m2 = (segmentos[pto2.s_key].p2.y-segmentos[pto2.s_key].p1.y)/(segmentos[pto2.s_key].p2.x-segmentos[pto2.s_key].p1.x);
				
				x = (m1*segmentos[pto.s_key].p1.x-m2*segmentos[pto2.s_key].p1.x+segmentos[pto2.s_key].p1.y-segmentos[pto.s_key].p1.y)/(m1-m2);
				y = m1*x-m1*segmentos[pto.s_key].p1.x +segmentos[pto.s_key].p1.y;
				ptoint.x = x;
				ptoint.y = y;
				ptoint.indic = 3;
				ptoint.s_key_int_i = pto.s_key;
				ptoint.s_key_int_d = pto2.s_key;
				if(x > pto.x){
					puntos.push_back(ptoint);
					sort(puntos.begin(),puntos.end());
				}
			}

		}else if(pto.indic==1){

			vector<point>::iterator it,it2;
			point ptod=NULL, ptoi=NULL;

			it = find(puntosy.begin(),puntosy.end(),pto);
			it2= find(puntosy.begin(),puntosy.end(), segmentos[pto.s_key].p1);
			++it;
			if(it!=puntosy.end()){
				 ptod=*it;
			}
			--it;
			if(it!=puntosy.begin()){
				--it;
				ptoi = *it;
				++it;
			}
			if(ptod!=NULL && ptoi != NULL){
				m1 = (segmentos[ptoi.s_key].p2.y-segmentos[ptoi.s_key].p1.y)/(segmentos[ptoi.s_key].p2.x-segmentos[ptoi.s_key].p1.x);
				m2 = (segmentos[ptod.s_key].p2.y-segmentos[ptod.s_key].p1.y)/(segmentos[ptod.s_key].p2.x-segmentos[ptod.s_key].p1.x);
				
				x = (m1*segmentos[ptoi.s_key].p1.x-m2*segmentos[ptod.s_key].p1.x+segmentos[ptod.s_key].p1.y-segmentos[ptoi.s_key].p1.y)/(m1-m2);
				y = m1*x-m1*segmentos[ptoi.s_key].p1.x +segmentos[ptoi.s_key].p1.y;
				ptoint.x=x;
				ptoint.y=y;
				ptoint.indic=3;
				ptoint.s_key_int_i = ptoi.s_key;
				ptoint.s_key_int_d = ptod.s_key;
				if(x > pto.x){
					puntos.push_back(ptoint);
					sort(puntos.begin(),puntos.end());
				}
			}
			
			puntosy.erase(it);
			puntosy.erase(it2);
		}else{
			

		}
	}	


	ifile.close();

}
