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
	_neigC = new vector<NegC>[n]; 
	_Adyac = new set<int>[n];
	_dist = new double *[_nV];
	BFS_FF = new int[_nV];
	BFS_FFL = new int[_nV];
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

void Grafo::Agregar_VecinoC(int prin, int vec, int cap){
	NegC tmp;
	tmp.id=vec;
	tmp.cap=cap;
	_neigC[prin].push_back(tmp);
	push_heap(_neigC[prin].begin(),_neigC[prin].end());	
	

}
void Grafo::Agregar_VecinoC2(int prin, int vec, int cap){
	
	bool indi = false;
	for(vector<NegC>::iterator it=_neigC[prin].begin(); it!= _neigC[prin].end(); ++it){
		if(it->id == vec){
			it->cap+=FF_cap;
			indi=true;
			break;				
		}

	}
	if(!indi){
		NegC tmp;
		tmp.id=vec;
		tmp.cap=cap;
		_neigC[prin].push_back(tmp);
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


bool Grafo::bfs_FF(int verIn, int ver_Fin){
	BFSFF_ver verT,verT2;
	list<BFSFF_ver> nVis;
	set<BFSFF_ver> vis;
	set<BFSFF_ver> labeled;
	verT.id = verIn;
	verT.level = 0;
	verT.parent = -1;
	verT.cap = INT_MAX;
	
	nVis.push_back(verT);
	BFS_FF[verIn]=-1;
	BFS_FFL[verIn]=0;
 	int capacity;


	
	while(!nVis.empty()){
		verT=*nVis.begin();
		vis.insert(verT);
		labeled.insert(verT);
		nVis.pop_front();
		if(_neigC[verT.id].size()!=0){
			sort_heap(_neigC[verT.id].begin(), _neigC[verT.id].end());
			for (auto el:_neigC[verT.id]){
				capacity = (verT.cap <= el.cap)?verT.cap:el.cap;
				verT2.id=el.id;
				verT2.cap= capacity;
				verT2.level=verT.level+1;
				verT2.parent=verT.id;
				
				if(vis.find(verT2)==vis.end() && labeled.find(verT2)==labeled.end()){
					nVis.push_back(verT2);
					BFS_FF[verT2.id]=verT.id;
					BFS_FFL[verT2.id]=verT2.level;
					labeled.insert(verT2);
					if(verT2.id==ver_Fin){
						FF_cap=verT2.cap;
						return true;	
					}	
				}
			}
		}
	}
	
	nVis.clear();
	vis.clear();
	return false;
}


void Grafo::FF(int source, int sink){
	int a,b;
	Total_Flow=0;
	int lev;
	int count =1;
	
	while(bfs_FF(source, sink)){
		a=BFS_FF[sink];
		b=sink;
		lev=BFS_FFL[b];
		cout << endl << endl;
		cout << "Envio " << count <<":" << endl;
		cout << "Capacidad de flujo: "<<FF_cap<< endl;
		cout << "Arcos: " << endl;
		while(a!=-1){
			cout << a << "\t" << b << endl;
			for(vector<NegC>::iterator it=_neigC[a].begin(); it!= _neigC[a].end(); ++it){
				if(it->id == b){
					
					if(lev==BFS_FFL[a]+1){
						
						it->cap=it->cap-FF_cap;
						if(it->cap == 0){
							_neigC[a].erase(it);
						}
						Agregar_VecinoC2(b,a,FF_cap);
						lev--;
						break;
					}				
				}
			}
			b=a;
			a=BFS_FF[b];
			if(a==-1){
				break;
			}
		}
		
		for(int i=0;i<_nV;i++){
			make_heap(_neigC[i].begin(),_neigC[i].end());	
		}

		Total_Flow+=FF_cap;
		count++;
		cout << "____________________________________" << endl;		
	}
	cout<< "Max Flow: " << Total_Flow << endl;
			
}
