#include <iostream>
#include <cstdlib>
#include <vector>

using namespace std;

class Individual
{
    public:
        int generadores;
        int size;
        vector<int> solution;
        Individual(int size,int generadores);
        Individual(vector<int> solucion);
        ~Individual();
        void print(int rows);
};