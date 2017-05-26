
#include "leer_arc.h"

leer_arc::leer_arc(string pth, Grafo *gr){
	ifstream _FileA(pth.c_str(),ios::in);
	string tmp;
	int i, j, cap;
	_gr=gr;
	bool ind =false;
	if(!_FileA.good()){
		cout << "No se puede abrir el archivo: " << pth << endl;
		exit(0);
	}
	
	_FileA >> tmp;
	_FileA >> tmp;
	if(tmp == "si"){
		_gr->Definir_Dirigido(true);
	}else{
		_gr->Definir_Dirigido(false);
	}
	_FileA >> tmp;
	int n;
	_FileA >> n;
	_gr-> Definir_NVertices(n);
	while(!_FileA.eof()){
		_FileA>>i;
		_FileA >>j;
		_FileA >> cap;
		if(_FileA.eof()){
			break;
		}
		_gr->Agregar_VecinoC(i,j,cap);

	}
	
	cout << "Archivo: " << pth << " leido correctamente." << endl;
	_FileA.close();
	tmp.clear();
}

leer_arc::~leer_arc(){
}
