#include <iostream>
#include <cstdlib>
#include <vector>
#include <fstream>
#include <cmath>
using namespace std;

class Individual
{
    public:
        int generadores=0; //Important to initialize the number of generators to 0
        int size=0;
        vector<int> solution;
        Individual(int size,int generadores);
        Individual(vector<int> solucion);
        Individual(Individual *ind);
        ~Individual();
        void print(int rows);
        void toString();
};