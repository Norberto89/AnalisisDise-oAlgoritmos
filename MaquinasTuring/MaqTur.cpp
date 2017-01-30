#include <iostream>
#include <fstream>
#include <list>
#include <stdlib.h>
#include <math.h>
#include <string>


using namespace std;

struct EstSym{
	
	string Estado;
	string symbol;
	bool operator==(const EstSym& rhs) const{
        if(Estado.compare(rhs.Estado)==0 && symbol.compare(rhs.symbol)==0) return true;
        else return false;
    }
};

struct Output{
	string Estado;
	string symbol;
	string trans;
	bool operator==(const Output& rhs) const{
        if(Estado.compare(rhs.Estado)==0 && symbol.compare(rhs.symbol)==0 &&  trans.compare(rhs.trans)==0) return true;
        else return false;
    }	
};

struct Transicion{
	EstSym input;
	Output outp;
};


bool find_L(list<Transicion> Trans, EstSym Elem, Output *OP){
	list<Transicion>::iterator IniT=Trans.begin(),FinIt=Trans.end();
	--FinIt;
	int siz=ceil((double)Trans.size()/2.0);
	for(int i=0;i<siz;i++){
		if(Elem==IniT->input || Elem==FinIt->input){
			*OP=(Elem==IniT->input)?IniT->outp:FinIt->outp;
			return true;
		}
		--FinIt;
		++IniT;
	}
	return false;
}


int main(int argv,char* argc[]){
	const char* pth=argc[1];
	string Cinta;
	string temp;
	Output outpt;
	Transicion Tr;
	
	list<Transicion> Trans;
	
	ifstream FileM(pth,ios::in);
	
	FileM>>Cinta;
	cout<< "Transiciones: "<<endl;
	
	while(!FileM.eof()){
		FileM>>Tr.input.Estado;
		FileM>>Tr.input.symbol;
		FileM>>Tr.outp.Estado;
		FileM>>Tr.outp.symbol;
		FileM>>Tr.outp.trans;
		Trans.push_back(Tr);
		cout << "(" << Tr.input.Estado<<","<<Tr.input.symbol<<")" <<"->("<<Tr.outp.Estado<<"," << Tr.outp.symbol<< "," << Tr.outp.trans<<")"<<endl;
	}
	cout<<"--------------------------------"<<endl<<endl<<endl;
	
	cout<<"Cinta: "<<endl;
	cout<<Cinta<<endl<<endl;

	cout << "Operaciones realizadas: "<<endl;
	EstSym Pair;
	
	Pair.Estado="s";
	Pair.symbol="init";
	Output op;
	bool fin=find_L(Trans, Pair, &op);
	int count=0;
	if(fin){
		cout << "(" << Pair.Estado<<","<<Pair.symbol<<")" <<"->("<<op.Estado<<"," << op.symbol<< "," << op.trans<<")"<<endl;
		while(op.Estado!= "stop" && op.Estado != "yes" && op.Estado != "no"){
			Pair.Estado=op.Estado;
			if(count>=0 && count<Cinta.size()){
				Pair.symbol=Cinta[count];
			}else if(count < 0){
				Pair.symbol="init";
			}else{
				Pair.symbol="blank";
			}
			if(op.trans=="left"){
				count--;
			}else if(op.trans=="right"){
				count++;
			}
			fin=find_L(Trans,Pair, &op);
			if(!fin && (Pair.Estado== "stop" || Pair.Estado == "yes" || Pair.Estado == "no")){
				cout<<"Estado no encontrado"<<endl;
				cout<< "No puedo continuar"<<endl<<endl;
				exit(0);
			}
			cout << "(" << Pair.Estado<<","<<Pair.symbol<<")" <<"->("<<op.Estado<<"," << op.symbol<< "," << op.trans<<")"<<endl;
		}
	}else{
		cout<<"Estado no encontrado"<<endl;
		cout << "No puedo continuar"<<endl<<endl;
		exit(0);
	}
	cout<<endl;
	cout<< op.Estado <<endl<<endl;
	
	return 0;
}