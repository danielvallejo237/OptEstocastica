/*
Códigos de la tarea 2 de optimización estocástica
@Author Daniel Vallejo Aldana / CIMAT

NOTA: Para hacer el código más legible y entendible realizaremos una programación modular en la cual 
explicaremos paso a paso el desarrollo de cada parte del algoritmo de forma que se pueda entender el 
proceso de desarrollo del mismo
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


/* Creación del heap actualizable con la posibilidad de borrar un nodo de acuerdo a una llave 
que en este caso es el índice del nodo que queremos borrar */

class Uheap : public priority_queue<iPair,vector<iPair>,greater<iPair> >
{
public:
  bool remove(const int &idx)
  {
    //Notemos que en este caso el índice va en segunda posición que corresponde al nodo con el que estamos conectando
    vector<iPair>::iterator it = find_if(this->c.begin(),this->c.end(),[&idx](const iPair& e) {return e.second == idx;});
    if(it==c.end()) return false; //No encontramos el elemento dentro de la cola de prioridad
    else
    {
      if (it==this->c.begin()) this->pop();
      else
      {
        this->c.erase(it);
        make_heap(this->c.begin(),this->c.end(),this->comp);
      }
    }
    return true;
  }
}; 


void ShowUheap(Uheap pq)
{
  Uheap p=pq;
  while(!p.empty())
  {
    cout<<" ("<<p.top().first<<" "<<p.top().second<<") ";
    p.pop();
  }
  cout<<endl;
}

/*En este caso tenemos un contenedor el cual está conformado por un vector de heaps actualizables donde 
cada uno de ellos */
class Container
{
public:
  vector<Uheap> vheap;
  int n;
  Container(int nodes)
  {
    this->n=nodes;
    vheap.resize(nodes);
    cout<<"Tamano del contenedor: "<<vheap.size()<<endl;
  }
  bool borra(const int &idx)
  {
    bool flag=true;
    for(vector<Uheap>::iterator it=this->vheap.begin();it!=this->vheap.end();++it) flag*=it->remove(idx);
    return flag;
  }

  void removeAndInsert(const int &idx, const int &ns,Graph G)
  {
    this->borra(idx);
    for(int i=0;i<n;i++)
    {
      vheap[i].push(make_pair(G.CostSD(i+1,ns+1),ns));
    }
  }
  void insertOnly(const int &ns, Graph G)
  {
    for(int i=0;i<n;i++)
    {
      vheap[i].push(make_pair(G.CostSD(i+1,ns+1),ns));
    }
  }
  void show()
  {
    for(int i=0;i<n;i++) ShowUheap(vheap[i]);
  }
  int GetLoss()
  {
    int loss=0;
    for (int i=0;i<n;i++) loss+=vheap[i].top().first;
    return loss;
  }
  Container(Solution s, Graph G)
  {
    //Creamos un contenedor principal a partir de una solución
    this->n=s.nNodes();
    vheap.resize(nodes);
    cout<<"Tamano del contenedor: "<<vheap.size()<<endl;
    for(int i=0;i<s.p;i++)
    {
      this->insertOnly(s.used[i]);
    }
  }
};


typedef struct descriptor
{
  int u; // u corresponde al índice en el vector de elementos que vamos a cambiar
  int nou; //nou corresponde al índice del vector de elementos no utilizados que vamos a cambiar
  //La idea es que para generar vecinos vamos a crear un inercambio de la solución actual con algun otro nodo
};


class Solution
{
public:
  int nodes;
  int p;
  vector<int> used;
  vector<int> notused;
  Solution(const int &nodes, const int& p, unsigned int seed)
  {
    this->p=p;
    this->nodes=nodes;
    this->used.resize(p);
    this->notused.resize(nodes-p);
    vector<int> numbers(nodes);
    for(int i=1;i<=nodes;i++) numbers[i-1]=i;
    std::shuffle(numbers.begin(), numbers.end(), std::default_random_engine(seed));
    for(int i=0;i<p;i++) used[i]=numbers[i]; //Nos aseguramos que no haya nodos repetidos
    for (int j=p;j<nodes;j++) notused[j-p]=numbers[j];
  }
  bool findBetterSolution()
  {
    //Primero generamos los vecinos que corresponde a una lista de descriptores
    
  }


}


int main(int argc, char* argv[])
{
  Graph G(argv[1]);
  cout<<"Creando grafo"<<endl;
  Container C(G.nNodes());
  C.insertOnly(10,G);
  C.insertOnly(20,G);
  C.show();
  cout<<C.GetLoss()<<endl;
  C.borra(10);
  C.show();
  return 0;
}