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

	//_gr->bfs(0);

	cout<<endl<<endl;
	
	//_gr->dfs(0);		

	bool indic=_gr->is_2col();
	
	cout<<indic<<endl;
	delete _gr;
	delete arch;
 	return 0;
}
