#include "fitness.h"

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
Container::Container(Individual &s, Problem &G)
{
    this->n=G.nodes;
    vheap.resize(G.nodes);
    for(set<int>::iterator it=s.used.begin();it!=s.used.end();it++)
    {
      this->InsertOnly(*it,G);
    }
}
void Container::reset(Individual &s, Graph &G)
{
    this->clear();
    for(set<int>::iterator it=s.used.begin();it!=s.used.end();it++)
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