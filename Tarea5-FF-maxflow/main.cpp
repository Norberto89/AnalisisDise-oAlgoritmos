/* Autores: Pamela Jocelyn Palomo Martínez
		Norberto Alejandro Hernández Leandro*/
#include "leer_arc.h"

int main(int argc, char* argv[]){
	srand(time(NULL));
	if(argc!=4){
		cout << "Entrada inválida, revisa tus argumentos." << endl;
		exit(0);
	}
	string fpath = argv[1];
	int source = atoi(argv[2]);
	int sink = atoi(argv[3]);

	
	Grafo *_gr = new Grafo();
	leer_arc *arch = new leer_arc(fpath,_gr);
	
	_gr->FF(source, sink);	

	delete _gr;
	delete arch;
 	return 0;
}
