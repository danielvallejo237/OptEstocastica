#ifndef __MA_H__
#define __MA_H__

#include "Individual.h"

class MA {
	public:
		MA(int N_, double pc_, double pm_, double finalTime_, string &outputFile);
		void run();
	private:
		//Parameters of MA
		int N;//Population Size
		double pc;//crossover probability
		double pm;//mutation probability
		double finalTime;//Seconds
		string outputFile;

		//Basic procedures of MA
		void initPopulation();
		void initDI();
		void selectParents();
		void crossover();
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
