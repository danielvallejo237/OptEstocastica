#include "Individual.h"
//Reservar memoria para la codificacion

#define TFitness long long 
#define TDistance long long 
#define MAX_DISTANCE LLONG_MAX
#define INF 0x3f3f3f3f

using namespace std;

typedef pair<int,int> iPair;

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

/// Necesitamos un vector de heaps actualizables para poder calcular la función de costo por cada una de las soluciones

Container::Container(int nodes)
{
    this->n=nodes;
    vheap.resize(nodes);
}
Container::Container(set<int> s, Problem G)
{
    this->n=G.nodes;
    vheap.resize(G.nodes);
    for(set<int>::iterator it=s.begin();it!=s.end();it++)
    {
      this->InsertOnly(*it,G);
    }
}
void Container::reset(set<int> s, Problem G)
{
    this->clear();
    for(set<int>::iterator it=s.begin();it!=s.end();it++)
    {
      this->InsertOnly(*it,G);
    }
}
bool Container::borra(const int &idx)
{
    /*
    Borramos un elemento por valor a lo largo de todos los 
    */
    bool flag=true;
    for(vector<Uheap>::iterator it=this->vheap.begin();it!=this->vheap.end();++it) flag*=it->remove(idx);
    return flag;
}
void Container::RemoveAndInsert(const int &idx, const int &ns, Problem G)
{
    this->borra(idx);
    for(int i=0;i<n;i++)
    {
      vheap[i].push(make_pair(G.CostSD(i+1,ns),ns));
    }
}
void Container::InsertOnly(const int &ns, Problem G)
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


/// Parte del contenedor adaptado para el nuevo problema
Individual::Individual(const int &nodes, const int &p)
{
    /*Debemos de siempre mantener los índices ordenados por lo que debemos de usar
    una estructura de conjuntos antes que una estructura de vector, el set nos permite encontrar
    ,borrar y modificar las cosas en tiempo logarítmico*/
    this->nodes=nodes;
    this->p=p;
}

void Individual::initialize_heuristic(unsigned int seed)
{
    vector<int> numbers(nodes);
    for (int i=0;i<nodes;i++) numbers[i]=i;
    shuffle(numbers.begin(), numbers.end(), default_random_engine(seed)); //Inicialización de una solución aleatoria
    this->used=set<int>(numbers.begin(),numbers.begin()+p);
    this->notused=set<int>(numbers.begin()+p,numbers.end());
	this->container=new Container(&used,&problem); //Inicializamos al individuo
}

bool Individual::check()
{
    //Siempre nos debemos de asegurar que la solución tenga p datos o p medianas
    return (p==used.size()) && (notused.size()==(nodes-p));
}

void Individual::swap_values_by_pos(int pos_used, int pos_notused)
{
    auto itused=next(used.begin(),pos_used);
    auto itnot=next(notused.begin(),pos_notused);
    int to_remove=*itused;
    int to_insert=*itnot;
    used.erase(to_remove);
    notused.insert(to_remove);
    notused.erase(to_insert);
    used.insert(to_insert);
    if (!check()){cout<<"fail";}
}

void Individual::print()
{
    cout<<"Used"<<endl;
    for (set<int>::iterator it=used.begin();it!=used.end();++it) cout<<*it<<" ";
    cout<<endl;
    cout<<"Not Used"<<endl;
    for (set<int>::iterator it=notused.begin();it!=notused.end();++it) cout<<*it<<" ";
    cout<<endl;
}

TDistance Individual::getDistance(Individual &ind)
{
    /*
    Calculamos el numero de elementos diferentes entre los dos conjuntos y es nuestra distancia
    */
    // Para esto es necesario calcular A NOT B Intersect B NOT A
    set<int> difference;
    set_difference(used.begin(),used.end(),ind.used.begin(),ind.used.end(),inserter(difference,difference.end()));
    return difference.size();
}

void Individual::crossover(Individual &ind)
{
    /*Para la cruza debemos de considerar los elementos que se encuentran en un conjunto pero no en otro
    si cruzamos elementos repetidos en los dos conjuntos habremos modificado el numero de elementos en cada
    uno de las soluciones*/
    //Calculamos A NOT B
    set<int> candidates1;
    for (set<int>::iterator it=used.begin();it!=used.end();++it)
    {
        if(ind.used.find(*it)==ind.used.end())
        {
            candidates1.insert(*it);
        }
    }
    //Calculamos B NOT A
    set<int> candidates2;
    for(set<int>::iterator it=ind.used.begin();it!=ind.used.end();++it)
    {
        if(used.find(*it)==used.end())
        {
            candidates2.insert(*it);
        }
    }
    int D=min(candidates1.size(),candidates2.size());
    float crossoverprob=1.0-1.0/(float)D; //La probabilidad de cruza viene dada por esa fórmula como en el caso de evolución diferencial
    float num;
    for (int i=0;i<D;i++)
    {
        num=(float) rand() / (float) RAND_MAX; 
        if (num<crossoverprob)
        {
            auto it=next(candidates1.begin(),i);
            auto it2=next(candidates2.begin(),i);
            used.insert(*it2);
            used.erase(*it);
            notused.erase(*it2);
            notused.insert(*it);
            ind.used.insert(*it);
            ind.notused.erase(*it);
            ind.used.erase(*it2);
            ind.notused.insert(*it2);
        }
    }
    cout<<(ind.check() && check())<<endl;
}

void Individual::update()
{
	container.reset(this,problem);
}
