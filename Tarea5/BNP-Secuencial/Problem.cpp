/********************************************************************************
  Authors: Carlos Segura, Emmanuel Romero Ruíz, Gabriel Tadeo Vazquez Ballesteros 
	and Mario Guzman Silverio
*********************************************************************************/

#include "Problem.h"

//Load instances and calculate structures to fast access to information
Problem::Problem(){}
Problem::Problem(string file)
{
    ifstream f(file);
    if (!f.is_open())
    {
      cout<<"Archivo no encontrado";
      exit(EXIT_FAILURE); //Salimos si no es posible abrir el archivo que contiene la estructura del grafo
    }
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
Problem::~Problem(){}
vector<int> Problem::getCost(int source)
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
void Problem::getDistMat(vector<vector<int> > &distances)
{
    for (int i=0;i<nodes;i++)
    {
      distances[i]=getCost(i);
    }
}
int Problem::CostSD(int source, int dest)
{
    //Notemos que el grafo o los nodos comiezan en un índice de 1 sin embargo las computadoras tienen lógica 0 por lo que debemos de convertir nuestro algoritmo a esa lógica
    return MC[source-1][dest-1]; //te regresa el costo mínimo del source al destino
}


