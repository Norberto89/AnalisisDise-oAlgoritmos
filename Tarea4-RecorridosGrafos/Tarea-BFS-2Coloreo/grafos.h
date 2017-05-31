#pragma once
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <list>
#include <set>
#include <math.h>
#include <stdlib.h>

using namespace std;

struct DFS_ver{
	int id;
	int parent;

	bool operator==(const DFS_ver& rhs) const{
		return id==rhs.id;
    	}

	bool operator<(const DFS_ver& rhs) const{
		return id<rhs.id; 
	}
	
};

struct BFS_ver{
	int id;
	int level;

	bool operator==(const BFS_ver& rhs) const{
		return id==rhs.id;
    	}

	bool operator<(const BFS_ver& rhs) const{
		return id<rhs.id; 
	}
};

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
	set<BFS_ver> _bfs;
	set<DFS_ver> _dfs;

public:
	Grafo();
	~Grafo();
	bool es_Dirigido(){return is_Dir;};

	void Definir_NVertices(int n);
	
	void Definir_Dirigido(bool is);
	
	void bfs(int verIn);
	
	void dfs(int verIn);
	
	void Agregar_Vecino(int prin, int vec);		//Agrega el vecino vec al vertice prin
	
	bool is_2col();	
	
	void Imprimir();
};	
