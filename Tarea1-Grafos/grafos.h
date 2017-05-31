#pragma once
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <list>
#include <set>
#include <math.h>
#include <stdlib.h>

using namespace std;


struct Vertice{
	int id;
	set<int> Neighbors;

	bool operator==(const Vertice& rhs) const{
		return id==rhs.id;
    	}

	bool operator<(const Vertice& rhs) const{
		return id<rhs.id; 
	}
};

struct Arco{
	int x;
	int y;
};


class Grafo{

private:
	int _nV;
	Vertice* _Vertices;
	set<int>* _Adyac;
	list<Arco> _Arcos;
	bool is_Dir;

public:
	Grafo();
	~Grafo();
	bool es_Dirigido(){return is_Dir;};

	void Definir_NVertices(int n);
	
	void Definir_Dirigido(bool is);
	
	void Agregar_Vecino(int prin, int vec);		//Agrega el vecino vec al vertice prin

	void Imprimir();
};	
