//============================================================================
// Name        : min_cut.cpp
// Author      : Pamela Jocelyn Palomo Martinez/ Norberto Alejandro Hernández Leandro
//============================================================================

#include <list>
#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;

struct Arco{
	int nodo_destino;
	Arco(int n){nodo_destino=n;}
	bool operator==(int n){return nodo_destino==n;}
};

struct Nodo{
	int id;
	list<Arco> arcos_incidentes;
	Nodo(int i){id=i;}
	bool operator==(int i){return id==i;}
	bool operator<(Nodo n){return id<n.id;}
};

list<Nodo> generarGrafo(string archivo_entrada){
	cout<<archivo_entrada<<endl;
	list<Nodo>grafo;

	ifstream datos_entrada;
	datos_entrada.open(archivo_entrada.c_str());

	while(true){
		int nodo_origen, nodo_destino;
		datos_entrada>>nodo_origen>>nodo_destino;
		nodo_origen--;
		nodo_destino--;

		if(datos_entrada.eof()) break;

		list<Nodo>::iterator itg=find(grafo.begin(),grafo.end(),nodo_origen);
		if(itg==grafo.end()){
			//El nodo origen no ha sido agregado previamente. Agregarlo al grafo.
			Nodo nuevo_nodo(nodo_origen);
			nuevo_nodo.arcos_incidentes.push_back(Arco(nodo_destino));
			grafo.push_back(nuevo_nodo);
		}
		else{
			//El nodo origen ya se encuentra en el grafo. Agregar arco.
			itg->arcos_incidentes.push_back(Arco(nodo_destino));
		}

		itg=find(grafo.begin(),grafo.end(),nodo_destino);
		if(itg==grafo.end()){
			//El nodo destino no ha sido agregado previamente. Agregarlo al grafo.
			Nodo nuevo_nodo(nodo_destino);
			nuevo_nodo.arcos_incidentes.push_back(Arco(nodo_origen));
			grafo.push_back(nuevo_nodo);
		}
		else{
			//El nodo destino ya se encuentra en el grafo. Agregar arco.
			itg->arcos_incidentes.push_back(Arco(nodo_origen));
		}

	}
	datos_entrada.close();
	grafo.sort();
	return grafo;
}


void imprimirGrafo(list<Nodo> grafo){
	cout<<"Arcos: (origen,destino,peso)"<<endl;
	for(list<Nodo>::iterator it=grafo.begin();it!=grafo.end();it++){
		cout<<"Nodo: "<<it->id<<endl;
		list<Arco> arcos_incidentes=it->arcos_incidentes;
		for(list<Arco>::iterator ita=arcos_incidentes.begin();ita!=arcos_incidentes.end();ita++){
			cout<<"("<<it->id<<","<<ita->nodo_destino<<","<<")"<<endl;
		}
		cout<<endl;
	}
}

pair<int,int> arista_random(list<Nodo> grafo){
	pair<int,int> arco;

	list<Nodo>::iterator it=grafo.begin();
	advance(it,rand()%grafo.size());
	Nodo n=*it;
	arco.first=n.id;

	list<Arco>::iterator it2=n.arcos_incidentes.begin();
	advance(it2,rand()%n.arcos_incidentes.size());
	Arco a=*it2;
	arco.second=a.nodo_destino;

	return arco;
}



int main(int argc, char** argv) {
	srand(time(NULL));

	list<Nodo> grafo=generarGrafo(argv[1]);

	int sets[grafo.size()];

	for(int i=0;i<grafo.size();i++) sets[i]=i;

	int n_sets=grafo.size();

	while(n_sets>2){
		//Seleccionar una arista al azar
		pair<int,int> rarco=arista_random(grafo);

		//Unir
		int minimo=min(sets[rarco.first],sets[rarco.second]);
		int maximo=max(sets[rarco.first],sets[rarco.second]);

		if(minimo!=maximo){
			for(int i=0;i<grafo.size();i++){
				if(sets[i]==maximo) sets[i]=minimo;
			}
			n_sets--;
		}


		}

	int n_arcos=0;
	for(list<Nodo>::iterator it=grafo.begin();it!=grafo.end();it++){
		Nodo n=*it;
		for(list<Arco>::iterator it2=n.arcos_incidentes.begin();it2!=n.arcos_incidentes.end();it2++){
			Arco a=*it2;
			if(sets[n.id]!=sets[a.nodo_destino]) n_arcos++;
		}
	}
	n_arcos/=2;

	for(int i=0;i<grafo.size();i++){
		cout<<i<<" "<<sets[i]<<endl;
	}
	cout<<n_arcos<<endl;

	return 0;
}

