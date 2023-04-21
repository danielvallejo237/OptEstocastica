#include "MA.h"
#include <unistd.h>
#include <bits/stdc++.h>

//Problem *Individual::problem;
int main(int argc, char **argv){
	if ((argc - 1) % 3 != 0){ cout << "Error en parametros" << endl; exit(0); }
	map<string, string> values;
	for (int i = 0; i < (argc - 1) / 3; i++){
		string name = argv[3*i+1];
		string eq = argv[3*i+2];
		if (eq != "="){cout << "Error en parametros" << endl; exit(0); }
		string value = argv[3*i+3];
		values[name] = value;
	}
	string parameters[7] = {"N", "pc", "pm", "finalTime", "seed", "fileName", "outputFile"};
	for (int i = 0; i < 7; i++){
		if (values.count(parameters[i]) == 0){
			cout << "Error: parametro " << parameters[i] << " no existe" << endl;
			exit(0);
		}
	}
	int N = atoi(values["N"].c_str());
	double pc = atof(values["pc"].c_str());
	double pm = atof(values["pm"].c_str());
	double finalTime = atof(values["finalTime"].c_str());
	string fileName = values["fileName"];
	int seed = atoi(values["seed"].c_str());
	string outputFile = values["outputFile"];
	srand(seed);
	MA ma(N, pc, pm, finalTime, outputFile);
	Problem p(fileName);
	//Individual::problem = &p;
	cout << "Run" << endl << flush;
	ma.run(seed,p.nodes,p.p,p);
}
