#include "data_sat.h"

data_sat::data_sat(string pth){
	ifstream ifile(pth.c_str(),ios::in);
	string temp;
	int tmp_int;

	
	while(!ifile.eof()){
		ifile >> temp;	
		if(ifile.eof()){
			break;
		}
		if(temp=="cnf"){
			ifile >> _nVars;
			ifile >> _nConds;
			_vars = new bool [_nVars];
			_conds = new vector<int> [_nConds];
	
			for(int i=0;i<_nVars;i++){
				_vars[i] = false;
			}

			for(int i=0;i<_nConds;i++){
				ifile >> tmp_int;
				while(tmp_int!=0){
					if(tmp_int<0){
						_conds[i].push_back(tmp_int+1);					
					}else{
						_conds[i].push_back(tmp_int-1);
					}
					ifile>>tmp_int;
				}
			}
		}		
		
	}

	ifile.close();
	
	cout << "Instancia: " << pth << endl;
	cout << "# de variables: " << _nVars << endl;
	cout << "# de condiciones: " << _nConds << endl;
	cout << "Condiciones: " << endl;
	for(int i=0;i<_nConds;i++){
		for(auto el:_conds[i]){
			cout << el << " ";
		}
		cout << endl;
	}
}	

data_sat::~data_sat(){

}

void data_sat::set_var(int ind){
	if(ind >=0  && ind <_nVars){
		_vars[ind] = !_vars[ind];
	}else{
		cout << "Esta variable no existe. " << endl;
	}
}

bool data_sat::check_feasibility(){
		
	bool indic = true;
	int count;
	
	_vio_vars.clear();

	for(int i=0;i<_nConds;i++){
		count=0;
		for(auto el:_conds[i]){
			if(el<0){
				if(!_vars[abs(el)]){
					count++;
				}
			}else{
				if(_vars[el]){
					count++;
				}
			}		
		}
		if(count == 0){
			for(auto el:_conds[i]){
				_vio_vars.push_back(abs(el));
			}
			if(indic){
				indic = false;
			}
		}
	}
	
	return indic;
}

int data_sat::select_node(set<int> Ntabu_list){
	std::default_random_engine generator;
  	std::uniform_int_distribution<int> distribution(0,_vio_vars.size()-1);
	std::uniform_int_distribution<int> distribution2(0,_nVars-1);
	int n=distribution(generator);
	int counter=0;
	
	while(Ntabu_list.find(_vio_vars[n])==Ntabu_list.end() && counter <= _nVars){
		n=distribution(generator);
		counter++;
	}
	if(counter>_nVars){
		n=distribution2(generator);
		while(Ntabu_list.find(n)==Ntabu_list.end()){
			n=distribution2(generator);
		}
		return n;
	}
	
	return _vio_vars[n];
}




