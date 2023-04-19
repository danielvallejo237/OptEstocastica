#include <sys/time.h>
#include <iostream>
#include <signal.h>

#include "MA.h"
#include "utils.h"

using namespace std;

MA::MA(int N_, double pc_, double pm_, double finalTime_, string &outputFile_){
	N = N_;
	pc = pc_;
	pm = pm_;
	finalTime = finalTime_;
	outputFile = outputFile_;
	struct timeval currentTime; 
	gettimeofday(&currentTime, NULL);
	initialTime = (double) (currentTime.tv_sec) + (double) (currentTime.tv_usec)/1.0e6;
}

void MA::initPopulation(){
	for (int i = 0; i < N; i++){
		Individual *ei = new Individual();
		ei->initialize_heuristic();
		ei->intensify();
		population.push_back(ei);
	}
}

//Select two parents with binary selection
void MA::selectParents(){
	parents.clear();
	for (int i = 0; i < N; i++){//Pueden ser 2 o N
		int first = getRandomInteger0_N(N - 1);
		int second = getRandomInteger0_N(N - 1);
		if (population[first]->getCost() <= population[second]->getCost()){
			parents.push_back(population[first]);
		} else {
			parents.push_back(population[second]);
		}
	}
}

void MA::crossover(){
	for (int i = 0; i < parents.size(); i++){
		Individual *ni = new Individual();
		*ni = *parents[i];
		offspring.push_back(ni);
	}
	for (int i = 0; i < offspring.size(); i+=2){
		if (generateRandomDouble0_Max(1) <= pc){
			offspring[i]->crossover(*offspring[i+1]);
		}
	}
}

void MA::mutation(){
	for (int i = 0; i < offspring.size(); i++){
		offspring[i]->mutation(pm);
	}
}

void MA::intensify(){
	for (int i = 0; i < offspring.size(); i++){
		offspring[i]->intensify();
	}
}


void MA::replacement(){
	vector < Individual* > all;
	
	//Join population and offspring
	for (int i = 0; i < population.size(); i++){
		all.push_back(population[i]);
		all.back()->distance = MAX_DISTANCE;
	}
	population.clear();

	for (int i = 0; i < offspring.size(); i++){
		all.push_back(offspring[i]);
		all.back()->distance = MAX_DISTANCE;
	}
	offspring.clear();
	
	//Select best solution
	int indexBest = 0;
	for (int i = 1; i < all.size(); i++){
		if (all[i]->getCost() < all[indexBest]->getCost()){
			indexBest = i;
		}
	}
	population.push_back(all[indexBest]);
	all[indexBest] = all.back();
	all.pop_back();

	struct timeval currentTime; 
	gettimeofday(&currentTime, NULL);
	double elapsedTime = (double) (currentTime.tv_sec) + (double) (currentTime.tv_usec)/1.0e6;
	elapsedTime -= initialTime;

	//Select next N - 1 solution
	double D = DI - DI * elapsedTime / finalTime;
	while(population.size() != N){
		//Update distances
		for (int i = 0; i < all.size(); i++){
			all[i]->distance = min(all[i]->distance, all[i]->getDistance(*population.back()));
		}
		//Select best option
		indexBest = 0;
		for (int i = 1; i < all.size(); i++){
			bool betterInDist =	(all[i]->distance > all[indexBest]->distance);
			bool eqInDist = (all[i]->distance == all[indexBest]->distance);
			bool betterInFit = (all[i]->getCost() < all[indexBest]->getCost());
			bool eqInFit = (all[i]->getCost() == all[indexBest]->getCost());

			if (all[indexBest]->distance < D){//Do not fulfill distance requirement
				if ((betterInDist) || (eqInDist && betterInFit)){
					indexBest = i;
				}
			} else {
				if (all[i]->distance >= D){
					if ((betterInFit) || (eqInFit && betterInDist)){
						indexBest = i;
					}
				}
			}
		}
		//Insert best option
		population.push_back(all[indexBest]);
		all[indexBest] = all.back();
		all.pop_back();
	}
	//Release memory
	for (int i = 0; i < all.size(); i++){
		delete(all[i]);
	}
}

void MA::initDI(){
	double meanDistance = 0;
	for (int i = 0; i < population.size(); i++){
		for (int j = i + 1; j < population.size(); j++){
			meanDistance += population[i]->getDistance(*population[j]);
		}
	}
	meanDistance /= (population.size() * (population.size() - 1)) / 2;
	DI = meanDistance * 0.5;
}

void MA::run(){
	initPopulation();
	initDI();
	generation = 0;
	double cTime;
	double bestCost;
	do {
		selectParents();
		crossover();
		mutation();
		intensify();
		replacement();
		struct timeval currentTime; 
		gettimeofday(&currentTime, NULL);
		cTime = (double) (currentTime.tv_sec) + (double) (currentTime.tv_usec)/1.0e6;
		elapsedTime = cTime - initialTime;
		if (generation == 0){
			population[0]->print(outputFile);
			bestCost = population[0]->getCost();
		} else {
			if (population[0]->getCost() < bestCost){
				population[0]->print(outputFile);
				bestCost = population[0]->getCost();
			}
		}
		generation++;
	} while(cTime - initialTime < finalTime);
}
