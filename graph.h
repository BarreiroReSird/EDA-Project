// File: graph.h
// Desc: 
// Auth: Carlos Barreiro
// Mail: a20360@alunos.ipca.pt
// Date: 2025/05

#ifndef GRAPH_H
#define GRAPH_H

#include <stdbool.h>

#define M 40

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
    int resonanceFrequency;
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

// Graph Initialization & Management
Vertex *InitializeVertexList();
Vertex *CreateVertex(char resonanceFrequency, float coordinateX, float coordinateY);
Vertex *InsertVertex(Vertex *newVertex, Vertex *head, int *res);
bool AdjacencyExists(Vertex *origin, int destinationIndex);
Adjacency *CreateAdjacency(float distance, int resonanceFrequency, int destinationIndex);
Vertex *InsertAdjacency(Vertex *head, Adjacency *newAdj, int originIndex, int destinationIndex, int *res);
bool LoadGraph(char *fileName, Graph *graph);
void ShowGraph(Graph *graph);
void ShowGraphAsGrid(Graph *graph);

// Search Algorithms
void DFT_FromCoordinates(float x, float y, Graph *graph);
void BFT_FromCoordinates(float x, float y, Graph *graph);
void FindAllPaths(Graph *graph, float startX, float startY, float endX, float endY);

// Utility Functions
float CalculateDistance(Vertex *a, Vertex *b);

// Queue for BFT
void Enqueue(Queue *q, int index);
int Dequeue(Queue *q);
bool IsQueueEmpty(Queue *q);

#endif // GRAPH_H
