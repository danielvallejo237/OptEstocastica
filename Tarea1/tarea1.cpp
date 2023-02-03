/*
Códigos de la tarea 1 de optimización estocástica
@Author Daniel Vallejo Aldana / CIMAT
*/
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
#define INF 0x3f3f3f3f

using namespace std;

typedef pair<int,int> iPair;

class Graph
{
public:
private:
  list<pair<int,int> > *adj;
  int nodes;
  int p;
public:
  vector <vector<int> > MC;
  Graph(string file)
  {
    ifstream f(file);
    if (!f.is_open())
    {
      cout<<"Archivo no encontrado";
      exit(EXIT_FAILURE); //Salimos si no es posible abrir el archivo que contiene la estructura del grafo
    }
    int edges;
    f>>nodes>>edges>>p;
    adj=new list<iPair>[nodes];
    for (int i=0;i<edges;i++)
    {
      int a,b,c;
      f>>a>>b>>c;
      adj[a-1].push_back(make_pair(b-1,c));
      adj[b-1].push_back(make_pair(a-1,c));
    }
    MC.resize(nodes);
    for(int i=0;i<nodes;i++) MC[i].resize(nodes);
    getDistMat(MC);
  }
  vector<int> getCost(int source)
  {
    //Vamos a aplicar el algoritmo de dijkstra para la evaluación de las instancias
    //En este caso usamos colas de prioridad para obtener la mínima distancia a cada uno de los nodos
    priority_queue<iPair,vector<iPair>,greater<iPair> >pq;
    vector<int> dist(nodes,INF);
    pq.push(make_pair(0,source));
    dist[source]=0;
    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();
        list<pair<int, int> >::iterator i;
        for (i = adj[u].begin(); i != adj[u].end(); ++i) {
            int v = (*i).first;
            int weight = (*i).second;
            if (dist[v] > dist[u] + weight) {
                dist[v] = dist[u] + weight;
                pq.push(make_pair(dist[v], v));
            }
        }
    }
    return dist; //Regresamos el vector con las mínimas distancias de un nodo a los demás
  }
  int nNodes()
  {
    return this->nodes;
  }
  int pVal()
  {
    return this->p;
  }
  void getDistMat(vector<vector<int> > &distances)
  {
    for (int i=0;i<nodes;i++)
    {
      distances[i]=getCost(i);
    }
  }
  int CostSD(int source, int dest)
  {
    return MC[source-1][dest-1]; //te regresa el costo mínimo del source al destino
  }
};


class SolutionGenerator
{
  public:
    int p,nodes,seed;
    vector<int> sol;
    vector<int> numbers;
    SolutionGenerator(int p, int nodes,int seed)
    {
      this->p=p;
      this->nodes=nodes;
      this->seed=seed;
      sol.resize(p);
      numbers.resize(nodes);
      for(int i=0;i<nodes;i++) numbers[i]=i;
    }
    void generateSolution()
    {
      std::shuffle(numbers.begin(), numbers.end(), std::default_random_engine(seed));
      for(int i=0;i<p;i++) sol[i]=numbers[i]; //Nos aseguramos que no haya nodos repetidos
    }
    int evalSolution(const vector<vector<int> > &C)
    {
      vector<vector<int> > aux(p,vector<int> (nodes));
      for(int i=0;i<p;i++)
      {
        aux[i]=C[sol[i]];
      }
      vector<int> best(nodes);
      for(int j=0;j<nodes;j++)
      {
        int md=aux[0][j];
        for(int k=1;k<p;k++)
        {
          if(aux[k][j]<md) md=aux[k][j];
        }
        best[j]=md;
      }
      return std::accumulate(best.begin(),best.end(),decltype(best)::value_type(0));
    }
    int GenerateAndEvaluate(const vector<vector<int> > &C)
    {
      generateSolution();
      int res=evalSolution(C);
      return res;
    }
};

int main(int argc,char* argv[])
{
  Graph G(argv[1]);
  unsigned int seed=atoi(argv[2]);
  SolutionGenerator Gen(G.pVal(),G.nNodes(),seed);
  int min=INF;
  int aux;
  for(int i=0;i<100000;i++)
  {
    aux=Gen.GenerateAndEvaluate(G.MC);
    if(aux<min) min=aux;
  }
  cout<<min<<endl;
  return 0;
}
