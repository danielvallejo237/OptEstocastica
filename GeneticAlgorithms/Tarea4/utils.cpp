#include "utils.h"

vector<vector<int> > Pop2Matrix(vector<Individual*> population)
{
    int rows,cols;
    rows=population.size();
    cols=population[0]->size;
    vector<vector<int> > mat(rows,vector<int> (cols));
    for(int i=0;i<rows;i++)
    {
        for(int j=0;j<cols;j++)
        {
            mat[i][j]=population[i]->solution[j];
        }
    }
    return mat;
}

vector<int> getCol(const vector<vector<int> > &mat,int colidx)
{
    vector<int> col(mat.size(),0);
    for (int i=0;i<mat.size();i++) col[i]=mat[i][colidx];
    return col;
}

double mutual_information(const vector<vector<int> > &mat, int c1, int c2)
{
    map<pair<int, int>, int> frecs;
    frecs[make_pair(0,0)]=0;
    frecs[make_pair(0,1)]=0;
    frecs[make_pair(1,0)]=0;
    frecs[make_pair(1,1)]=0;
    double pc1=0;
    double pc2=0; //Probabilidad de que suceda la variable 1
    for (int i=0;i<mat.size();i++)
    {
        frecs[make_pair(mat[i][c1],mat[i][c2])]+=1;
        if (mat[i][c1]) pc1+=1;
        if (mat[i][c2]) pc2+=1;
    }
    double mutual_information=0.0;
    set<int> nums={0,1};
    float joint,marginal;
    double p1=(float) pc1/(float)mat.size();
    double p2=(float) pc2/ (float) mat.size();
    for(set<int>::iterator it=nums.begin();it!=nums.end();++it)
    {
        for(set<int>::iterator it2=nums.begin();it2!=nums.end();++it2)
        {
            joint=(float) frecs[make_pair(*it,*it2)]/(float) mat.size();
            if (joint==0.0) mutual_information+=0.0;
            else 
            {
                if((*it==0) && (*it2 ==0))
                {
                    if ((1-p1)*(1-p2)>0) mutual_information+=joint*log(joint/((1-p1)*(1-p2)));
                    else mutual_information+=0;
                }
                else if ((*it==1) && (*it2 ==0))
                {
                    if ((p1)*(1-p2)>0) mutual_information+=joint*log(joint/((p1)*(1-p2)));
                    else mutual_information+=0;
                }
                else if ((*it==0) && (*it2 ==1))
                {
                    if ((1-p1)*(p2)>0) mutual_information+=joint*log(joint/((1-p1)*(p2)));
                    else mutual_information+=0;
                }
                else 
                {
                    if((p1)*(p2)>0) mutual_information+=joint*log(joint/((p1)*(p2)));
                    else mutual_information+=0;
                }
            }
        }
    }
    return mutual_information;
}

vector<vector<double> > MIMAT(vector<Individual*> population)
{
    vector<vector<int> > mat=Pop2Matrix(population);
    int size=population[0]->size;
    vector<vector<double> > IM(size, vector<double> (size,0.0));
    for (int i=0;i<size;i++)
    {
        for(int j=i;j<size;j++)
        {
            IM[i][j]=mutual_information(mat,i,j);
            IM[j][i]=IM[i][j];
        }
    }
    return IM;
}

bool compare_pair( const pair<pair<int,int>,double> &pair1, const pair<pair<int,int>,double> &pair2)
{
    bool result = true;
    if(pair2.second >= pair1.second) result = false;
    return result;
}


vector<pair<pair<int,int>,double> > MSTEDGES(const vector<vector<double> > &G)
{
    int edge;
    int visit[G.size()];
    for (int i=0;i<G.size();++i) visit[i]=false;
    edge=0;
    visit[0]=true;
    int x,y;
    int V=(int) G.size();
    vector<pair<pair<int,int>,double> > mst(V-1);
    int counter=0;
    while (edge<V-1)
    {
        double max=(double) INT16_MIN;
        x=0;
        y=0;
        for (int i=0;i<V;i++)
        {
            if(visit[i])
            {
                for(int j=0;j<V;j++)
                {
                    if(!visit[j] && G[i][j]>0)
                    {
                        if (max<G[i][j])
                        {
                            max=G[i][j];
                            x=i;
                            y=j;
                        }
                    }
                }
            }
        }
        visit[y]=true;
        mst[counter].first=make_pair(x,y);
        mst[counter].second=G[x][y];
        edge++;
        counter++;
    }
    sort(mst.begin(),mst.end(),&compare_pair);
    return mst;
}

pair<double,double> marginal(const vector<vector<int> > &poblacion, int col)
{
    vector<int> columna=getCol(poblacion,col);
    double ones,zeros;
    ones=0.0;
    zeros=0.0;
    for(vector<int>::iterator it=columna.begin();it!=columna.end();++it)
    {
        if(*it) ones+=1.0;
        else zeros+=1.0;
    }
    ones=ones/(double) columna.size();
    zeros=zeros/(double) columna.size();
    return make_pair(zeros,ones);
}

vector<double> joint(const vector<vector<int> > &poblacion, int col1, int col2)
{
    //Probabilidades conjuntas
    /*
    (0,0)
    (0,1)
    (1,0)
    (1,1)
    */
    pair<double,double> marginalc1=marginal(poblacion,col1);
    pair<double,double> marginalc2=marginal(poblacion,col2);
    map<pair<int,int>,double > frecs;
    frecs[make_pair(0,0)]=0;
    frecs[make_pair(0,1)]=0;
    frecs[make_pair(1,0)]=0;
    frecs[make_pair(1,1)]=0;
    vector<double> pbs(4,0.0);
    int counter=0;
    double prod=0;
    for (int i=0;i<poblacion.size();i++) frecs[make_pair(poblacion[i][col1],poblacion[i][col2])]+=1.0;
    for (map<pair<int,int>,double >::iterator it=frecs.begin();it!=frecs.end();++it)
    {
        it->second/=(double) poblacion.size();
        if (!it->first.first && !it->first.second)
        {
            prod=marginalc1.first*marginalc2.first;
            if (prod>0) pbs[counter]=it->second;
            else pbs[counter]=0;
        }
        else if(it->first.first && !it->first.second)
        {
            prod=marginalc1.second*marginalc2.first;
            if (prod>0) pbs[counter]=it->second;
            else pbs[counter]=0;
        }
        else if(!it->first.first && it->first.second)
        {
            prod=marginalc1.first*marginalc2.second;
            if (prod>0) pbs[counter]=it->second;
            else pbs[counter]=0;
        }
        else 
        {
            prod=marginalc1.second*marginalc2.second;
            if (prod>0) pbs[counter]=it->second;
            else pbs[counter]=0;
        }
        counter++;
    }
    return pbs;
}

pair<pair<double,double>,vector<vector<double> > > probabilityTable(const vector<pair<pair<int,int>,double> > &MST,const vector<vector<int> > &poblacion)
{
    //Calculamos la tabla de probabilidad para cada una de las entradas del individuo
    //Caluculamos las probabilidades para cada parte de la raíz
    pair<double,double> marginalRoot=marginal(poblacion,MST[0].first.first);
    vector<bool> visited(poblacion[0].size(),false);
    vector<vector<double> > probas(MST.size(),vector<double> (4));
    vector<double> v(4);
    for (int i=0;i<MST.size();i++)
    {
        v=joint(poblacion,MST[i].first.first,MST[i].first.second);
        for(int j=0;j<4;j++)
        {
            probas[i][j]=v[j];
        } 
    }
    return make_pair(marginalRoot,probas);
}

double genrandom()
{
    return (double) rand() / (double) RAND_MAX;
}

Individual* generateIndividual(const pair<pair<double,double>,vector<vector<double> > > &generador,const vector<pair<pair<int,int>,double> > &MST, double epsilon,int size)
{
    //Generador de nuevos individuos 
    double rd=genrandom();
    double eg;
    double tmp1;
    vector<int> ind(size,0);
    if (rd>generador.first.first) ind[MST[0].first.first]=1;
    for(int i=0;i<generador.second.size();i++)
    {
        if(ind[MST[i].first.first])
        {
            eg=genrandom();
            if(eg<epsilon)
            {
                if(genrandom()>0.5) ind[MST[i].first.second]=1;
                else ind[MST[i].first.second]=0;
            }
            else
            {
                //This is a deterministic approach, it needs to be changed to a stochastic one
                tmp1=generador.second[i][2]/(generador.second[i][2]+generador.second[i][3]);
                ind[MST[i].first.second]= (genrandom() < tmp1) ? 0 : 1; 
            }
        }
        else 
        {
            eg=genrandom();
            if(eg<epsilon)
            {
                if(genrandom()>0.5) ind[MST[i].first.second]=1;
                else ind[MST[i].first.second]=0;
            }
            else
            {
                //Changing the model to a moree stochastic approach
                tmp1=generador.second[i][0]/(generador.second[i][0]+generador.second[i][1]);
                ind[MST[i].first.second]= (genrandom() < tmp1) ? 0 : 1; 
            }
        }
    } 
    return new Individual(ind);
}


vector<Individual*> nextPopulation(vector<Individual*> old_population,double epsilon, int popsize, int size)
{
    /*
    Generación de la siguiente población de individuos en donde calculamos las soluciones 
    más cercanas a la cantidad de generadores que queremos encontrar
    */
    vector<vector<double> > IM=MIMAT(old_population); //Matriz de información mutua
    vector<vector<int> > PM=Pop2Matrix(old_population); //Convert a matriz to a next population
    vector<pair<pair<int,int>,double> > parejas=MSTEDGES(IM); //Minimum spanning tree que corresopnde a la construcción del arbol de Chow Liu
	pair<pair<double,double>,vector<vector<double> > > ks=probabilityTable(parejas,PM); //Tabla de probabilidades para la generación de los infividuos

    vector<Individual*> new_pop(popsize);
    for (int i=0;i<new_pop.size();i++)
    {
        new_pop[i]=generateIndividual(ks,parejas,epsilon,size);
        //cout<<"Generadores est "<<new_pop[i]->generadores<<endl;
    }
    return new_pop; //Regresamos la nueva población 
}

bool isValid(Individual *ind, int expected_gen)
{
    int suma=0;
    for(int i=0;i<ind->solution.size();i++) suma+=ind->solution[i];
    return (suma > expected_gen) ? false : true;
}

vector<Individual*> selectKbest(vector<Individual*> poblacion, int expected_gen, int new_pop_size,GridEvaluator evaluador)
{
    vector<pair<int,int> > cercania(poblacion.size());
    int i=0;
    for (vector<Individual*>::iterator it=poblacion.begin();it!=poblacion.end();++it)
    {
        int *fl=flatten((*it)->solution);
        int suma=0;
        for(int i=0;i<(*it)->solution.size();i++) suma+=(*it)->solution[i];
        cercania[i]=make_pair(0.4*evaluador.evaluateGrid(fl,suma)+0.6*((double)abs(expected_gen-suma)/(double)expected_gen),i);
        //Tomamos los elementos que estén más cercanos al número de generadores esperados, en este caso son 20
        i++;
    }
    sort(cercania.begin(),cercania.end());
    vector<Individual*> selected(new_pop_size);
    for (int j=0;j<new_pop_size;j++)
    {
        if (isValid(poblacion[cercania[j].second],expected_gen)) selected[j]=new Individual(poblacion[cercania[j].second]);
        else selected[j]=new Individual(poblacion[cercania[j].second]->size,expected_gen);
        int suma=0;
        for(int i=0;i<selected[j]->solution.size();i++) suma+=selected[j]->solution[i];
        selected[j]->generadores=suma;
    }
    return selected;
}

double decay_epsilon(double epsilon_init, double epsilon_end,int step,int gen)
{
    return epsilon_init-(double) step *(epsilon_init-epsilon_end)/(double)gen;
}


vector<Individual*> run(int generations, double epsiloninit, double epsilon_end, int popsize, int size, int expected_gen, GridEvaluator evaluador)
{
    vector<Individual*> population;
	for (int i=0;i<5*popsize;i++) population.push_back(new Individual (size,expected_gen));
    for(int i=0;i<generations;i++)
    {
        //Agarramos a los mejores de la población
        vector<Individual*> best=selectKbest(population,expected_gen,popsize,evaluador);
        vector<Individual*> population=nextPopulation(best,decay_epsilon(epsiloninit,epsilon_end,i,generations),5*popsize,size);
    }
    vector<Individual*> np=selectKbest(population,expected_gen,popsize,evaluador);
    //for(vector<Individual*>::iterator it=np.begin();it!=np.end();++it) cout<<(*it)->generadores<<endl;
    return np; //Regresa la población con los que se supone son los mejores elementos encontrados por el algoritmo
}

int* flatten(const vector<int> &v)
{
    int *aplanado=new int [v.size()];
    for (int i=0;i<v.size();i++) aplanado[i]=v[i];
    return aplanado;
}