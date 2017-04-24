#include "Arbol_Binario.h"

int main(int argv, char* argc[]){
	
	string cadena;
	string temp;
	for(int i=1; i < argv; i++){
		temp = argc[i];
		cadena += temp;
		cadena += " ";
		temp.clear();		
	}
	
	stringstream lin;
	lin.str(cadena);
	Arbol_Binario *Arbol;
	int key;
	

	lin >> key;
	nodo root;
	root.key=key;
	root.left_child = NULL;
	root.right_child = NULL;
	
	Arbol = new Arbol_Binario(&root);
	cout << "raiz" << root.key << endl;
	while (!lin.eof()){
		nodo* nNo = (nodo*)malloc(sizeof(struct nodo));
		lin >> key;
		nNo->key=key;
		nNo->left_child = NULL;
		nNo->right_child = NULL;
		cout << "nodo" << key << endl;
		Arbol->insert(nNo);
		getchar();
	}
	lin.clear();	

	//Arbol->print();
	
	return 0;
}
