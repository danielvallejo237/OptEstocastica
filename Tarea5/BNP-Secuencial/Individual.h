#ifndef __INDIVIDUAL_H__
#define __INDIVIDUAL_H__

#include "Problem.h"
#define TFitness long long 
#define TDistance long long 
#define MAX_DISTANCE LLONG_MAX

class Individual {
	public:
		Individual();
		~Individual(){};
		void initialize_heuristic();
		TDistance getDistance(Individual &ind);
		void mutation(double pm);
		void crossover(Individual &ind);//Cross both individuals (the result is saved in the individuals).
		void intensify();
		void send(int to);
		void receive(int from);
		static Problem *problem;
		void print(const string &fileName);
		void evaluate();
		TDistance distance;//Minimium distance to survivors
		TFitness getCost();//Cost: minimization problem 
		
		vector<int> S;
	private:	
		TFitness cost;
};

#endif
