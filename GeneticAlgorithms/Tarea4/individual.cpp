#include "individual.h"

Individual::Individual(int size,int generadores)
{
    this->size=size;
    this->generadores=generadores;
    float genprob=(float) generadores/(float)size;
    solution.resize(size);
    float aleatorio;
    for (int i=0;i<size;i++)
    {
        aleatorio=(float)rand()/RAND_MAX;
        if (aleatorio<genprob)
        {
            solution[i]=1;
        }
        else
        {
            solution[i]=0;
        }
    }
}

Individual::Individual(vector<int> solucion)
{
    this->size=solucion.size();
    solution.resize(size);
    for(int i=0;i<size;++i)
    {
        solution[i]=solucion[i];
        this->generadores+=solution[i];
    } 
}

void Individual::print(int rows)
{
    int cols=size/rows;
    for (int i=0;i<rows;i++)
    {
        for(int j=0;j<cols;j++)
        {
            cout<<solution[i*rows+j]<<" ";
        }
        cout<<endl;
    }
}

Individual::~Individual(){}