#include "leer_arc.h"

int main(int argc, char* argv[]){
	if(argc!=2){
		cout << "Entrada inválida, revisa tus argumentos." << endl;
		exit(0);
	}
	string fpath = argv[1];

	
	Grafo *_gr = new Grafo();
	leer_arc *arch = new leer_arc(fpath,_gr);
	
	_gr->Imprimir();

	bool ind=_gr->es_bipartito();	
	
	if(ind){
		cout << "El grafo es Bipartito." << endl;
	}else{
		cout << "El grafo no es Bipartito." << endl;
	}
	
	delete _gr;
	delete arch;
 	return 0;
}
