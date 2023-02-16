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

class descriptor
{
public:
  int u; // u corresponde al índice en el vector de elementos que vamos a cambiar
  int nou; //nou corresponde al índice del vector de elementos no utilizados que vamos a cambiar
  //La idea es que para generar vecinos vamos a crear un inercambio de la solución actual con algun otro nodo
  descriptor(int u, int nou)
  {
    this->u=u;
    this->nou=nou;
  }
  descriptor(){}
  descriptor create(int u, int nou)
  {
    return descriptor(u,nou);
  }
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
  void print()
  {
    for(vector<int>::iterator it=used.begin();it!=used.end();++it) cout<<*it<<" ";
      cout<<endl;
  }
  Solution(Graph G)
  {
    //Iniciamos una solución mediante una heurística de construcción para esto necesitamos la matriz de costo
    /*Calculamos la solucion inicial como los p nodos cuya distancia promedio al resto es menor que los demás
    en este caso solo debemos de ordenar dichas distancias */
    this->p=G.pVal();
    this->nodes=G.nNodes();
    this->used.resize(this->p);
    this->notused.resize(this->nodes-this->p);
    //Sumamos por filas la matriz de adyacencia de tal forma que obtengamos el promedio de un nodo a otro
    vector<iPair> additions(this->nodes,make_pair(0,0));
    for(int i=0;i<this->nodes;i++)
    {
      additions[i].second=i+1;
      for(int j=0;j<this->nodes;j++) additions[i].first+=G.MC[i][j];
    }
    sort(additions.begin(),additions.end(),greater<iPair>());
    reverse(additions.begin(),additions.end());
    for(int i=0;i<this->p;i++) this->used[i]=additions[i].second;
    for(int i=0;i<this->nodes-this->p;i++) this->notused[i]=additions[this->p+i].second;
  }
};
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
      vheap[i].push(make_pair(G.CostSD(i+1,ns),ns));
    }
  }
  void insertOnly(const int &ns, Graph G)
  {
    for(int i=0;i<n;i++)
    {
      vheap[i].push(make_pair(G.CostSD(i+1,ns),ns));
    }
  }
  void show()
  {
    for(int i=0;i<n;i++) ShowUheap(vheap[i]);
  }
  int GetLoss()
  {
    int loss=0;
    for(vector<Uheap>::iterator it=this->vheap.begin();it!=this->vheap.end();++it) loss+=(*it).top().first;
    return loss;
  }
  Container(Solution s, Graph G)
  {
    //Creamos un contenedor principal a partir de una solución
    this->n=G.nNodes();
    vheap.resize(G.nNodes());
    for(int i=0;i<s.p;i++)
    {
      this->insertOnly(s.used[i],G);
    }
  }
};

 bool findBetterSolution(Solution &s,Container &C,Graph G,unsigned int seed)
  {
    //Primero generamos los vecinos que corresponde a una lista de descriptores
    vector<descriptor> dv(s.nodes-s.p);
    int index=rand() % s.p; //Algún número vamos a cambiar de la solución
    int counter=0;
    descriptor ds;
    bool flag=false; //Hasta que no encontremos una mejor solución continuaremos buscando nuevas soluciones
    for(vector<int>::iterator it=s.notused.begin();it!=s.notused.end();++it)
    {
      dv[counter]=ds.create(index,counter);
      counter++;
    }
    int current_cost=C.GetLoss();
    int aux_cost;
    shuffle(dv.begin(),dv.end(),default_random_engine(seed)); //Se supone que con eso estamos quitando los vectores de forma aleatorio
    for(vector<descriptor>::iterator it=dv.begin();it!=dv.end();++it)
      {
        C.removeAndInsert(s.used[(*it).u],s.notused[(*it).nou],G);
        aux_cost=C.GetLoss();
        if (aux_cost<current_cost)
        {
          int tmp=s.used[(*it).u];
          s.used[(*it).u]=s.notused[(*it).nou];
          s.notused[(*it).nou]=tmp;
          flag=true;
          break;
        }
        C.removeAndInsert(s.notused[(*it).nou],s.used[(*it).u],G);
      }
    return flag;
  }

void findLocalOptimum(Solution &s,Container &C,Graph G,unsigned int seed)
{
  bool flag=true;
  while(flag){ flag=findBetterSolution(s,C,G,seed);}
  //cout<<C.GetLoss()<<endl;
}

int main(int argc, char* argv[])
{
  Graph G(argv[1]);
  Solution sol(G.nNodes(),G.pVal(),atoi(argv[2]));
  Container C(sol,G);
  auto start = chrono::steady_clock::now();
  findLocalOptimum(sol,C,G,atoi(argv[2]));
 auto end = chrono::steady_clock::now();
  Solution sc(G);
  Container C2(sc,G);
    auto start1 = chrono::steady_clock::now();
  findLocalOptimum(sc,C2,G,atoi(argv[2]));
    auto end1 = chrono::steady_clock::now();
  //cout<<C.GetLoss()<<" "<<C2.GetLoss()<<endl;
    cout<<chrono::duration<double>(end - start).count()<<" "<<chrono::duration<double>(end1 - start1).count()<<endl;
  return 0;
}
