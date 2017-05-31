#include "data_sat.h"

int main(int argv, char* argc[]){
	string pth = argc[1];
	set<int> Nvisited;
	list<int> tabu_lst;
	int Svar,tabu_counter=0,iter;

	data_sat *_data = new data_sat(pth);

	for(int i=0; i<_data->get_nVars();i++){
		Nvisited.insert(i);
	}
	iter = _data->get_nVars()/2;
	
	while (!_data->check_feasibility()){
		Svar = _data->select_node(Nvisited);
		_data->set_var(Svar);
		Nvisited.erase(Svar);
		tabu_lst.push_back(Svar);
		cout<<Svar<< endl;
		if(tabu_counter>=5){
			Nvisited.insert(tabu_lst.front());
			tabu_lst.pop_front();		
		}
		tabu_counter++;	
	}	
	
	return 0;
}
