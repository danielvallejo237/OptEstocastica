#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstring>
#include <fstream>

#define INF 10000000
using namespace std;


class Graph
{
public:
private:
  vector<vector<int> > adjacencia;
  int p,nodes;
public:
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
    adjacencia=InicializaMat(nodes);
    cout<<"Nodos: "<<nodes<<endl<<"Aristas: "<<edges<<endl;
    for (int i=0;i<edges;i++)
    {
      int a,b,c;
      f>>a>>b>>c;
      cout<<a<<" "<<b<<" "<<c<<endl;
      adjacencia[a-1][b-1]=c;
      adjacencia[b-1][a-1]=c; //Tenemos una matriz simétrica como es la matriz de adjacencia
    }
  }
  vector<vector<int> > InicializaMat(int nodes)
  {
    vector<vector<int> > M(nodes,vector<int> (nodes,INF));
    return M;
  }
  bool areConnected(int i, int j)
  {
    return !(this->adjacencia[i-1][j-1]==INF);
  }
  int getCost(int i, int j)
  {
    return this->adjacencia[i-1][j-1];
  }
  int nNodes()
  {
    return this->nodes;
  }
};

class Instancia
{
public:
  int p;
  vector<int> ind;
  Instancia(int Nodes,int p)
  {
    //Recibimos las restricciones de numero de nodos y de numero de instalaciones
    ind.resize(Nodes,0);
  }
  // Generamos una solución candidata para posteriormente evaluar dicha solución
}

int main(int argc,char* argv[])
{
  Graph G(argv[1]);
  cout<<G.areConnected(1,2)<<" "<<G.getCost(1,2)<<endl;
  return 0;
}
