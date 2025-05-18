// File: graph.h
// Desc:
// Auth: Carlos Barreiro
// Mail: a20360@alunos.ipca.pt
// Date: 2025/05

#ifndef GRAPH_H
#define GRAPH_H

#include <stdbool.h>

#define M 40 // Tamanho máximo do grid (40x40)

typedef struct Vertex
{
    char resonanceFrequency;
    float coordinateX, coordinateY;
    struct Vertex *next;
    struct Adjacency *adjacencies;
} Vertex;

typedef struct Adjacency
{
    float distance;
    char resonanceFrequency;
    int destinationVertexIndex;
    struct Adjacency *next;
} Adjacency;

typedef struct Graph
{
    int numVertices;
    Vertex *head;
} Graph;

typedef struct QueueNode
{
    int index;
    struct QueueNode *next;
} QueueNode;

typedef struct Queue
{
    QueueNode *front, *rear;
} Queue;

// Lista de caminhos para cada frequência
typedef struct
{
    Vertex *start;
    Vertex *end;
    bool processed; // Para evitar duplicados
} Path;

// Inicialização e gestão de gráficos
Vertex *CreateVertex(char resonanceFrequency, float coordinateX, float coordinateY);
Vertex *InsertVertex(Vertex *newVertex, Vertex *head, int *res);
bool AdjacencyExists(Vertex *origin, int destinationIndex);
Adjacency *CreateAdjacency(float distance, char resonanceFrequency, int destinationIndex);
Vertex *InsertAdjacency(Vertex *head, Adjacency *newAdj, int originIndex, int destinationIndex, int *res);
bool LoadGraph(char *fileName, Graph *graph);
int ShowGraph(Graph *graph);
int ShowGraphAsGrid(Graph *graph);
int findIntersections(Graph *graph, char freqA, char freqB);

// Algoritmos de procura
int visitDFT(Vertex *v, bool *visited, Graph *graph, int index);
int DFT_FromCoordinates(float x, float y, Graph *graph);
int BFT_FromCoordinates(float x, float y, Graph *graph);
int FindAllPathsUtil(Vertex *current, int currentIndex, int endIndex, bool *visited, int *path, int pathIndex, Graph *graph);
int FindAllPaths(Graph *graph, float startX, float startY, float endX, float endY);

// Funcões auxiliares
float CalculateDistance(Vertex *a, Vertex *b);

// BFT
int Enqueue(Queue *q, int index);
int Dequeue(Queue *q);
bool IsQueueEmpty(Queue *q);

#endif // GRAPH_H
