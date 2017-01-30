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
	const char* pth=argc[1];
	string Cinta;
	string temp;
	EstSym Pair;
	Output outpt;
	Transicion Tr;
	
	list<Transicion> Trans;
	ifstream FileM(pth,ios::in);
	
	FileM>>Cinta;
	while(!FileM.eof()){
		FileM>>temp;
		Tr.input.Estado=temp.c_str();
		FileM>>temp;
		Tr.input.symbol=temp.c_str();
		FileM>>temp;
		Tr.outp.Estado=temp.c_str();
		FileM>>temp;
		Tr.outp.symbol=temp.c_str();
		FileM>>temp;
		Tr.outp.trans=temp.c_str();
		Trans.push_back(Tr);
		cout<<temp<<endl;
	}
	
	
	return 0;
}