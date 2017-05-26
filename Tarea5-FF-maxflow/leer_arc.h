#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include "grafos.h"

class leer_arc{
private:
	Grafo *_gr;
public:
	leer_arc(string pth, Grafo *gr);
	~leer_arc();

};
