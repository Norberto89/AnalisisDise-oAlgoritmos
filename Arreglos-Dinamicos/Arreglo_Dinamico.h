#include <iostream>

using namespace std;

class Arreglo_Dinamico{
private:
	int _iniSize;
	int *_arr;
	int _modif;

public:
	Arreglo_Dinamico();
	~Arreglo_Dinamico();
	void insertar(int elem);
};
