
#include "leer_arc.h"

leer_arc::leer_arc(string pth, Grafo *gr){
	ifstream _FileA(pth.c_str(),ios::in);
	string tmp;
	_gr=gr;
	if(!_FileA.good()){
		cout << "No se puede abrir el archivo: " << pth << endl;
		exit(0);
	}
	int n;
	int i,j;
	double dist;
	_FileA >> n;
 	_gr->Definir_NVertices(n);
	_gr->Definir_Dirigido(false);
	 while (!_FileA.eof()){
		_FileA >> i;
		_FileA >> j;
		_FileA >> dist;
		_gr-> Agregar_Vecino(i,j);
		_gr-> Agregar_Vecino(j,i);
		_gr -> Definir_Distancia(i,j,dist);
		_gr -> Definir_Distancia(j,i,dist);				
	}
	
	cout << "Archivo: " << pth << " leido correctamente." << endl;
	_FileA.close();
	tmp.clear();
}

leer_arc::~leer_arc(){
}
