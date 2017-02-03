#include "grafos.h"


Grafo::Grafo(){

}

Grafo::~Grafo(){
	_Arcos.clear();
	for(int i = 0; i < _nV; i++){
		_Vertices[i].Neighbors.clear();
		_Adyac[i].clear();
	}
	delete[] _Vertices;
	delete[] _Adyac;
}

void Grafo::Definir_NVertices(int n){
	_nV = n;
	_Vertices = new Vertice[n];
	_Adyac = new set<int>[n];
}

void Grafo::Definir_Dirigido(bool is){
	is_Dir=is;
}

void Grafo::Agregar_Vecino(int prin, int vec){
	if(vec < 0 || vec>=_nV){
		cout << "No se puede agregar el vecino " << vec << endl;
		cout << "Revisa el numero de vertices." << endl;
	}else if (prin <0 || prin >=_nV){
		cout << "El nodo " << prin << " no existe." << endl;
	}else{
		_Vertices[prin].Neighbors.insert(vec);
	}
}

void Grafo::Imprimir(){
	cout << endl;
	if(is_Dir){
		cout << "Grafo dirigido." << endl;
	}else{
		cout << "Grafo no dirigido." << endl;
	}

	cout << endl << "Numero de vertices: " << _nV << endl << endl;

	cout << "Arcos: " << endl;

	for (int  i=0; i<_nV; i++){
		for (auto el : _Vertices[i].Neighbors){
			cout << "(" << i << "," << el << ")" << "\t"; 
		}
		cout << endl;
	}
}
