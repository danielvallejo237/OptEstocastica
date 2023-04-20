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
    Container(int nodes);
    Container(set<int> s, Problem G);
    bool borra(const int &idx);
    void RemoveAndInsert(const int &idx, const int &ns, Problem G);
    void InsertOnly(const int &ns, Problem G);
    void show();
    void reset(set<int> s, Problem G);
    void clear();
    int GetLoss();
};


class Individual
{
    private:
    int p;
    int nodes;
    public:
	set<int> used;
    set<int> notused;
    Individual(const int &nodes, const int &p);
    void initialize_heuristic(unsigned int seed);
    bool check();
    void swap_values_by_pos(int pos_used, int pos_notused);
    void print();
	static Problem *problem;
	Container *container;
	void update();
    TDistance getDistance(Individual &ind);
    void crossover(Individual &ind); //Cruza de los individuos y como dice ahí el resultado de la cruza se guarda en los individuos.
};

#endif
