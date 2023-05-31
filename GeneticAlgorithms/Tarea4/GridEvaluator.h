#ifndef __GRID_EVALUATOR_H__
#define __GRID_EVALUATOR_H__

#include "KusiakLayoutEvaluator.h"
#include "Matrix.hpp"
#include "WindScenario.h"
#include <fstream>
#include <limits>

using namespace std;

class GridEvaluator {

	int N;
	double TerrainWidth, TerrainHeight;
	int gridWidth, gridHeight;
	double energyCost = std::numeric_limits<double>::max();
	double WakeFreeRatio = 0;
	double energyCapture = 0;


	WindScenario * scenario;
	KusiakLayoutEvaluator * simulator;

	public:	

	Matrix <double>  * Positions;
	GridEvaluator();
	~GridEvaluator();
	GridEvaluator(KusiakLayoutEvaluator * simulator_, WindScenario * scenario_,
			int gridWidth_, int gridHeight_);
	

	void readGrid(string & fileName);
	double evaluateGrid( int * grid);
	double evaluateGrid();
	double inline getEnergyCost() {return energyCost;}
	double inline getWakeFreeRatio() {return WakeFreeRatio;}
	double inline getEnergyOutput() {return energyCapture;}
	Matrix<double> * getEnergyOutputs() { return simulator -> getEnergyOutputs();} 
	Matrix<double> * getTurbineFitnesses() {return simulator -> getTurbineFitnesses();}

};

#endif


