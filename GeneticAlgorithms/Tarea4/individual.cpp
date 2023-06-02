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
    this->generadores=0;
    for(int i=0;i<solucion.size();i++)
    {
        solution[i]=solucion[i];
        this->generadores+=solution[i];
    }
}

Individual::Individual(Individual *ind)
{
    this->size=ind->size;
    this->generadores=ind->generadores;
    this->solution.resize(ind->solution.size());
    for(int i=0;i<ind->solution.size();i++)
    {
        this->solution[i]=ind->solution[i];
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

void Individual::toString()
{
    ofstream ofs ("toeval.txt", std::ofstream::out);
    int rows=(int) sqrt(size);
    int cols=rows;
    ofs<<rows<<" "<<cols<<endl;
    ofs<<generadores<<endl;
    for(int i=0;i<rows;i++)
    {
        for(int j=0;j<cols;j++) ofs<<solution[i*rows+j]<<" ";
        ofs<<endl;
    }
    ofs.close();
}

Individual::~Individual(){}