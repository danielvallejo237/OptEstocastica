#include "Individual.h"
#include "Problem.h"
#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <numeric>
#include <algorithm>
#include <chrono>
#include <random>
#include <queue>
#include <ctime>
#include <unordered_set>
#include <set> 

#define TFitness long long 
#define TDistance long long 
#define MAX_DISTANCE LLONG_MAX
#define INF 0x3f3f3f3f

int main(int argc, char* argv[])
{
    Problem p(argv[1]);
    Individual ind(20,9);
    Individual ind2(20,9);
    ind.initialize_heuristic(123);
    ind2.initialize_heuristic(234);
    ind.print();
    ind.swap_values_by_pos(2,10);
    ind.print();
    ind2.print();
    //cout<<ind.getDistance(ind2)<<endl;
    ind.crossover(ind2);
    ind.print();
    ind2.print();
    return 0;      
}