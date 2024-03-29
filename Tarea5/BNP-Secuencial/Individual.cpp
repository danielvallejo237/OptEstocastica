#include "Individual.h"
#include "Problem.h"
//Reservar memoria para la codificacion

#define TFitness long long 
#define TDistance long long 
#define MAX_DISTANCE LLONG_MAX
#define INF 0x3f3f3f3f

using namespace std;

typedef pair<int,int> iPair;
pair<int,int> gen_random_descriptor(const int &n,const int &p)
{
    int inp=random() % p;
    int pos=random() % (n-p);
    return make_pair(inp,pos);
}

int get_element_by_index(unordered_set<int> &s,int index)
{
    auto it=next(s.begin(),index); //Sacamos un elemento del conjunto no ordenado
    return *it; 
}

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
Container::Container(){}
Container::Container(unordered_set<int> s, Problem G)
{
    this->n=G.nodes;
    vheap.resize(G.nodes);
    for(unordered_set<int>::iterator it=s.begin();it!=s.end();it++)
    {
      this->InsertOnly(*it,G);
    }
}
void Container::reset(unordered_set<int> s, Problem G)
{
    this->clear();
    for(unordered_set<int>::iterator it=s.begin();it!=s.end();it++)
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

/*
Definimos a la clase individuo que es la clase que corresponde a cada indivivuo de la población
Cada individuo debe de tener la clase de intensificación y de obtención de su función de costo.
*/
Individual::Individual(const int &nodes, const int &p,Problem &G)
{
    /*Debemos de siempre mantener los índices ordenados por lo que debemos de usar
    una estructura de conjuntos antes que una estructura de vector, el set nos permite encontrar
    ,borrar y modificar las cosas en tiempo logarítmico*/
    this->nodes=nodes;
    this->p=p;
    this->problem=G;
    this->distance=MAX_DISTANCE;
}

void Individual::initialize_heuristic(unsigned int seed)
{
    vector<int> numbers(nodes);
    for (int i=1;i<=nodes;i++) numbers[i-1]=i;
    shuffle(numbers.begin(), numbers.end(), default_random_engine(seed)); //Inicialización de una solución aleatoria
    this->used=unordered_set<int>(numbers.begin(),numbers.begin()+p);
    this->notused=unordered_set<int>(numbers.begin()+p,numbers.end());
	this->container=Container(used,problem); //Inicializamos al con su respectivo contenedor para poder calcular la pérdida
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
    this->update();
}

void Individual::print(const string &fileName, const int &min_mean_dist)
{
    /*
    Lo que se va a imprimir en el fichero o en el archivo de texto es la solución como tal al problema así como 
    el costo de dicha solución que será reportado en las tablas del final
    */
    ofstream fich(fileName);
    for (unordered_set<int>::iterator it=used.begin();it!=used.end();++it) fich<<*it<<" ";
    fich<<endl;
    fich<<getCost()<<endl<<min_mean_dist<<endl;
    fich.close();
}

void Individual::print_to_console()
{
    cout<<"Solucion: "<<endl;
    for (unordered_set<int>::iterator it=used.begin();it!=used.end();++it) cout<<*it<<" ";
    cout<<endl;
    cout<<getCost()<<endl;
}

TDistance Individual::getDistance(Individual &ind)
{
    /*
    Calculamos el numero de elementos diferentes entre los dos conjuntos y es nuestra distancia
    */
    // Para esto es necesario calcular A NOT B Intersect B NOT A
    unordered_set<int> difference;
    set_difference(used.begin(),used.end(),ind.used.begin(),ind.used.end(),inserter(difference,difference.end()));
    return difference.size();
}

void Individual::crossover(Individual &ind)
{
    /*Para la cruza debemos de considerar los elementos que se encuentran en un conjunto pero no en otro
    si cruzamos elementos repetidos en los dos conjuntos habremos modificado el numero de elementos en cada
    uno de las soluciones*/
    //Calculamos A NOT B
    unordered_set<int> candidates1;
    for (unordered_set<int>::iterator it=used.begin();it!=used.end();++it)
    {
        if(ind.used.find(*it)==ind.used.end())
        {
            candidates1.insert(*it);
        }
    }
    //Calculamos B NOT A
    unordered_set<int> candidates2;
    for(unordered_set<int>::iterator it=ind.used.begin();it!=ind.used.end();++it)
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
    //cout<<(ind.check() && check())<<endl;
    this->update();
    ind.update();
}

void Individual::update()
{
	container.reset(this->used,problem);
}

TFitness Individual::getCost()
{
    return (TFitness) this->container.GetLoss();
}

void Individual::intensify()
{
    /*
    Para la parte del algoritmo memético vamos a regresar usar el algoritmo de recocido simulado
    para poder encontrar mejores soluciones a las que ya se tienen. 

    */
    float temperature=1.25;
    float delayInSeconds=1;
    float decreaseRate=0.9;
    int RepCounter=3000;
    int tChange=0;
   //Se dice que el algoritmo deberá de ser corrido por 20 minutos
    time_t startTime;
    time_t now;
    float elapsedTime=0;
    float setTime = delayInSeconds;
    time(&startTime);
    srand(time(NULL)); //Inicialización de forma aleatoria conforme al tiempo máquina 
    pair<int,int> auxdes; //
    float indicator;
    int aux_cost,delta;
    TFitness current_cost=getCost(); //Sacamos el costo actual de la función de pérdida
    while (elapsedTime< setTime)
    {
        //El proceso de intensificación no debe de durar más de un segundo
        for (int i=0;i<RepCounter;i++)
        {
            auxdes=gen_random_descriptor(nodes,p);
            container.RemoveAndInsert(get_element_by_index(used,auxdes.first),get_element_by_index(notused,auxdes.second),problem);
            aux_cost=container.GetLoss();
            delta=aux_cost-current_cost;
            if(delta < 0)
            {
                swap_values_by_pos(auxdes.first,auxdes.second);
                update();
                current_cost=getCost(); //Ya hemos actualizado el costo a la nueva solución
                break;
            }
            else if(delta >= 0 && temperature >0)
            {
                indicator= (float) rand() / RAND_MAX; //Generamos un aleatorio entre 0 y 1
                if(indicator < max((float)0.0,exp(-delta/temperature))) 
                {
                    swap_values_by_pos(auxdes.first,auxdes.second);
                    update();
                    current_cost=getCost(); //Ya hemos actualizado el costo a la nueva solución
                    break;
                }
            }
            container.RemoveAndInsert(get_element_by_index(notused,auxdes.second),get_element_by_index(used,auxdes.first),problem);
        }
        tChange=tChange+1;
        temperature=max((float)0.0,decreaseRate-((decreaseRate*tChange)/RepCounter));
        now = time(NULL);
        elapsedTime = difftime(now, startTime);
    }
}
