// File: graphSearch.c
// Desc:
// Auth: Carlos Barreiro
// Mail: a20360@alunos.ipca.pt
// Date: 2025/05

#include "graph.h"
#include <stdio.h>
#include <stdlib.h> // Para malloc, free

// DFT
int visitDFT(Vertex *v, bool *visited, Graph *graph, int index)
{
    if (!v)
        return 0;

    int count = 1; // Conta o vértice atual
    visited[index] = true;
    printf("Antena: %c (%.0f, %.0f)\n", v->resonanceFrequency, v->coordinateX, v->coordinateY);

    // Criar uma lista temporária de adjacências
    Adjacency *adjList = NULL;
    Adjacency *adj = v->adjacencies;

    // Copiar as adjacências para uma lista temporária
    while (adj)
    {
        Adjacency *newAdj = CreateAdjacency(adj->distance, adj->resonanceFrequency, adj->destinationVertexIndex);
        newAdj->next = adjList;
        adjList = newAdj;
        adj = adj->next;
    }

    // Visitar cada adjacência
    adj = adjList;
    while (adj)
    {
        if (!visited[adj->destinationVertexIndex])
        {
            Vertex *next = graph->head;
            for (int i = 0; next && i < adj->destinationVertexIndex; i++)
                next = next->next;

            if (next && next->resonanceFrequency == v->resonanceFrequency)
                count += visitDFT(next, visited, graph, adj->destinationVertexIndex);
        }
        adj = adj->next;
    }

    // Libertar a lista temporária
    while (adjList)
    {
        Adjacency *temp = adjList;
        adjList = adjList->next;
        free(temp);
    }

    return count;
}

int DFT_FromCoordinates(float x, float y, Graph *graph)
{
    if (!graph || !graph->head)
    {
        printf("Grafo vazio.\n");
        return 0;
    }

    Vertex *v = graph->head;
    int index = 0;
    while (v && (v->coordinateX != x || v->coordinateY != y))
    {
        v = v->next;
        index++;
    }

    if (!v)
    {
        printf("Antena não encontrada nas coordenadas (%.0f, %.0f)\n", x, y);
        return 0;
    }

    bool *visited = (bool *)calloc(graph->numVertices, sizeof(bool));
    if (!visited)
    {
        printf("Erro de memória.\n");
        return 0;
    }

    printf("DFT a partir da antena %c (%.0f, %.0f):\n", v->resonanceFrequency, x, y);
    int verticesVisited = visitDFT(v, visited, graph, index);
    free(visited);

    printf("Total de vértices visitados: %d\n", verticesVisited);
    return verticesVisited;
}

// BFT
int Enqueue(Queue *q, int index)
{
    if (q == NULL)
        return 0;

    QueueNode *newNode = (QueueNode *)malloc(sizeof(QueueNode));
    if (newNode == NULL)
        return 0;

    newNode->index = index;
    newNode->next = NULL;

    if (q->rear == NULL)
    {
        q->front = q->rear = newNode;
    }
    else
    {
        q->rear->next = newNode;
        q->rear = newNode;
    }

    return 1;
}

int Dequeue(Queue *q)
{
    if (!q->front)
        return -1;
    QueueNode *temp = q->front;
    int index = temp->index;
    q->front = temp->next;
    if (!q->front)
        q->rear = NULL;
    free(temp);
    return index;
}

bool IsQueueEmpty(Queue *q)
{
    return q->front == NULL;
}

int BFT_FromCoordinates(float x, float y, Graph *graph)
{
    if (!graph || !graph->head)
    {
        printf("Grafo vazio.\n");
        return 0;
    }

    Vertex *v = graph->head;
    int index = 0;
    while (v && (v->coordinateX != x || v->coordinateY != y))
    {
        v = v->next;
        index++;
    }

    if (!v)
    {
        printf("Antena não encontrada nas coordenadas (%.0f, %.0f)\n", x, y);
        return 0;
    }

    char targetResonance = v->resonanceFrequency;
    bool *visited = (bool *)calloc(graph->numVertices, sizeof(bool));
    if (!visited)
    {
        printf("Erro de memória.\n");
        return 0;
    }

    Queue q = {NULL, NULL};
    Enqueue(&q, index);
    visited[index] = true;

    printf("BFT a partir da antena %c (%.0f, %.0f):\n", v->resonanceFrequency, x, y);
    int verticesVisited = 0;

    while (!IsQueueEmpty(&q))
    {
        int currentIndex = Dequeue(&q);
        Vertex *curr = graph->head;
        for (int i = 0; curr && i < currentIndex; i++)
            curr = curr->next;

        if (curr)
        {
            verticesVisited++;
            printf("Antena: %c (%.0f, %.0f)\n", curr->resonanceFrequency, curr->coordinateX, curr->coordinateY);

            Adjacency *adj = curr->adjacencies;
            while (adj)
            {
                if (!visited[adj->destinationVertexIndex])
                {
                    Vertex *neighbor = graph->head;
                    for (int i = 0; neighbor && i < adj->destinationVertexIndex; i++)
                        neighbor = neighbor->next;

                    if (neighbor && neighbor->resonanceFrequency == targetResonance)
                    {
                        Enqueue(&q, adj->destinationVertexIndex);
                        visited[adj->destinationVertexIndex] = true;
                    }
                }
                adj = adj->next;
            }
        }
    }

    free(visited);
    printf("Total de vértices visitados: %d\n", verticesVisited);
    return verticesVisited;
}

// Caminhos
int FindAllPathsUtil(Vertex *current, int currentIndex, int endIndex, bool *visited, int *path, int pathIndex, Graph *graph)
{
    int pathCount = 0;
    visited[currentIndex] = true;
    path[pathIndex] = currentIndex;
    pathIndex++;

    if (currentIndex == endIndex)
    {
        pathCount++;
        for (int i = 0; i < pathIndex; i++)
        {
            Vertex *v = graph->head;
            for (int j = 0; j < path[i]; j++)
                v = v->next;
            printf("%c(%.0f,%.0f)", v->resonanceFrequency, v->coordinateX, v->coordinateY);
            if (i < pathIndex - 1)
                printf(" -> ");
        }
        printf("\n");
    }
    else
    {
        Adjacency *adj = current->adjacencies;
        while (adj)
        {
            if (!visited[adj->destinationVertexIndex])
            {
                Vertex *next = graph->head;
                for (int i = 0; i < adj->destinationVertexIndex; i++)
                    next = next->next;

                if (next && next->resonanceFrequency == current->resonanceFrequency)
                {
                    pathCount += FindAllPathsUtil(next, adj->destinationVertexIndex, endIndex, visited, path, pathIndex, graph);
                }
            }
            adj = adj->next;
        }
    }

    // Backtrack
    visited[currentIndex] = false;
    pathIndex--;
    return pathCount;
}

int FindAllPaths(Graph *graph, float startX, float startY, float endX, float endY)
{
    if (!graph || !graph->head)
    {
        printf("Grafo vazio.\n");
        return 0;
    }

    Vertex *start = graph->head;
    Vertex *end = graph->head;
    int startIndex = 0, endIndex = 0;
    bool startFound = false, endFound = false;

    // Procura pelos vértices de início e fim
    Vertex *current = graph->head;
    int index = 0;
    while (current)
    {
        if (!startFound && current->coordinateX == startX && current->coordinateY == startY)
        {
            start = current;
            startIndex = index;
            startFound = true;
        }

        if (!endFound && current->coordinateX == endX && current->coordinateY == endY)
        {
            end = current;
            endIndex = index;
            endFound = true;
        }

        current = current->next;
        index++;
    }

    if (!startFound || !endFound)
    {
        printf("Antenas inicial ou final não encontradas.\n");
        return 0;
    }

    if (start->resonanceFrequency != end->resonanceFrequency)
    {
        printf("As antenas não tem a mesma frequência.\n");
        return 0;
    }

    bool *visited = (bool *)calloc(graph->numVertices, sizeof(bool));
    int *path = (int *)malloc(graph->numVertices * sizeof(int));

    printf("Todos os caminhos de %c(%.0f,%.0f) a %c(%.0f,%.0f):\n",
           start->resonanceFrequency, startX, startY,
           end->resonanceFrequency, endX, endY);

    int totalPaths = FindAllPathsUtil(start, startIndex, endIndex, visited, path, 0, graph);
    printf("Total de caminhos encontrados: %d\n", totalPaths);

    free(visited);
    free(path);
    return totalPaths;
}