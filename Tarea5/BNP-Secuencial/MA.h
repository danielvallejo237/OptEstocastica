#ifndef __MA_H__
#define __MA_H__

#include "Individual.h"
#include "Problem.h"
#include "utils.h"

class MA {
	public:
		MA(int N_, double pc_, double pm_, double finalTime_, string &outputFile);
		void run(unsigned int seed,int &n, int &p, Problem problem);
	private:
		//Parameters of MA
		int N;//Population Size
		double pc;//crossover probability
		double pm;//mutation probability
		double finalTime;//Seconds
		string outputFile;

		//Basic procedures of MA
		void initPopulation(unsigned int seed,int &n, int &p, Problem problem);
		void initDI();
		void selectParents();
		void crossover(int &n, int &p, Problem problem);
		void mutation();
		void intensify();
		void replacement();

		//Internal attributes of MA
		vector< Individual * > population; 
		vector< Individual * > parents;
		vector< Individual * > offspring;
		double initialTime;
		double DI;
		int generation;
		double elapsedTime;
};

#endif
