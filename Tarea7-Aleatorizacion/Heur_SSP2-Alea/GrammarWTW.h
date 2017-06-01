#ifndef GRAMMARWTW
#define GRAMMARWTW

#include <iostream>
#include <limits.h>
#include <limits>

using namespace std;

const int INF = std::numeric_limits<int>::max();//0x7FFFFFFF;

typedef struct Production{
  int left;
  int length;
  int* right;		 // There are "length" elements in this vector
  int minSpan;	 // 0 if no minimum
  int maxSpan;	 // INF if no maximum
  int tws;	//beginning of time window
  int twl;
  double cost;   // Cost of using this production
} Production;

typedef struct GrammarWTW{
  int nbProductions;
  Production* productions;
  int nbTerminals;     // Any character c subject to  0 <= c < terminalCount is a terminal character
  int nbNonTerminals;
} GrammarWTW;

typedef struct _AndNode AndNode; // Early declaration

typedef struct _InnerNode {
  AndNode** children;
  AndNode* succ;
} InnerNode;

typedef struct _LeafNode {
  int terminal;
  int position;
} LeafNode;

typedef struct _OrNode {
  int nbAncestors;
  AndNode** ancestors;
  int nbChildren;	 // If the number of children is 0, then it is a terminal node
  double cost;
  union {
    InnerNode inner;
    LeafNode leaf;
  };
  int id;
  int position;
  int span;
} OrNode;

typedef struct _AndNode {
  OrNode* leftChild;
  OrNode* rightChild;
  OrNode* parent;
  Production* production; // Production associated to this node
  int id;
} AndNode;

typedef struct _GrammarGraph {
  OrNode* root;
  int nbLeaves;	   // Number of leaves
  int nbOrNodes;
  int nbNodes;			// Total number of nodes including andNodes
  int sequenceLength;
  OrNode** postOrderNodes; // The first nbLeaves nodes are leaves. Nodes are
			   // sorted in post-order to make the traversal of
			   // the graph in that order easy.
} GrammarGraph;

class ProductionConstraint {
public:
  // Returns true if the production p = A -> BC can be applied at position p,
  // when A is a sequence of length span, and B is a sequence of length subSpan
  virtual bool hasPermission(Production* const p, int position, int span, int subSpan) = 0;
};

GrammarWTW* readGrammar(istream& input);
void freeGrammar(GrammarWTW*& g);
GrammarWTW* convertToChomsky(const GrammarWTW& g); // Create a new grammar in the normal Chomsky form accepting the same language as g

GrammarGraph* constructCYKGraph(int sequenceLength, const GrammarWTW& g, ProductionConstraint* pc);
void freeGraph(GrammarGraph*& g);

// Generates a random sequence accepted by the grammar. Warning: not all sequences are equiprobable.
// The buffer must be large enough to accept a sequence of gr.sequenceLength characters.
//void generateRandomSequence(const GrammarWTW& g, const GrammarGraph& gr, int* buffer);
void generateRandomSequence(const GrammarGraph& gr, int* buffer);
#endif
