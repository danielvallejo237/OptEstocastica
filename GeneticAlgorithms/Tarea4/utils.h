#include <iostream>
#include <vector>
#include "individual.h"
#include <map>
#include <set>

using namespace std;

vector<vector<int> > Pop2Matrix(vector<Individual*> population);
vector<int> getCol(const vector<vector<int> > &mat,int colidx);
double mutual_information(const vector<vector<int> > &mat, int c1, int c2);
vector<vector<double> > MIMAT(vector<Individual*> population);
bool compare_pair( const pair<pair<int,int>,double> &pair1, const pair<pair<int,int>,double> &pair2);
vector<pair<pair<int,int>,double> > MSTEDGES(const vector<vector<double> > &G);
pair<double,double> marginal(const vector<vector<int> > &poblacion, int col);
vector<double> joint(const vector<vector<int> > &poblacion, int col1, int col2);
pair<pair<double,double>,vector<vector<double> > > probabilityTable(const vector<pair<pair<int,int>,double> > &MST,const vector<vector<int> > &poblacion);
Individual* generateIndividual(const pair<pair<double,double>,vector<vector<double> > > &generador,const vector<pair<pair<int,int>,double> > &MST, double epsilon,int size);