#include <iostream>
#include <string>
#include <fstream>
#include <sstream>


using namespace std;

struct nodo{
	int key;
	nodo *left_child;
	nodo *right_child;
};

class Arbol_Binario{	
private:
	nodo *root;
	
public:
	Arbol_Binario(nodo *rt);
	~Arbol_Binario();
	
	void crear_root(int nr);
	
	bool insert_nodo(nodo *nno, nodo *nod);
	
	bool find_element(int n, nodo *nod);
	
	bool insert(nodo *nno);
	


	void print();
	void print(nodo *nd);
};
