#ifndef __INDIVIDUAL_H__
#define __INDIVIDUAL_H__

#include "Problem.h"
#define TFitness long long 
#define TDistance long long 
#define MAX_DISTANCE LLONG_MAX

class Uheap : public priority_queue<iPair,vector<iPair>,greater<iPair> >
{
    public:
    bool remove(const int &idx);
    void screenShow();
    void clear();
};

class Container
{
    public:
    vector<Uheap> vheap;
    int n;
    Container();
    Container(int nodes);
    Container(unordered_set<int> s, Problem G);
    bool borra(const int &idx);
    void RemoveAndInsert(const int &idx, const int &ns, Problem G);
    void InsertOnly(const int &ns, Problem G);
    void show();
    void reset(unordered_set<int> s, Problem G);
    void clear();
    int GetLoss();
};

pair<int,int> gen_random_descriptor(const int &n,const int &p);

class Individual
{
    private:
    int p;
    int nodes;
    public:
	unordered_set<int> used;
    unordered_set<int> notused;
    Individual(const int &nodes, const int &p, Problem &G);
    void initialize_heuristic(unsigned int seed);
    bool check();
    void swap_values_by_pos(int pos_used, int pos_notused);
    void print(const string &fileName, const int&min_mean_dist);
    void print_to_console();
	Problem problem;
	Container container;
	void update();
    TDistance getDistance(Individual &ind);
    TFitness getCost();
    TDistance distance;
    void crossover(Individual &ind); //Cruza de los individuos y como dice ahí el resultado de la cruza se guarda en los individuos.
    void intensify(); //La parte del algoritmo memético en donde vamos a encontrar una mejor solución
};


int get_element_by_index(unordered_set<int> &s,int index);

#endif
