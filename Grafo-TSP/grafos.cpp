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
	_dist = new double *[_nV];
	for(int i=0;i<_nV;i++){
		_dist[i] = new double[_nV];
		for(int j=0; j<_nV;j++){
			_dist[i][j]=0.0;
		}
	}
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
	set<BFS_ver> vis;
	
	verT.id=verIn;
	verT.level=0;

	nVis.push_back(verT);
	while(!nVis.empty()){
		verT=*nVis.begin();
		vis.insert(verT);
		nVis.pop_front();
		if(_Vertices[verT.id].Neighbors.size()!=0){
			for (auto el:_Vertices[verT.id].Neighbors){
				verT2.id=el;
				verT2.level=verT.level+1;
				if(vis.find(verT2)==vis.end()){
					nVis.push_back(verT2);	
				}
			}
		}
	}
	
	for(auto el:vis){
		cout << el.id << "\t" << el.level << endl;
	}

}

void Grafo::dfs(int verIn){
	DFS_ver verT,verT2;
	list<DFS_ver> nVis;
	set<DFS_ver> vis;

	verT.id = verIn;
	verT.parent = -1;
	
	nVis.push_back(verT);
	
	while(!nVis.empty()){
		verT=nVis.back();
		vis.insert(verT);
		nVis.pop_back();
		if(_Vertices[verT.id].Neighbors.size()!=0){
			for (auto el:_Vertices[verT.id].Neighbors){
				verT2.id=el;
				verT2.parent=verT.id;
				if(vis.find(verT2)==vis.end()){
					nVis.push_back(verT2);	
				}
			}			
		}
	}	
	for(auto el:vis){
		cout << el.id << "\t" << el.parent << endl;
	}
}

void Grafo::Definir_Distancia(int  i, int j, double dist){
	_dist[i][j]=dist;
	Arco ar;
	ar.x=i;
	ar.y=j;
	ar.dist=dist;
	Arcos_cand.push_back(ar);
}

void Grafo::generar_MST(){
	sort(Arcos_cand.begin(),Arcos_cand.end());
	
	int count=0;
	int *arr=new int [_nV];
	int min,max;
	MST_vert=new Vertice[_nV];
	for(int i=0;i<_nV; i++){
		arr[i]=i;
	}
	while(Arcos_cand.size()>0){
		count=0;
		Arco ar = Arcos_cand.back();
		Arcos_cand.pop_back();
		
		if(arr[ar.x]!=arr[ar.y]){
			cout << ar.x << " " << ar.y << endl;
			MST_vert[ar.x].Neighbors.insert(ar.y);
			MST_vert[ar.y].Neighbors.insert(ar.x);			
			MST.push_back(ar);
			min = (arr[ar.x]<arr[ar.y])?arr[ar.x]:arr[ar.y];
			max = (arr[ar.x]>arr[ar.y])?arr[ar.x]:arr[ar.y];
			
			
			for(int i=0;i<_nV;i++){
				
				if(arr[i]==max){
					arr[i]=min;
					count++;
				}else if(arr[i]==min){
					count++;
				}
			}
			
 		}
		if(count>=_nV){
			break;
		}
	}
}

void Grafo::dfs_mst(int verIn){
	MST_cost=0.0;
	DFS_ver verT,verT2;
	list<DFS_ver> nVis;
	set<DFS_ver> vis;
	list<int> visited;	
	verT.id = verIn;
	verT.parent = -1;
	
	nVis.push_back(verT);
	
	while(!nVis.empty()){
		verT=nVis.back();
		vis.insert(verT);
		visited.push_back(verT.id);
		nVis.pop_back();
		if(MST_vert[verT.id].Neighbors.size()!=0){
			for (auto el:MST_vert[verT.id].Neighbors){
				verT2.id=el;
				verT2.parent=verT.id;
				if(vis.find(verT2)==vis.end()){
					nVis.push_back(verT2);	
				}
			}			
		}
	}	
	cout << endl;
	MST_cost+=_dist[visited.back()][visited.front()];
	for(list<int>::iterator it=visited.begin();it!=visited.end();it++){
		list<int>::iterator it2=it;
		it2++;
		if(it2!=visited.end()){
			MST_cost+=_dist[*it][*it2];
		}
		
	}
	for(auto el:visited){
		
		cout << el << endl;
	}

	cout << "Costo: "<< MST_cost << endl;
}

