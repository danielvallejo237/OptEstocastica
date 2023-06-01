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
	string layoutFileName = string(argv[2]);
	string outputFile = string(argv[3]);
	srand((unsigned)time( NULL ));
	int popsize=10;

	// These Three objects must be initialized 
    WindScenario wsc(scFileName); // Problem Instance
    KusiakLayoutEvaluator simu; // Simulator
	GridEvaluator evaluator(&simu, &wsc, 10, 10); // Grid Evaluator: Change Grid Size to 10x10 for layout.txt example

	evaluator.readGrid(layoutFileName); // Read Grid from file
	evaluator.evaluateGrid(); // Evaluate Grid
	ofstream outFile(outputFile);

	outFile << fixed << setprecision(15)<<endl;
	outFile << "WakeFreeRatio: " << evaluator.getWakeFreeRatio() <<endl;
	outFile << "EnergyCosts: " << evaluator.getEnergyCost() <<endl;
	outFile << "EnergyOutput: " << evaluator.getEnergyOutput() <<endl;

	outFile.close();

	vector<Individual*> solution=run(5,0.16,0.05,128,100,20); //Para este caso trabajaremos con 20 generadores
	for(int i=0;i<solution.size();++i) solution[i]->print(10);
	for(int i=0;i<solution.size();++i)
	{
		int *fl=flatten(solution[i]->solution);
		cout<<evaluator.evaluateGrid(fl)<<endl;
	}
	

	/* 
	 * Use example with integer array
	 * Uncomment next section to test the use of the functions.
	 * 
	 * */

	//int * arr = new int[10000];
	//int * arr2 = new int[10000];



	//for(int i = 0; i<100; i++) {
		//for(int j = 0; j<100; j++) {
			//*(arr + i*100 + j) = rand() % 2;
			//*(arr2 + i*100 + j) = 0;
		//}
	//}

	//int counter = 0;
	//int selected = -1;

	//while(counter < wsc.nturbines) 
		//while( selected == -1 ||  arr2[selected] == 1) {
			//selected = rand() % 10000;
		//}
		//arr2[selected] = 1;	
		//counter++;
	//}

	//// The first array has more turbines than the allowed number
	//cout << " Invalid Solution " <<endl;
	//evaluator.evaluateGrid(arr); // This function takes a pointer to an integer array and evaluates its fitness
	//cout.precision(15);
	//cout << "WFR results: " << evaluator.getWakeFreeRatio() <<endl;
	//cout << "energy costs: " << evaluator.getEnergyCost() <<endl;
	//cout << "EnergyOutput: " << evaluator.getEnergyOutput() <<endl<<endl;

	//cout << "Feasible Solution" <<endl;
	//evaluator.evaluateGrid(arr2);
	//cout.precision(15);
	//cout << "WFR results: " << evaluator.getWakeFreeRatio() <<endl;
	//cout << "energy costs: " << evaluator.getEnergyCost() <<endl;
	//cout << "EnergyOutput: " << evaluator.getEnergyOutput() <<endl;


	return 0;
}
