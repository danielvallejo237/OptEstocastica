
#ifndef __FITNESS_H__
#define __FITNESS_H__

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
#include <climits>

#define INF 0x3f3f3f3f

using namespace std;

typedef pair<int,int> iPair;


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
    Container(Individual &s, Problem &G);
    bool borra(const int &idx);
    void RemoveAndInsert(const int &idx, const int &ns,Individual G);
    void InsertOnly(const int &ns, Problem G);
    void show();
    void reset(Individual &s, Problem &G);
    void clear();
    int GetLoss();
};

#endif