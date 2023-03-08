/*
Implementación del algoritmo de recocido simulado para el problema de las p-medianas

Daniel Vallejo Aldana // CIMAT 2023

Nota: El código de la tarea 4 se encuentra al final de este archivo justo antes de comenzar con la función
main()
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
#include <queue>
#include <ctime>
#include <unordered_set>

#define INF 0x3f3f3f3f

using namespace std;

typedef pair<int,int> iPair;

/*
            DECLARACIÓN DE LAS CLASES NECESARIAS PARA LAS TAREAS A RESOLVER

*/

class Graph
{
    private:
    list<iPair> *adj;
    public:
    int p,nodes;
    vector<vector<int> > MC;
    Graph(string file);
    vector<int> getCost(int source);
    void getDistMat(vector<vector<int> > &distances);
    int CostSD(int source, int dest);
    
};

Graph::Graph(string file)
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

vector<int> Graph::getCost(int source)
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
void Graph::getDistMat(vector<vector<int> > &distances)
{
    for (int i=0;i<nodes;i++)
    {
      distances[i]=getCost(i);
    }
}
int Graph::CostSD(int source, int dest)
{
    //Notemos que el grafo o los nodos comiezan en un índice de 1 sin embargo las computadoras tienen lógica 0 por lo que debemos de convertir nuestro algoritmo a esa lógica
    return MC[source-1][dest-1]; //te regresa el costo mínimo del source al destino
}


class Uheap : public priority_queue<iPair,vector<iPair>,greater<iPair> >
{
    public:
    bool remove(const int &idx);
    void screenShow();
    void clear();
};

bool Uheap::remove(const int &idx)
{
    /*
    A nuestra cola de prioridad necesitamos agregar una función que quite parejas de enteros, de acuerdo al índice del 
    nodo que queremos remover para poder llevar a cabo la evaluación incremental de la solución. Así no tenemos que modificar
    todos los costos solo aquellos en los que el nodo agregado sea el más cercano
    */
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

void Uheap::screenShow()
{
    //Mostramos el contenido del contenedor del heap que tiene los elementos
    for(vector<iPair>::iterator it=this->c.begin();it!=this->c.end();++it)
    {
        cout<<" ("<<(*it).first<<" "<<(*it).second<<") ";
    }
    cout<<endl;
}

void Uheap::clear()
{
    while(!this->empty()) this->pop();
}

class descriptor
{
    public:
    int u,nou;
    descriptor(int u,int nou);
    descriptor();
    descriptor create(int u, int nou);
};

descriptor::descriptor(int u,int nou)
{
    this->u=u;
    this->nou=nou;
}

descriptor::descriptor(){}

descriptor descriptor::create(int u, int nou)
{
    return descriptor(u,nou);
}

class Solution
{
    public:
    int nodes,p;
    vector<int> used;
    vector<int> notused;
    Solution(const int &nodes, const int& p, unsigned int seed);
    Solution(Graph G);
    void print();
    void reset();

};

Solution::Solution(const int &nodes, const int &p, unsigned int seed)
{
    /*
    Inicializamos de forma aleatoria las soluciones generando un vector de números únicos 
    y le hacemos un reordenamiento aleatorio, tomamos los p primeros números como nuestra solución candidata
    y el resto de los números como el vector de posibles nodos de reemplazo
    */
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

void Solution::reset()
{
    vector<int> numbers(nodes);
    for(int i=1;i<=nodes;i++) numbers[i-1]=i;
    shuffle(numbers.begin(),numbers.end(),default_random_engine(time(NULL)));
    for(int i=0;i<p;i++) used[i]=numbers[i]; //Nos aseguramos que no haya nodos repetidos
    for (int j=p;j<nodes;j++) notused[j-p]=numbers[j];
}

Solution::Solution(Graph G)
{
    this->p=G.p;
    this->nodes=G.nodes;
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

void Solution::print()
{
    for(vector<int>::iterator it=used.begin();it!=used.end();++it) cout<<*it<<" ";
    cout<<endl;
}

class Container
{
    public:
    vector<Uheap> vheap;
    int n;
    Container(int nodes);
    Container(Solution s, Graph G);
    bool borra(const int &idx);
    void RemoveAndInsert(const int &idx, const int &ns,Graph G);
    void InsertOnly(const int &ns, Graph G);
    void show();
    void reset(Solution s, Graph G);
    void clear();
    int GetLoss();
};
Container::Container(int nodes)
{
    this->n=nodes;
    vheap.resize(nodes);
}
Container::Container(Solution s, Graph G)
{
    this->n=G.nodes;
    vheap.resize(G.nodes);
    for(int i=0;i<s.p;i++)
    {
      this->InsertOnly(s.used[i],G);
    }
}
void Container::reset(Solution s, Graph G)
{
    this->clear();
    for(int i=0;i<s.p;i++)
    {
      this->InsertOnly(s.used[i],G);
    }
}
bool Container::borra(const int &idx)
{
    bool flag=true;
    for(vector<Uheap>::iterator it=this->vheap.begin();it!=this->vheap.end();++it) flag*=it->remove(idx);
    return flag;
}
void Container::RemoveAndInsert(const int &idx, const int &ns,Graph G)
{
    this->borra(idx);
    for(int i=0;i<n;i++)
    {
      vheap[i].push(make_pair(G.CostSD(i+1,ns),ns));
    }
}
void Container::InsertOnly(const int &ns, Graph G)
{
    for(int i=0;i<n;i++)
    {
      vheap[i].push(make_pair(G.CostSD(i+1,ns),ns));
    }
}
void Container::show()
{
    for(int i=0;i<n;i++) vheap[i].screenShow();
}

void Container::clear()
{
    for(int i=0;i<n;i++) vheap[i].clear(); //Hacemos un reset del contenedor para no necesitar mover más cosas
}
int Container::GetLoss()
{
    int loss=0;
    for(vector<Uheap>::iterator it=this->vheap.begin();it!=this->vheap.end();++it) loss+=(*it).top().first;
    return loss;
}

/*
        ALGORITMOS DE BÚSQUEDA LOCAL DE LA TAREA 2
*/

bool FindBetterSolution(Solution &s,Container &C,Graph G,unsigned int seed)
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
        C.RemoveAndInsert(s.used[(*it).u],s.notused[(*it).nou],G);
        aux_cost=C.GetLoss();
        if (aux_cost<current_cost)
        {
          int tmp=s.used[(*it).u];
          s.used[(*it).u]=s.notused[(*it).nou];
          s.notused[(*it).nou]=tmp;
          flag=true;
          break;
        }
        C.RemoveAndInsert(s.notused[(*it).nou],s.used[(*it).u],G);
      }
    return flag;
}

void FindLocalOptimum(Solution &s,Container &C,Graph G,unsigned int seed)
{
    /*
    En cada una de las iteraciones de la función FindBetterSolution se busca evaluar los vecinos de forma aleatoria
    de forma en la que si encontramos un mejor vecino nos movemos ahí
    */
    bool flag=true;
    while(flag){ flag=FindBetterSolution(s,C,G,seed);}
}

/*
        CÓDIGOS DE LA TAREA 3: ENCONTRAR MEJORES SOLUCIONES CANDIDATAS
*/

/*
    Aproximación con programación dinámica:
    -> Modificar la evaluación de las soluciones modificando solamente algunas partes de la solución en lugar de modificar un solo elemento
    -> Necesitamos:
    -> Una guía que nos permita checar que es lo que vamos a ir modificando y que es lo que nos queda por modificar 
    -> Sería un tipo contenedor pero que contenga los índices de que se van a optimizar a lo largo del proceso a partir de una solución aleatoria
*/

class OptimizationContainer
{
    /*Por el momento y porque no se si esto va a funcionar relamente creamos un contenedor de parejas en donde cada pareja corresponde a 
    los dos nodos que se van a optimizar dejando fijos el resto de los nodos, vamos a ir resolviendo múltiples problemas de tamaño
    2 hasta resolver para p*/
    public:
    int p;
    queue<iPair> Order; //Orden en que se optimizarán los p´s
    OptimizationContainer(int p, unsigned int seed);
    void show();
    void reset();
};

OptimizationContainer::OptimizationContainer(int p, unsigned int seed)
{
    //Aquí creamos el orden en que serán considerados los elementos del vector a optimizar
    this->p=p; 
    vector<int> numbers(p);
    for(int i=0;i<p;i++) 
    {
        numbers[i]=i;
    }
    shuffle(numbers.begin(),numbers.end(),default_random_engine(seed)); //Hacemos un shuffle de los números en los cuales vamos a considerar el nuevo orden
    for(int k=0;k<(int)ceil((double)numbers.size()/2.0);k++)
    {
        iPair a(-1,-1);
        a.first=numbers[2*k];
        if(2*k+1 < p) a.second=numbers[2*k+1];
        Order.push(a);
    }
}

void OptimizationContainer::reset()
{
    vector<int> numbers(p);
    while(!Order.empty()) Order.pop(); //Quitamos todos los elementos presentes en la cola
    for(int i=0;i<p;i++) 
    {
        numbers[i]=i;
    }
    shuffle(numbers.begin(),numbers.end(),default_random_engine(time(NULL))); //Hacemos un shuffle de los números en los cuales vamos a considerar el nuevo orden
    for(int k=0;k<(int)ceil((double)numbers.size()/2.0);k++)
    {
        iPair a(-1,-1);
        a.first=numbers[2*k];
        if(2*k+1 < p) a.second=numbers[2*k+1];
        Order.push(a);
    }
}

void OptimizationContainer::show()
{
    queue<iPair> aq=Order;
    while(!aq.empty())
    {
        cout<<" ( "<<aq.front().first<<" "<<aq.front().second<<" ) ";
        aq.pop();
    }
    cout<<endl;
}

/*
        Parte de Optimización: Vamos a encontrar una buena solución por pares con base en una pareja que elijamos
        La forma en la que vamos a proceder con la optimización es que vamos a generar una pareja de descriptores 
        en la que definiremos que parejas de vectores vamos a intercambiar.

        NOTA: No hay algo que avale que esta solución funcionará pero es lo que por el momento se propone 
*/

typedef pair<descriptor,descriptor> Dpair; // Necesitamos una pareja de descriptores que es la que nos va a controlar el cambio

bool FindPairedBetterSolution(iPair toMove, Solution &s, Graph G,Container &C, unsigned int seed)
{
    /*
    Como parametros de entrada necesitamos una pareja de numeros que son las posiciones que vamos a mantener fijas dentro de nuestro arreglo de soluciones
    la solución actual que es sobre la que vamos a modificar los valores existentes y un contenedor inicializado con la solución aleatoria
    que es lo que nos va  dar la función de costo de la solución actual que hay que mejorar mediante búsqueda local
    */
    bool flag=false;
    if(toMove.second==-1)
    {
        //No tenemos entonces una pareja que mover solamente vamos a mover 
        vector<descriptor> des(s.notused.size());
        int index=toMove.first;
        descriptor ds;
        int counter=0;
        for(vector<int>::iterator it=s.notused.begin();it!=s.notused.end();++it)
        {
            des[counter]=ds.create(index,counter);
            counter++;
        }
        int current_cost=C.GetLoss();
        int aux_cost;
        shuffle(des.begin(),des.end(),default_random_engine(seed));
        for(vector<descriptor>::iterator it=des.begin();it!=des.end();++it)
        {
            C.RemoveAndInsert(s.used[(*it).u],s.notused[(*it).nou],G);
            aux_cost=C.GetLoss();
            if (aux_cost<current_cost)
            {
                int tmp=s.used[(*it).u];
                s.used[(*it).u]=s.notused[(*it).nou];
                s.notused[(*it).nou]=tmp;
                flag=true;
                break;
            }
            C.RemoveAndInsert(s.notused[(*it).nou],s.used[(*it).u],G); //Si no mejora la solución regresamos al estado anterior en el que nos encontrábamos
        }
        return flag;
    }
    else
    {
        vector<Dpair> des(2*(s.notused.size()-1)); //Vamos a contar los intercambios entre pares de numeros de esta forma tenemos 
        descriptor ds1,ds2; //En este caso debemos de guardar una pareja de descriptores que es la que vamos a ir optimizando
        int counter=0;
        for (int i=0;i<s.notused.size()-1;i++)
        {
            des[counter]=make_pair(ds1.create(toMove.first,i),ds2.create(toMove.second,i+1));
            des[counter+1]=make_pair(ds1.create(toMove.first,i+1),ds2.create(toMove.second,i));
            counter+=2;
        }
        int current_cost=C.GetLoss();
        int aux_cost;
        shuffle(des.begin(),des.end(),default_random_engine(seed));
        for(vector<Dpair>::iterator it=des.begin();it!=des.end();++it)
        {
            C.RemoveAndInsert(s.used[(*it).first.u],s.notused[(*it).first.nou],G); //Modificacion del primer descriptor
            C.RemoveAndInsert(s.used[(*it).second.u],s.notused[(*it).second.nou],G);
            aux_cost=C.GetLoss();
            if (aux_cost<current_cost)
            {

                int tmp1=s.used[(*it).first.u];
                int tmp2=s.used[(*it).second.u];
                s.used[(*it).first.u]=s.notused[(*it).first.nou];
                s.used[(*it).second.u]=s.notused[(*it).second.nou];
                s.notused[(*it).first.nou]=tmp1;
                s.notused[(*it).second.nou]=tmp2;
                flag=true;
                break;
            }
            C.RemoveAndInsert(s.notused[(*it).first.nou],s.used[(*it).first.u],G);
            C.RemoveAndInsert(s.notused[(*it).second.nou],s.used[(*it).second.u],G);
        }
        return flag;
    }
}

void FindPairedLocalOptimum(iPair toMove, Solution &s, Graph G,Container &C, unsigned int seed)
{
    bool flag=true;
    while(flag)
    {
        flag=FindPairedBetterSolution(toMove,s,G,C,seed);
    }
}

void FindOptimumDynamicProgramming(OptimizationContainer &O , Solution &s, Graph G, Container &C, unsigned int seed)
{
    /*
    Esta es la función principal relacionada con la optimización de tipo programación dinámica, vamos a ir optimizando por cachos en este caso cachos de tamano 
    2, resolveremos el problema para dichos casos y luego procederemos a resolver los problemas siguientes de acuerdo al contenedor de optimizacion asociado. 
    De los experimentos que realizamos si vemos que al menos el tiempo de ejecución del algoritmo es significativamente más rápido que el 
    de solamente usar movimeintos aleatorios de las cosas
    */
   while(!O.Order.empty())
   {
    //Mientras tengamos parejas que optimizar  seguiremos el proceso
    /*cout<<" Optimizando parejas de nodos: "<<O.Order.front().first<<" "<<O.Order.front().second<<endl;
    cout<<" Costo hasta el momento: "<<C.GetLoss()<<endl;*/
    //cout<<"Pairs left: "<<O.Order.size()<<endl;
    FindPairedLocalOptimum(O.Order.front(),s,G,C,seed);
    O.Order.pop();
   }
}


/* 
                CÓDIGO DE LA TAREA 4, RECOCIDO SIMULADO
*/


descriptor gen_random_descriptor(const int &n,const int &p)
{
    int inp=random() % p;
    int pos=random() % (n-p);
    descriptor des(inp,pos);
    return des;
}
void SimulatedAnnealing(Solution &sol, float temperature,int RepCounter, Container &C, Graph G,float delayInSeconds, float decreaseRate)
{
    /*
    Describiremos paso por paso el algoritmo de recocido simulado para la tarea de las p medianas
    temperature-> Tempreatura inicial del algoritmo de recocido simulado
    Repcounter -> Cuántas veces seleccionaremos vecinos de forma aleatoria para evaluar la función de costo
    seed -> la semilla con la cual será inicializada la generación de números aleatorios 
    Pasos 1 al 3: Seleccionar una solución y una temperatura inicial y definir el cambio de la temperatura a 0
    delayInSeconds -> Tiempo el cual será ejecutado el algoritmo de recocido simulado en este caso corresponde a 1200 segs
    decreaseRate -> Tasa de decaimiento de la temperatura 
    */
   int tChange=0;
   //Se dice que el algoritmo deberá de ser corrido por 20 minutos
   time_t startTime;
   time_t now;
   float elapsedTime=0;
   float setTime = delayInSeconds;
   time(&startTime);
   srand(time(NULL)); //Inicialización de forma aleatoria conforme al tiempo máquina 
   descriptor auxdes;
   float indicator;
   int aux_cost,delta;
   int current_cost=C.GetLoss(); //Sacamos el costo actual de la función de pérdida
   while (elapsedTime < setTime) 
   {

        for(int i=0;i<RepCounter;i++)
        {
            //De acuerdo al algoritmo de recocido simulado debemos de generar una solución aleatoria del vecindario que vamos a considerar
            auxdes=gen_random_descriptor(sol.nodes,sol.p);
            C.RemoveAndInsert(sol.used[auxdes.u],sol.notused[auxdes.nou],G);
            aux_cost=C.GetLoss();
            delta=aux_cost-current_cost;
            if(delta < 0)
            {
                //Rompemos el ciclo y nos pasamos a la nueva solución donde generaremos un nuevo vecindario
                int tmp=sol.used[auxdes.u];
                sol.used[auxdes.u]=sol.notused[auxdes.nou];
                sol.notused[auxdes.nou]=tmp;
                current_cost=aux_cost;
                break;
            }
            else if(delta >= 0 && temperature >0)
            {
                indicator= (float) rand() / RAND_MAX; //Generamos un aleatorio entre 0 y 1
                if(indicator < max((float)0.0,exp(-delta/temperature))) 
                {
                    int tmp=sol.used[auxdes.u];
                    sol.used[auxdes.u]=sol.notused[auxdes.nou];
                    sol.notused[auxdes.nou]=tmp;
                    current_cost=aux_cost;
                    break;
                } 
            }
            C.RemoveAndInsert(sol.notused[auxdes.nou],sol.used[auxdes.u],G);
        }
        //Incrememntar el tamaño de los cambios
        tChange=tChange+1;
        temperature=max((float)0.0,decreaseRate-((decreaseRate*tChange)/RepCounter));
        now = time(NULL);
        elapsedTime = difftime(now, startTime);
    }

}

int TimedDynamicProgrammingOptFinding(Graph G, unsigned int seed, float delayInSeconds)
{
    /*Creamos métodos reset del contenedor y de la solución de forma que no ocupemos más
    memoria de la necesaria*/
    time_t startTime;
    time_t now;
    float elapsedTime=0;
    float setTime = delayInSeconds;
    time(&startTime);
    Solution sol(G.nodes,G.p,seed);
    Container C(sol,G);
    OptimizationContainer O(G.p,seed);
    int min=INF;
    int aux_cost;
    while(elapsedTime < setTime)
    {
        FindOptimumDynamicProgramming(O,sol,G,C,seed);
        aux_cost=C.GetLoss();
        if(aux_cost<min) min=aux_cost;
        sol.reset();
        C.reset(sol,G);
        O.reset();
        now = time(NULL);
        elapsedTime = difftime(now, startTime);
    }
    return min;
}


/*                PROPUESTA: VNS con Recocido simulado en donde el tamaño de la vecindad puede reducirse de dos formas

                    1: Por mejora en la función de fitness
                    2: Por un scheduler de decaimiento con una tasa multiplicativa
*/

vector<descriptor> genMultipleDescriptors(const int &n, const int &p, const int &num)
{
    vector<descriptor> descs(num);
    unordered_set<int> fs;
    unordered_set<int> fs2;
    int counter=0;
    descriptor ds;
    while(fs.size()!=num)
    {
        fs.insert(random() % p);
    }
    while(fs2.size()!=num)
    {
        fs2.insert(random() % (n-p));
    }
    for(unordered_set<int>::iterator it=fs.begin(),it2=fs2.begin();it!=fs.end();++it,++it2)
    {
        descs[counter]=ds.create(*it,*it2);
        counter++;
    }

    return descs;
}

// Decaimiento del tamaño de la vecindad en caso de que una solución mejore

void VNSSAonPlateu(Solution &sol, float temperature,int RepCounter, Container &C, Graph G,float delayInSeconds, float decreaseRate, int max_n_size)
{
    /*La primera forma que vamos a probar de decaimiento es en cuanto a mejora de la solución si la solución mejora, hacemos un decaímiento proporcional
    a la mejora hasta quedarnos con una vecindad más pequeña y seguirle con el algoritmo de recocido simulado que ya conocíamos
    
    max_n_size -> Al algoritmo previamente definido le definimos el tamaño máximo de la vecindad que queremos considerar en este caso
    se considera una vecindad de a lo más 3 nodos por cuestiones de poder computacional.
    */
    int tChange=0;
   //Se dice que el algoritmo deberá de ser corrido por 20 minutos
   time_t startTime;
   time_t now;
   float initial_rate=1.0;
   float elapsedTime=0;
   float setTime = delayInSeconds;
   time(&startTime);
   srand(time(NULL)); //Inicialización de forma aleatoria conforme al tiempo máquina 
   descriptor auxdes;
   float indicator;
   int aux_cost,delta;
   vector<descriptor> vedes;
   int current_cost=C.GetLoss(); //Sacamos el costo actual de la función de pérdida
   while (elapsedTime < setTime) 
   {

        for(int i=0;i<RepCounter;i++)
        {
            //De acuerdo al algoritmo de recocido simulado debemos de generar una solución aleatoria del vecindario que vamos a considerar
            vedes=genMultipleDescriptors(sol.nodes,sol.p,1+(int)(initial_rate*(float)max_n_size));
            for(int i=0;i<vedes.size();i++)
            {
                C.RemoveAndInsert(sol.used[vedes[i].u],sol.notused[vedes[i].nou],G);
            }
            aux_cost=C.GetLoss();
            delta=aux_cost-current_cost;
            if(delta < 0)
            {
                //Rompemos el ciclo y nos pasamos a la nueva solución donde generaremos un nuevo vecindario
                int tmp;
                for(int i=0;i<vedes.size();i++)
                {
                    tmp=sol.used[vedes[i].u];
                    sol.used[vedes[i].u]=sol.notused[vedes[i].nou];
                    sol.notused[vedes[i].nou]=tmp;
                }
                initial_rate=(fabs(delta)/current_cost);
                current_cost=aux_cost;
                break;
            }
            else if(delta >= 0 && temperature >0)
            {
                indicator= (float) rand() / RAND_MAX; //Generamos un aleatorio entre 0 y 1
                if(indicator < max((float)0.0,exp(-delta/temperature))) 
                {
                    int tmp;
                    for(int i=0;i<vedes.size();i++)
                    {
                        tmp=sol.used[vedes[i].u];
                        sol.used[vedes[i].u]=sol.notused[vedes[i].nou];
                        sol.notused[vedes[i].nou]=tmp;
                    }
                    current_cost=aux_cost;
                    break;
                } 
            }
            for(int i=0;i<vedes.size();i++) C.RemoveAndInsert(sol.notused[vedes[i].nou],sol.used[vedes[i].u],G);
        }
        //Incrememntar el tamaño de los cambios
        tChange=tChange+1;
        temperature=max((float)0.0,decreaseRate-((decreaseRate*tChange)/RepCounter));
        now = time(NULL);
        elapsedTime = difftime(now, startTime);
    }

}


//Mejora del algoritmo con un tamaño de paso predeterminado

void VNSSAMultiplicativeReduction(Solution &sol, float temperature,int RepCounter, Container &C, Graph G,float delayInSeconds, float decreaseRate, int max_n_size)
{
    /*La primera forma que vamos a probar de decaimiento es en cuanto a mejora de la solución si la solución mejora, hacemos un decaímiento proporcional
    a la mejora hasta quedarnos con una vecindad más pequeña y seguirle con el algoritmo de recocido simulado que ya conocíamos
    
    max_n_size -> Al algoritmo previamente definido le definimos el tamaño máximo de la vecindad que queremos considerar en este caso
    se considera una vecindad de a lo más 3 nodos por cuestiones de poder computacional.
    */
    int tChange=0;
   //Se dice que el algoritmo deberá de ser corrido por 20 minutos
   time_t startTime;
   time_t now;
   float initial_rate=1.0;
   float lr=0.01;
   float elapsedTime=0;
   float setTime = delayInSeconds;
   time(&startTime);
   srand(time(NULL)); //Inicialización de forma aleatoria conforme al tiempo máquina 
   descriptor auxdes;
   float indicator;
   int aux_cost,delta;
   vector<descriptor> vedes;
   int current_cost=C.GetLoss(); //Sacamos el costo actual de la función de pérdida
   while (elapsedTime < setTime) 
   {

        for(int i=0;i<RepCounter;i++)
        {
            //De acuerdo al algoritmo de recocido simulado debemos de generar una solución aleatoria del vecindario que vamos a considerar
            vedes=genMultipleDescriptors(sol.nodes,sol.p,1+(int)(initial_rate*(float)max_n_size));
            for(int i=0;i<vedes.size();i++)
            {
                C.RemoveAndInsert(sol.used[vedes[i].u],sol.notused[vedes[i].nou],G);
            }
            aux_cost=C.GetLoss();
            delta=aux_cost-current_cost;
            if(delta < 0)
            {
                //Rompemos el ciclo y nos pasamos a la nueva solución donde generaremos un nuevo vecindario
                int tmp;
                for(int i=0;i<vedes.size();i++)
                {
                    tmp=sol.used[vedes[i].u];
                    sol.used[vedes[i].u]=sol.notused[vedes[i].nou];
                    sol.notused[vedes[i].nou]=tmp;
                }
                current_cost=aux_cost;
                break;
            }
            else if(delta >= 0 && temperature >0)
            {
                indicator= (float) rand() / RAND_MAX; //Generamos un aleatorio entre 0 y 1
                if(indicator < max((float)0.0,exp(-delta/temperature))) 
                {
                    int tmp;
                    for(int i=0;i<vedes.size();i++)
                    {
                        tmp=sol.used[vedes[i].u];
                        sol.used[vedes[i].u]=sol.notused[vedes[i].nou];
                        sol.notused[vedes[i].nou]=tmp;
                    }
                    current_cost=aux_cost;
                    break;
                } 
            }
            for(int i=0;i<vedes.size();i++) C.RemoveAndInsert(sol.notused[vedes[i].nou],sol.used[vedes[i].u],G);
        }
        //Incrememntar el tamaño de los cambios
        tChange=tChange+1;
        temperature=max((float)0.0,decreaseRate-((decreaseRate*tChange)/RepCounter));
        initial_rate=max((float)0.0,initial_rate-lr);
        now = time(NULL);
        elapsedTime = difftime(now, startTime);
    }

}

int main(int argc, char *argv[])
{
    vector<int> costs(4,0);
    Graph G(argv[1]);
    Solution sol(G.nodes,G.p,atoi(argv[2]));
    Container C(sol,G);
    SimulatedAnnealing(sol,1.25,3000,C,G,10,1.0);
    costs[0]=C.GetLoss();
    sol.reset();
    C.reset(sol,G);
    VNSSAMultiplicativeReduction(sol,1.25,3000,C,G,10,1.0,3);
    costs[1]=C.GetLoss();
    sol.reset();
    C.reset(sol,G);
    VNSSAonPlateu(sol,1.25,3000,C,G,10,1.0,3);
    costs[2]=C.GetLoss();
    int c=TimedDynamicProgrammingOptFinding(G,atoi(argv[2]),10);
    costs[3]=c;
    cout<<costs[0]<<" "<<costs[1]<<" "<<costs[2]<<" "<<costs[3]<<endl;
    return 0;
}
