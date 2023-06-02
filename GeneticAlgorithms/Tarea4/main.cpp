//
//  main.cpp
//  Turbines Evaluator Test
//  Example of main function using the Evaluator

#include <iostream>
#include <time.h>
#include <unistd.h>
#include <ios>
#include <fstream>
#include <string>
#include <iomanip>
#include <list>

#include "WindScenario.h"
#include "KusiakLayoutEvaluator.h"
#include "GridEvaluator.h"
#include "Matrix.hpp"
#include "utils.h"


int main(int argc, const char * argv[]) 
{
	srand(time(NULL));
    string scFileName = string(argv[1]);
	srand((unsigned)time( NULL ));
	int popsize=10;

	// These Three objects must be initialized 
    WindScenario wsc(scFileName); // Problem Instance
    KusiakLayoutEvaluator simu; // Simulator
	GridEvaluator evaluator(&simu, &wsc, 10, 10); // Grid Evaluator: Change Grid Size to 10x10 for layout.txt example
	vector<Individual*> solution=run(500,0.05,0.005,500,100,30,evaluator); //Para este caso trabajaremos con 20 generadores
	for(int i=0;i<solution.size();++i)
	{
		int *fl=flatten(solution[i]->solution);
		cout<<evaluator.evaluateGrid(fl,solution[i]->generadores)<<endl;
	}
	return 0;
}
