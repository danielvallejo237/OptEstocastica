/*
Implementación del código de evolución diferencial para encotrar óptimos en diversas funciones

Daniel Vallejo Aldana (danielvallejo237 Github) / CIMAT 2023
*/

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>

using namespace std;

//Así mismo generar una función que genere números aleatorios de punto flotante 

class RandFGenerator
{
    public:
    double dmin,dmax;
    RandFGenerator(double dmin, double dmax);
    double randf();
};

RandFGenerator::RandFGenerator(double dmin, double dmax)
{
    this->dmin=dmin;
    this->dmax=dmax;
}

double RandFGenerator::randf()
{
    double f=(double) rand() / RAND_MAX;
    return dmin + f*(dmax-dmin);
}

//Generación de números ente 0 y 1

double randco()
{
    //Genera un número aleatorio entre 0 y 1
    return (double) rand()/RAND_MAX;
}

//Definición de las funciones que serán usadas en la tarea

double Ackley(vector<double> x)
{
    double a=20;
    double b=0.2;
    double c=2*M_PI;
    double suma=0; 
    double sumacos=0;
    for(int i=0;i<x.size();i++)
    {
        suma+=(x[i]*x[i]);
        sumacos+=cos(c*x[i]);
    }
    suma=sqrt(((double)1.0/((double)x.size()))*suma);
    sumacos=(double)1.0/(double)x.size()*sumacos;
    return -a*exp(-b*suma)-exp(sumacos)+a+exp(1); 
}

double sphere(vector<double> x)
{
    double sum=0;
    for(int i=0;i<x.size();i++) sum+=x[i]*x[i];
    return sum;
}

double Rastigrin(vector<double> x)
{
    double sum=0;
    for(int i=0;i<x.size();i++)
    {
        sum+=(x[i]*x[i])-10*cos(2*M_PI*x[i]);
    }
    return 10*(int)x.size()+sum;
}
//Algunas funciones auxiliares necesarias para la tarea como el clip de los valores generados y la generación de los pesos iniciales
void InitWeights(vector<vector<double> > &pesos, RandFGenerator gen)
{
    for (int i=0;i<pesos.size();i++)
    {
        for(int j=0;j<pesos[0].size();j++) pesos[i][j]=gen.dmin+randco()*(gen.dmax-gen.dmin);
    }
}

vector<double> getFitnessVector(vector<vector<double> > pop, double (*f)(vector<double>))
{
    vector<double> fitness(pop.size());
    for (int i=0;i<pop.size();i++)
    {
        fitness[i]=f(pop[i]);
    }
    return fitness;
} 

int main(int argc, char *argv[])
{
     //Es el numero máximo de generaciones que haremos en esta evaluación
    int pop_size=atoi(argv[1]);
    double (*f)(vector<double>); //Un apuntador a funciones
    int D=atoi(argv[2]); //Dimensión del problema a tratar
    int generations=10000;
    double F=0.85;
    int opt=atoi(argv[6]);
    if(opt==1) f=Ackley;
    else if(opt==2) f=sphere;
    else f=Rastigrin;
    double cr=1.0-1.0/(double) D; //Probabilidad de cruza
    RandFGenerator gen(atof(argv[3]),atof(argv[4]));
    srand(atoi(argv[5])); //Inicializamos con una semilla el generador de numeros aleatorios
    vector<vector<double> > Population(pop_size,vector<double> (D));
    InitWeights(Population,gen); //Inicializamos soluciones de forma aleatoria dentro de la población
    vector<double> fitness=getFitnessVector(Population,f); //Regresamos el vector inicial de la evaluación
    vector<double> tmp(Population[0].size());
    double tmpeval;
    int a,b,c,Rnd;
    int ibest;
    for(int generation=0;generation<generations;generation++)
    {
        for(int ind=0;ind<Population.size();ind++)
        {
            a=rand() % (Population.size()-1);
            while(a==ind) a=rand() % (Population.size()-1);
            b=rand() % (Population.size()-1);
            while (( b==a) || (b==ind) ) b=rand() % (Population.size()-1);
            c=rand() % (Population.size()-1);
            while ((c==a) || (c==b) || (c==ind)) c=rand() % (Population.size()-1);
            
            //Creamos un individuo de prueba en el que al menos un elemento es cambiado
            Rnd=(int)(randco()*D);
            for(int i=0;i<tmp.size();i++)
            {
                if((randco()<cr) || (Rnd==i)) tmp[i]=Population[c][i]+F*(Population[a][i]-Population[b][i]);
                else tmp[i]=Population[ind][i];
            }
            //Verificamos las condiciones de frontera
            for(int i=0;i<tmp.size();i++)
            {
                if((tmp[i]<gen.dmin)|| (tmp[i]>gen.dmax)) tmp[i]=gen.dmin+(gen.dmax-gen.dmin)*randco(); 
            }
            tmpeval=f(tmp);
            if(tmpeval<fitness[ind])
            {
                //Hacemos la actualización de los datos sobre nuestra población
                for(int i=0;i<tmp.size();i++)
                {
                    Population[ind][i]=tmp[i];
                }
                fitness[ind]=tmpeval;
                if (tmpeval<fitness[ibest]) ibest=ind;

            }
        } 
    }
    cout<<(double) fitness[ibest]<<endl;
    return 0;
}