// File: graph.h
// Desc:
// Auth: Carlos Barreiro
// Mail: a20360@alunos.ipca.pt
// Date: 2025/05

#ifndef GRAPH_H
#define GRAPH_H

#include "aerial.h"
#include <stdbool.h>

typedef struct Adjacency
{
    ED *dst;
    float weight;
    struct Adjacency *next;
} Adjacency;

typedef struct Vertex
{
    ED *aerial;
    Adjacency *adjList;
} Vertex;

typedef struct GR
{
    int vertexNum;
    Vertex *vertexList;
} GR;

// Protótipos das funções
GR *createGR(ED *aerialList);                            // Cria um grafo a partir da lista de antenas
bool addEdge(GR *graph, ED *src, ED *dst, float weight); // Adiciona uma aresta ao grafo
bool buildResonanceGR(GR *graph, ED *aerialList);        // Constrói o grafo
int printGR(const GR *graph);                            // Mostra o grafo
bool freeGR(GR *graph);                                  // Liberta a memória do grafo
void dfsGR(GR *graph, int startX, int startY);
void bfsGR(GR *graph, int startX, int startY);
GR *createResonanceGraph(ED *aerialList);

#endif // GRAPH_H