#include <iostream>
#include <fstream>
#include <set>
#include <list>
#include <random>
#include <vector>
#include <stdlib.h>

using namespace std;

class data_sat{
private:
	bool *_vars;
	vector <int> _vio_vars;
	vector<int> *_conds;
	int _nVars;
	int _nConds;
public:
	data_sat(string pth);
	~data_sat();
	void set_var(int ind);
	bool check_feasibility();
	int select_node(set<int> Ntaboo_list);
	int get_nVars(){return _nVars;};
};
