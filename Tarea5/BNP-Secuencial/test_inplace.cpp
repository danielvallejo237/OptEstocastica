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
    Individual ind(100,9,p);
    Individual ind2(100,9,p);
    ind.initialize_heuristic(123);
    cout<<"Costo 1: "<<ind.getCost()<<endl;
    ind2.initialize_heuristic(234);
    ind.swap_values_by_pos(2,10);
    //cout<<ind.getDistance(ind2)<<endl;
    ind.crossover(ind2);
    cout<<"Elemento en la posición 2: "<<get_element_by_index(ind.used,2)<<endl;
    cout<<"Costo 2: "<<ind.getCost()<<endl;
    ind.intensify();
    cout<<"Costo 3: "<<ind.getCost()<<endl;
    ind.print_to_console();
    cout<<"Costo 4: "<<ind.getCost()<<endl;
    return 0;      
}