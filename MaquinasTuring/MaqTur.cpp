#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include <string.h>


using namespace std;

struct EstSym{
	
	const char* Estado;
	const char* symbol;
	bool operator==(const EstSym& rhs) const{
        if(strcmp(Estado, rhs.Estado)==0 && strcmp(symbol,rhs.symbol)==0) return true;
        else return false;
    }
};

struct Output{
	const char* Estado;
	const char* symbol;
	const char* trans;
	bool operator==(const Output& rhs) const{
        if(strcmp(Estado, rhs.Estado)==0 && strcmp(symbol,rhs.symbol)==0 &&  strcmp(trans,rhs.trans)==0) return true;
        else return false;
    }	
};

struct Transicion{
	EstSym input;
	Output outp;
};




int main(int argv,char* argc[]){
	const char pth=argc[1];
	const char* Cinta;
	const char* temp;
	list<Transicion> Trans;
	ifstream FileM(pth,ios::in);
	while(!FileM.eof()){
		FileM>>temp;
		cout<<temp<<endl;
	}
	
	
	return 0;
}