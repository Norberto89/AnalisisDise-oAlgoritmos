#include <iostream>
#include <fstream>
#include <math.h>
#include <random>
#include <set>

using namespace std;

int main(int argv, char* argc[]){
	string tmp_pth = argc[1];
	ofstream ofile(tmp_pth.c_str(),ios::out);
	ofile << "Directed: si"<<endl; 

	int n=atoi(argc[2]), min_flow = atoi(argc[3]), max_flow = atoi(argc[4]);
	int cap;
	set<int> vis;
	int compl_edges=round((double)(n-1)/2.0);
	//cout<<compl_edges<<endl;
	int neig,newne;
	ofile << "Nnodes: " << n << endl;

	default_random_engine generator;
	uniform_int_distribution<int> distribution(min_flow, max_flow);
	uniform_int_distribution<int> distribution2(floor((double)compl_edges-0.1*(double)compl_edges), ceil((double)compl_edges + 0.1*(double)compl_edges));
	//cout << floor((double)compl_edges-0.1*(double)compl_edges) << "\t" << ceil((double)compl_edges + 0.1*(double)compl_edges) << endl;
	uniform_int_distribution<int> distribution3(0,n-1);

	for(int i=0;i<n; i++){
		neig = distribution2(generator);
		vis.insert(i);
		while(vis.size()-1<neig){
			newne = distribution3(generator);
			while(vis.find(newne)!=vis.end()){
				newne=distribution3(generator);
			}
			cap = distribution(generator);
			ofile << i << "\t" << newne << "\t" << cap << endl;
			vis.insert(newne);
		}
		vis.clear();
	}
	
	ofile.close();
	cout << "Instancia: " << tmp_pth << " generada correctamente." << endl;
	return 0;
}
