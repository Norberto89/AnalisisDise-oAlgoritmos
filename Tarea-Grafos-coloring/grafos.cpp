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
		if(_Vertices[i].Neighbors.size()==0){
			cout << "Verice " << i << " sin arcos de salida." << endl;		
		}else{
			for (auto el : _Vertices[i].Neighbors){
				cout << "(" << i << "," << el << ")" << "\t"; 
			}
			cout << endl;
		}
	}
}

void Grafo::bfs(int verIn){
	BFS_ver verT,verT2;
	list<BFS_ver> nVis;
	//set<BFS_ver> vis;
	
	verT.id=verIn;
	verT.level=0;

	nVis.push_back(verT);
	while(!nVis.empty()){
		verT=*nVis.begin();
		_bfs.insert(verT);
		nVis.pop_front();
		if(_Vertices[verT.id].Neighbors.size()!=0){
			for (auto el:_Vertices[verT.id].Neighbors){
				verT2.id=el;
				verT2.level=verT.level+1;
				if(_bfs.find(verT2)==_bfs.end()){
					nVis.push_back(verT2);	
				}
			}
		}
	}
	
	for(auto el:_bfs){
		cout << el.id << "\t" << el.level << endl;
	}

}

void Grafo::dfs(int verIn){
	DFS_ver verT,verT2;
	list<DFS_ver> nVis;
	//set<DFS_ver> vis;

	verT.id = verIn;
	verT.parent = -1;
	
	nVis.push_back(verT);
	
	while(!nVis.empty()){
		verT=nVis.back();
		_dfs.insert(verT);
		nVis.pop_back();
		if(_Vertices[verT.id].Neighbors.size()!=0){
			for (auto el:_Vertices[verT.id].Neighbors){
				verT2.id=el;
				verT2.parent=verT.id;
				if(_dfs.find(verT2)==_dfs.end()){
					nVis.push_back(verT2);	
				}
			}			
		}
	}	
	for(auto el:_dfs){
		cout << el.id << "\t" << el.parent << endl;
	}
}

bool Grafo::is_2col(){
	set<int> nLa;
	int *lab;
	
	lab=new int [_nV];
	for(int i=0;i<_nV;i++){
		nLa.insert(i);
	}
	
	while(!nLa.empty()){
		_bfs.clear();
		bfs(*nLa.begin());
		for (auto el:_bfs){
			lab[el.id]=el.level%2;
			nLa.erase(el.id);
		}
	}

	for(int i=0;i<_nV;i++){
		for(auto el:_Vertices[i].Neighbors){
			if(lab[i]==lab[el]){
				return false;
			}
		}
	}
	return true;
}


