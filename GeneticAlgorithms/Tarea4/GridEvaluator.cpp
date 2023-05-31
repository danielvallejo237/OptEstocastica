#include "GridEvaluator.h"

GridEvaluator::GridEvaluator()
{}

GridEvaluator::GridEvaluator(KusiakLayoutEvaluator * simulator_, WindScenario * scenario_,
		int gridWidth_,  int gridHeight_)
{
	simulator = simulator_;
	scenario = scenario_;

	simulator -> initialize(*scenario);
	N = scenario -> nturbines;
	TerrainWidth = scenario -> width;
	TerrainHeight = scenario -> height;
	gridWidth = gridWidth_;
	gridHeight = gridHeight_;
}
GridEvaluator::~GridEvaluator()
{
	if(Positions) delete Positions;
}

// Read Grid from file
// the format of the file must be
// gridWidth gridHeight
// Number_of_turbines
// 0 1 0 ... 0
// ...
// 0 0 ... 0 // Array of 0 and 1
void GridEvaluator::readGrid( string & filename)
{
	ifstream file(filename);
	int fileGridHeight, fileGridWidth;
	int n;

	if(file.is_open()) {
		file >> fileGridWidth >> fileGridHeight;
		file >> n;
		if(fileGridWidth != gridWidth || fileGridHeight != gridHeight ) {
			cerr << "Grid Width or Height different than the simulator."<<endl;
			exit(-1);
		}

		double squareWidth = TerrainWidth / gridWidth;
		double squareHeight = TerrainWidth / gridHeight;
		//delete Positions;
		Positions = new Matrix<double>(n, 2);

		if(n > gridWidth*gridHeight){ 
			cerr << "Number of turbines bigger than grid spaces."<<endl;
			exit(-1);
		}
	
		int counter = 0;
		
		for(int i = 0; i < gridWidth; i++) {
			for(int j = 0; j< gridHeight; j++) {
				int square;
				file >> square;

				if(square != 0 && square != 1) {
					cerr << "Layout vector can only be 0 or 1." <<endl;
					exit(-1);
				} else if(square == 1) {
					counter++;
					double x = j*squareWidth + (double) squareWidth / 2.0; // Calculate turbine position
					double y = i*squareHeight + (double) squareHeight / 2.0;
					Positions->set(counter-1, 0, x);
					Positions->set(counter-1, 1, y);

				}
			}
		}

		if(counter > n || counter < n) {
			cerr << "Turbine number set to " << n << ", " << counter << " specified." <<endl;
			exit(-1);
		}
	}

	file.close();

}
// Evaluates a grid layout given an array with 0 and 1
// If the number of turbines excedes the number established by the instance
// the solution is not valid.
//
double GridEvaluator::evaluateGrid(int * grid)
{
	double squareWidth = TerrainWidth / gridWidth;
	double squareHeight = TerrainHeight / gridHeight;

	delete Positions;
	Positions = new Matrix<double> (N, 2);
	int counter = 0;


	for(int i = 0; i < gridHeight; i++) {
		for(int j = 0; j < gridWidth; j++) {
			if( *(grid+ i*gridWidth + j) == 1) {
				counter++;
				if(counter > N) break;
				double x = j*squareWidth + squareWidth / 2.0; // Calculate turbine position
				double y = i*squareHeight + squareHeight / 2.0;
				Positions->set(counter-1, 0, x);
				Positions->set(counter-1, 1, y);
			}
			if(counter > N) break;
		}
	}
	if(counter > N) {
		energyCost = std::numeric_limits<double>::max();
		energyCapture  = 0;
		WakeFreeRatio = 0;

		return std:: numeric_limits<double>::max();
	}
	return  evaluateGrid();
}

double GridEvaluator::evaluateGrid()
{
	simulator -> evaluate(Positions);

	WakeFreeRatio =  simulator -> getWakeFreeRatio();
	if(WakeFreeRatio == 0) energyCost = std::numeric_limits<double>::max();
	else energyCost =  simulator -> getEnergyCost();

	energyCapture = simulator -> getEnergyOutput();
	return energyCost;

}
