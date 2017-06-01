#include "GrammarWTW.h"

#include <stdio.h>
#include <stdlib.h>

#include <assert.h>
#include <map>
#include <stack>
#include <fstream>

GrammarWTW* readGrammar(istream& input) { 
	GrammarWTW* g = new GrammarWTW;
	while (input.peek() == '#')
		while (input.get() != '\n');
	
	input >> g->nbTerminals; 
	input >> g->nbNonTerminals; 
	input >> g->nbProductions; 
	g->productions = new Production[g->nbProductions];
	
	for (int i = 0; i < g->nbProductions; i++) { 
		//char c = input.peek();
		while (input.peek() == '#' || input.peek() == '\n')
			while (input.get() != '\n');
		input >> g->productions[i].cost; 
		input >> g->productions[i].minSpan;
		input >> g->productions[i].maxSpan;
		assert(g->productions[i].minSpan <= g->productions[i].maxSpan);
		input >> g->productions[i].length; assert(g->productions[i].length<=2);
		input >> g->productions[i].left; 
		assert(g->nbTerminals <= g->productions[i].left && g->productions[i].left < g->nbTerminals + g->nbNonTerminals);
		g->productions[i].right = new int[g->productions[i].length];
		for (int j = 0; j < g->productions[i].length; j++){
			input >> g->productions[i].right[j];}
		input >> g->productions[i].tws; 
		input >> g->productions[i].twl; //assert((g->productions[i].tws - 1) + g->productions[i].twl <= g->sequenceLength);
//		if(g->productions[i].length==2){
//			if(g->productions[i].right[0]!=g->productions[i].right[1]){
//				if(g->productions[i].cost==0.0){
//					cout << "Error dans le graphe: "<<g->productions[i].left<<"->"<<g->productions[i].right[0]<<" "<<g->productions[i].right[1]<<endl;
//					exit(0);
//				}
//			}
//			else if(g->productions[i].cost>0.0)
//			{
//				cout << "Error dans le graphe*: "<<g->productions[i].left<<"->"<<g->productions[i].right[0]<<" "<<g->productions[i].right[1]<<endl;
//				exit(0);
//			}
//		}
	}
	
//	while (input.peek() == '\n')
//		while (input.get() != '\n');
//	string test;
//
//	if(!input.eof()){
//		
//		input >> test;
//		cout << test<<endl;
//	}
	return g;
}

void freeGrammar(GrammarWTW*& g) {
	while (g->nbProductions--)
		delete g->productions[g->nbProductions].right;
	delete g->productions;
	delete g;
	g = NULL;
    cout<<"Grammar Freed"<<endl;
}

// Create a new grammar in the normal Chomsky form accepting the same language as g
GrammarWTW* convertToChomsky(const GrammarWTW& g) {
	assert(false);		// Not implemented yet
	return NULL;
}

// Return true if n is a power of two. Zero is considered as a power
// of two since 2^{-infty} = 0 (ok, it is a bit of a stretch but I
// need it in my algo).
bool isPowerOfTwo(unsigned int n) {
	static const unsigned int bitCount[16] = {0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4};
	unsigned int buf;
	do {
		buf = n & 0x0000000F;
		n >>= 8;
	}
	while (n != 0 && buf == 0);
	assert(0 <= buf && buf < 16);
	return bitCount[buf] <= 1 && n == 0;
}

template<class T>
T* getNew(T*& array, int& count) {
	assert(count >= 0);
	assert((array != NULL) ^ (count == 0));
	if (array == NULL)
		array = (T*)malloc(sizeof(T));
	else if (isPowerOfTwo(count))
		array = (T*)realloc(array, sizeof(T) * (count * 2));
	assert(array != NULL);
	
	return array + count++;
}

// Compute the position in the table where the OrNode of the character c produced at position i on a span of j
inline int idx(const GrammarWTW& g, int sequenceLength, int i, int j, int c) {
	assert(sequenceLength > 0);
	assert(0 <= i && i < sequenceLength);
	assert(0 <= i + j && i + j < sequenceLength);
	assert(0 <= c && c < g.nbTerminals + g.nbNonTerminals);
	
	int a = sequenceLength - j;
	int res = (a * (a - 1) / 2 + i) * (g.nbNonTerminals + g.nbTerminals) + c;
	
	// The resulting index must fit with the size of the table
	assert(0 <= res && res < (sequenceLength + 1) * sequenceLength * (g.nbTerminals + g.nbNonTerminals) / 2);
	return res;
}

// Assert: Grammer g is in its normal Chomsky form
GrammarGraph* constructCYKGraph(const int sequenceLength, const GrammarWTW& g, ProductionConstraint* pc) {
	// assert(pc == NULL); // There seems to be some bugs with that option. I disable it for now.
	
	GrammarGraph* gr = new GrammarGraph;
	gr->nbLeaves = gr->nbOrNodes = 0;
	gr->postOrderNodes = NULL;
	gr->sequenceLength = sequenceLength;
	
	
	// Construct the leaf nodes
	const int tableSize = sequenceLength * (sequenceLength + 1) * (g.nbTerminals + g.nbNonTerminals) / 2;
	
	//cout << "\n Table Size: "<< tableSize;
	//getchar();
	
	OrNode** table = new OrNode*[tableSize];
	for (int i = 0; i < tableSize; i++)
		table[i] = NULL;
	for (int i = 0; i < sequenceLength; i++)
		for (int c = 0; c < g.nbTerminals; c++) { 
			OrNode* n = new OrNode;
			n->nbChildren = n->nbAncestors = 0;
			n->ancestors = NULL;
			n->cost = 0.0;
			n->leaf.terminal = c;
			n->leaf.position = i;
			n->position = i;
			n->span = 1;
			n->id = 0;
			table[idx(g, sequenceLength, i, 0, c)] = n;
		}
	
	// Process the productions of the form Non-Terminal -> Terminal
	for (int i = 0; i < sequenceLength; i++) /*position dans la séquence*/
		for (Production* p = g.productions + g.nbProductions - 1; p >= g.productions; p--) /*scan les productions*/
			if ((p->length == 1) && ((pc == NULL) || pc->hasPermission(p, i, 1, 0))) {
				/*Valider si une production est dans la time window*/
				if(i >= p->tws && i < p->tws + p->twl){
					OrNode* parent = table[idx(g, sequenceLength, i, 0, p->left)];
					if (parent == NULL) {// If the parent does not exist, create it and add it to the table
						table[idx(g, sequenceLength, i, 0, p->left)] = parent = new OrNode;
						parent->cost = 0.0;
						parent->nbChildren = 0;
						parent->nbAncestors = 0;
						parent->ancestors = NULL;
						parent->inner.children = NULL;
						parent->inner.succ = NULL;
						parent->id = 0;
						parent->position = i;
						parent->span = 1;
					}
					assert(parent != NULL);
					
					// Retrieve the child node
					OrNode* child = table[idx(g, sequenceLength, i, 0, p->right[0])];
					assert(child != NULL); // The child is a terminal and it should have been added in the prevous for loop
					
					// Create an and node among the children of the parent
					AndNode* newAndNode = *getNew(parent->inner.children, parent->nbChildren) = new AndNode;
					newAndNode->parent = parent;
					newAndNode->leftChild = child;
					newAndNode->rightChild = NULL;
					newAndNode->production = p;
					newAndNode->id = 0; //if(newAndNode->production->cost >= 10){cout << newAndNode->production->cost << " ";}
					*getNew(child->ancestors, child->nbAncestors) = newAndNode;
				}
			}
	
	// j: span of the rule - 1
	// prod: Production
	// i: Beginning of the production
	// k: The first half of the rule spans form i to i + k - 1 and the second half spans from i = k to i + j - 1
	for (int j = 1; j < sequenceLength; j++) /*longueur de la sous-séquence générée par la production*/
		for (Production* prod = g.productions + g.nbProductions - 1; prod >= g.productions; prod--) 
			if (prod->length == 2 && prod->minSpan - 1 <= j && j < prod->maxSpan) // If we can apply the rule : dans le span de la prod?
				for (int i = 0; i + j < sequenceLength; i++) {/*position de la sous-séquence générée par la production*/
					/*Vérifier ici si la production génère une sous séquence dans le span*/
					if(i >= prod->tws && i + j < prod->tws + prod->twl){
						OrNode* orNode = table[idx(g, sequenceLength, i, j, prod->left)];
						
						for (int k = 0; k < j; k++) {
							assert(idx(g, sequenceLength, i, j, prod->left) < idx(g, sequenceLength, i, k, prod->right[0]));
							assert(idx(g, sequenceLength, i, j, prod->left) < idx(g, sequenceLength, i + k + 1, j - k - 1, prod->right[1]));
							
							if (pc == NULL || pc->hasPermission(prod, i, j + 1, k + 1)) { //cout << "i j k " << i << " " << j << " "<< k << endl;
								/*Vérifier ici si les deux non-terminaux sont dans leur time window???*/
								OrNode* left = table[idx(g, sequenceLength, i, k, prod->right[0])]; //if(left!=NULL){cout << "l " << prod->right[0] << endl;}else{cout << "l " << prod->right[0] << " NULL" << endl;}
								OrNode* right = table[idx(g, sequenceLength, i + k + 1, j - k - 1, prod->right[1])]; //if(right!=NULL){cout << "r " << prod->right[1] << endl;}else{cout << "r " << prod->right[1] << " NULL" << endl;}
								/*n'est-ce pas la vérification??*/
								if (left != NULL && right != NULL) {
									// Let prod be A -> BC. Create the or-node associated to
									// A at starting at position i with span of j.
									if (orNode == NULL) {
										orNode = table[idx(g, sequenceLength, i, j, prod->left)] = new OrNode;
										orNode->nbChildren = 0;
										orNode->inner.children = NULL;
										orNode->inner.succ = NULL;
										orNode->ancestors = NULL;
										orNode->nbAncestors = 0;
										orNode->cost = 0.0;
										orNode->id = 0;
										orNode->position = i;
										orNode->span = j + 1;
									}
									
									// Let prod be A -> BC. We create an and-node with left child B and right child C.
									AndNode* andNode = *getNew(orNode->inner.children, orNode->nbChildren) = new AndNode;
									andNode->id = 0;
									andNode->parent = orNode;
									andNode->leftChild = left;
									andNode->rightChild = right;
									andNode->production = prod;  
									*getNew(left->ancestors, left->nbAncestors) = andNode;
									*getNew(right->ancestors, right->nbAncestors) = andNode;
									
									assert(orNode->inner.children[orNode->nbChildren - 1] == andNode);
									assert(andNode->parent == orNode);
									assert(andNode->leftChild->ancestors[andNode->leftChild->nbAncestors - 1] == andNode);
									assert(andNode->rightChild->ancestors[andNode->rightChild->nbAncestors - 1] == andNode);
								}
							}
						}
					}
				}
	
	//map<int, OrNode*>::iterator rootIt = table.find((g.nbTerminals * sequenceLength + 1) * sequenceLength - 1);
	gr->root = table[idx(g, sequenceLength, 0, sequenceLength - 1, g.nbTerminals)];
	assert(gr->root != NULL); // There must be at lest one valid parsing tree
	
	// Traverse the tree from the root of the graph
	//stack<OrNode*> s;
	OrNode** s = new OrNode*[2 * sequenceLength];
	int top = 0;
	//  s.push(gr->root);
	s[top] = gr->root;
	gr->root->cost = 1.0;
	
	gr->nbOrNodes = 0;
	//while (!s.empty()) {
	while (top >= 0) {
		//OrNode* n = s.top();
		OrNode* n = s[top];
		assert (n->cost == 1.0);
		if (n->id < n->nbChildren) {
			AndNode* c = n->inner.children[(n->id)++];
			assert(c->rightChild == NULL || c->rightChild->cost != 1.0); // The graph is acyclic
			if ((c->rightChild != NULL) && (c->rightChild->cost == 0.0)) {
				c->rightChild->cost = 1.0; // Tag the node as visited
				s[++top] = c->rightChild;
				assert(top < 2 * sequenceLength);
				assert(c->rightChild->id == 0); // If the cost was null, then no id has been affected yet
			}
			assert(c->leftChild != NULL);
			assert(c->leftChild->cost != 1.0); // The graph is acyclic
			if (c->leftChild->cost == 0.0) {
				c->leftChild->cost = 1.0; // Tag the node as visisted
				s[++top] = c->leftChild;
				assert(top < 2 * sequenceLength);
				assert(c->leftChild->id == 0); // If the cost was null, then no id has been affected yet
			}
		}
		else {
			n->cost = 2.0;
			top--;
			n->id = gr->nbOrNodes;
			*getNew(gr->postOrderNodes, gr->nbOrNodes) = n;
			for (int i = 0; i < n->nbChildren; i++) { // Debug
				assert(n->inner.children[i]->leftChild->cost == 2.0);
				assert((n->inner.children[i]->rightChild == NULL) || (n->inner.children[i]->rightChild->cost == 2.0));
			}
		}
	}
	delete [] s;
	
	// Remove all ancestors that cannot be part of a valid parsing tree
	for (int i = 0; i < tableSize; i++) {
		OrNode* n = table[i];
		if (n != NULL && n->cost > 0.0)  // If the node cannot be part of a valid parsing tree
			for (int i = n->nbAncestors - 1; i >= 0; i--) 
				if (n->ancestors[i]->parent->cost == 0.0) 
					n->ancestors[i] = n->ancestors[--(n->nbAncestors)];
	}
	// Delete nodes that have been removed from the graph
//	for (int i = 0; i < tableSize; i++) {
//		OrNode* n = table[i];
//		if (n != NULL && n->cost == 0.0) {
//			free(n->ancestors);    	// Free the memory associated to this node
//			if (n->nbChildren > 0) {
//				while (n->nbChildren--)
//					delete n->inner.children[n->nbChildren];
//				free(n->inner.children);
//			}
//			delete n;
//		}
//	}
	
	// Give an id to every and-node.
	int nextId = gr->nbOrNodes;
	for (int i = 0; i < gr->nbOrNodes; i++) {
		OrNode* n = gr->postOrderNodes[i];
		for (int j = 0; j < n->nbChildren; j++)
			n->inner.children[j]->id = nextId++;
	}
	gr->nbNodes = nextId;
	
	assert(gr->postOrderNodes[gr->nbOrNodes - 1] == gr->root);
	
	{
		bool* test = new bool[gr->nbNodes];
		for (int i = 0; i < gr->nbNodes; i++)
			test[i] = false;
		for (int i = 0; i < gr->nbOrNodes; i++) {
			OrNode* n = gr->postOrderNodes[i];
			assert(!test[n->id]);
			test[n->id] = true;
			for (int j = 0; j < n->nbChildren; j++) {
				AndNode* an = n->inner.children[j];
				assert(!test[an->id]);
				test[an->id] = true;
				assert(test[an->leftChild->id]);
				assert((an->rightChild == NULL) || (test[an->rightChild->id]));
			}
		}
		
		delete [] test;
	}
	//cout << "Dans grammar nbNodes: " << gr->nbNodes << endl;
	//cout << "Dans grammar nbOrNodes: " << gr->nbOrNodes << endl;
	
	
	delete [] table;
	return gr;
}

void freeGraph(GrammarGraph*& g) {
	assert(g != NULL);
	
	for (int i = 0; i < g->nbOrNodes; i++) {
		OrNode* n = g->postOrderNodes[i];
		if (n->ancestors != NULL)
			free(n->ancestors);
		
		if (n->nbChildren > 0) {
			while (n->nbChildren--)
				delete n->inner.children[n->nbChildren];
			free(n->inner.children);
		}
	}
	
	free(g->postOrderNodes);
	delete g;
	g = NULL;
}

// Generates a random sequence accepted by the grammar. Warning: not all sequences are equiprobable.
// The buffer must be large enough to accept a sequence of gr.sequenceLength characters.
//void generateRandomSequence(const GrammarWTW& g, const GrammarGraph& gr, int* buffer) {
	// const char convertMap[24] = {'r', 'b', 'l', '1', '2', 'S', 'A', 'C', 'X', 'P', 'R', 'Z', 'H', 'K', 'L', 'I', 'N', 'F', 'G', 'J', 'D', 'E', 'M', 'B'};
void generateRandomSequence(const GrammarGraph& gr, int* buffer) {	
	OrNode** stack = new OrNode*[gr.sequenceLength];
	int stackTop = 0;
	stack[stackTop++] = gr.root;
	
	int counter = 0;
	while (stackTop > 0) {
		OrNode* n = stack[--stackTop];
		if (n->nbChildren > 0) {
			const int choice = rand() % n->nbChildren;
			
			/*Production* p = n->inner.children[choice]->production;
			 cout << convertMap[p->left] << " -> ";
			 for (int i = 0; i < p->length; i++)
			 cout << convertMap[p->right[i]] << " ";
			 cout << endl;*/
			
			if (n->inner.children[choice]->rightChild != NULL)
				stack[stackTop++] = n->inner.children[choice]->rightChild;
				stack[stackTop++] = n->inner.children[choice]->leftChild;

		}
		else {
			//assert(n->leaf.terminal < g.nbTerminals);
			buffer[n->leaf.position] = n->leaf.terminal;
			//cout<<buffer[n->leaf.position]<<"\t";
			counter++;
		}
	}
	//cout<<endl;
	
	//assert(counter == gr.sequenceLength);
	delete[] stack;
}

