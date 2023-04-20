#ifndef __PROBLEM_H__
#define __PROBLEM_H__

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

#define INF 0x3f3f3f3f

using namespace std;

typedef pair<int,int> iPair;

class Problem
{
    private:
    	list<iPair> *adj;
    public:
    	int p,nodes;
		vector<vector<int> > MC;
		Problem();
		Problem(string file);
		~Problem();
		vector<int> getCost(int source);
		void getDistMat(vector<vector<int> > &distances);
		int CostSD(int source, int dest);
};

#endif
