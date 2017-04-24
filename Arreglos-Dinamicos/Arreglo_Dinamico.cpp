#include "Arreglo_Dinamico.h"

Arreglo_Dinamico::Arreglo_Dinamico(){
	modif = 0;
	_iniSize = 10;
	_arr = new int[10];
}

Arreglo_Dinamico::~Arreglo_Dinamico(){
	delete[] _arr;
}

void Arreglo_Dinamico
