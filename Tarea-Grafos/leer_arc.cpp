#include "leer_arc.h"

leer_arc::leer_arc(string pth, Grafo *gr){
	ifstream _FileA(pth.c_str(),ios::in);
	string tmp;
	_gr=gr;
	if(!_FileA.good()){
		cout << "No se puede abrir el archivo: " << pth << endl;
		exit(0);
	}
	
	
	while (!_FileA.eof()){
		_FileA >> tmp;
		if(tmp == "Dirigido"){
			_FileA >> tmp;
			if(tmp == "si"){
				_gr->Definir_Dirigido(true);
			}else{
				_gr->Definir_Dirigido(false);
			}
		}else if (tmp == "Vertices") {
			int n;
			int numb;
			_FileA >> n;
			_gr->Definir_NVertices(n);
			getline(_FileA,tmp);
			for(int i=0; i<n; i++){
				getline(_FileA,tmp);
				stringstream lin;
				lin.str(tmp);
				while (!lin.eof()){
					lin >> numb;
					_gr->Agregar_Vecino(i,numb);
				}
				lin.clear();
			}
		}
	}
	cout << "Archivo: " << pth << " leido correctamente." << endl;
	_FileA.close();
	tmp.clear();
}

leer_arc::~leer_arc(){
}
