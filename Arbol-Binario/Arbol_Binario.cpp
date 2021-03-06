#include "Arbol_Binario.h"

Arbol_Binario::Arbol_Binario(nodo *rt){
	root = rt;
}

Arbol_Binario::~Arbol_Binario(){

}


bool Arbol_Binario::find_element(int key, nodo *nod){	
	if(nod == NULL){
		return false;
	}	
	if(nod->key ==	key){
		return true;
	}else if (nod->key < key){
		return find_element(key, nod->left_child);
	}else{
		return find_element(key, nod->right_child);
	}
}


bool Arbol_Binario::insert(nodo *nno){
	return 	insert_nodo(nno, root);

}

bool Arbol_Binario::insert_nodo(nodo *nno, nodo *nod){
	cout << "en" << nod->key << "poniendo" << nno-> key << endl;	
	if(nod->key ==	nno->key){
		return false;
	}else if (nod->key > nno->key){
		if (nod->left_child != NULL){
			return insert_nodo(nno, nod->left_child);
		}else {
			nod->left_child = nno;
			cout << "left" << nod->left_child->key << endl;
			return true;
		}	 
	}else{
		if (nod->right_child != NULL){
			return insert_nodo(nno, nod->right_child);
		}else {
			nod->right_child = nno;
			cout << "right" << nod->right_child->key << endl;

			return true;
		}
	}
}

void Arbol_Binario::print(){
	print(root);
}

void Arbol_Binario::print(nodo *nd){
	cout << "Nodo: " << nd->key << endl;
	cout << "LC: " << nd->left_child->key;
	cout << "RC: " << nd->right_child -> key;
	print(nd->left_child);
	print(nd->right_child);
}

