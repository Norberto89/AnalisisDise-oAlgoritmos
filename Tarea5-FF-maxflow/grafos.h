#pragma once
#include <iostream>
#include <climits>
#include <algorithm>
#include <stdlib.h>
#include <stdio.h>
#include <list>
#include <vector>
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

struct BFSFF_ver{
	int id;
	int parent;
	int level;
	int cap;

	bool operator==(const BFSFF_ver& rhs) const{
		return id==rhs.id;
    	}

	bool operator<(const BFSFF_ver& rhs) const{
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
	double dist;
	bool operator==(const Arco& rhs) const{
		return dist==rhs.dist;
    	}

	bool operator<(const Arco& rhs) const{
		return dist>rhs.dist; 
	}
};

struct NegC{
	int id;
	double cap;
	bool operator==(const NegC& rhs) const{
		return cap==rhs.cap;
    	}

	bool operator<(const NegC& rhs) const{
		return cap<rhs.cap; 
	}
};

class Grafo{

private:
	int _nV;
	double MST_cost;
	Vertice* _Vertices;
	vector<NegC>* _neigC;
	set<int>* _Adyac;
	list<Arco> _Arcos;
	bool is_Dir;
	double** _dist;
	vector<Arco> Arcos_cand;
	list<Arco> MST;
	Vertice *MST_vert;
	int *BFS_FF;
	int *BFS_FFL;
	int FF_cap;
	int Total_Flow;
	void Agregar_VecinoC2(int prin, int vec, int cap);

public:
	Grafo();
	~Grafo();
	bool es_Dirigido(){return is_Dir;};

	void generar_MST();
	
	int get_Nver(){return _nV;};

	void Definir_NVertices(int n);
	
	void Definir_Dirigido(bool is);
	
	void Definir_Distancia(int  i, int j, double dist);	

	void bfs(int verIn);
	
	void dfs(int verIn);
	void dfs_mst(int verIn);
	
	void Agregar_VecinoC(int prin, int vec, int cap);
	
	void Agregar_Vecino(int prin, int vec);		//Agrega el vecino vec al vertice prin

	void FF(int source, int sink);
	bool bfs_FF(int verIn, int ver_fin);

	void Imprimir();
};	
