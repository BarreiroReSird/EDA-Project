// File: graph.c
// Desc: 
// Auth: Carlos Barreiro
// Mail: a20360@alunos.ipca.pt
// Date: 2025/05

#include "graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

Vertex *CreateVertex(char resonanceFrequency, float coordinateX, float coordinateY)
{
    Vertex *v = (Vertex *)malloc(sizeof(Vertex));
    if (v)
    {
        v->resonanceFrequency = resonanceFrequency;
        v->coordinateX = coordinateX;
        v->coordinateY = coordinateY;
        v->adjacencies = NULL;
        v->next = NULL;
    }
    return v;
}

bool AdjacencyExists(Vertex *origin, int destinationIndex)
{
    Adjacency *adj = origin->adjacencies;
    while (adj)
    {
        if (adj->destinationVertexIndex == destinationIndex)
            return true;
        adj = adj->next;
    }
    return false;
}

Adjacency *CreateAdjacency(float distance, int resonanceFrequency, int destinationIndex)
{
    Adjacency *adj = (Adjacency *)malloc(sizeof(Adjacency));
    if (adj)
    {
        adj->distance = distance;
        adj->resonanceFrequency = resonanceFrequency;
        adj->destinationVertexIndex = destinationIndex;
        adj->next = NULL;
    }
    return adj;
}

Vertex *InsertAdjacency(Vertex *head, Adjacency *newAdj, int originIndex, int destinationIndex, int *res)
{
    if (!head || !newAdj)
    {
        *res = 0;
        return head;
    }

    Vertex *origin = head;
    for (int i = 0; origin && i < originIndex; i++)
        origin = origin->next;
    Vertex *dest = head;
    for (int i = 0; dest && i < destinationIndex; i++)
        dest = dest->next;

    if (!origin || !dest)
    {
        *res = 0;
        return head;
    }

    newAdj->next = origin->adjacencies;
    origin->adjacencies = newAdj;
    *res = 1;
    return head;
}

Vertex *InsertVertex(Vertex *newVertex, Vertex *head, int *res)
{
    if (!newVertex)
    {
        *res = 0;
        return head;
    }

    if (!head)
    {
        *res = 1;
        return newVertex;
    }

    Vertex *last = head;
    int newIndex = 0;
    while (last->next)
    {
        last = last->next;
        newIndex++;
    }
    last->next = newVertex;
    newIndex++;

    Vertex *aux = head;
    int existingIndex = 0;
    while (aux)
    {
        if (aux->resonanceFrequency == newVertex->resonanceFrequency)
        {
            float dist = CalculateDistance(aux, newVertex);
            if (!AdjacencyExists(aux, newIndex))
            {
                Adjacency *a1 = CreateAdjacency(dist, newVertex->resonanceFrequency, newIndex);
                head = InsertAdjacency(head, a1, existingIndex, newIndex, res);
            }
            if (!AdjacencyExists(newVertex, existingIndex))
            {
                Adjacency *a2 = CreateAdjacency(dist, aux->resonanceFrequency, existingIndex);
                head = InsertAdjacency(head, a2, newIndex, existingIndex, res);
            }
        }
        aux = aux->next;
        existingIndex++;
    }

    *res = 1;
    return head;
}

bool LoadGraph(char *fileName, Graph *graph)
{
    FILE *file = fopen(fileName, "r");
    if (!file)
    {
        printf("Error opening file '%s'.\n", fileName);
        return false;
    }

    char line[256];
    int y = 1;
    while (fgets(line, sizeof(line), file))
    {
        int x = 1;
        for (int i = 0; line[i]; i++)
        {
            if (line[i] == ' ' || line[i] == '\n')
                continue;

            if (isalpha(line[i]))
            {
                Vertex *v = CreateVertex(line[i], (float)x, (float)y);
                int res;
                graph->head = InsertVertex(v, graph->head, &res);
                if (res)
                    graph->numVertices++;
            }
            x++;
        }

        y++;
    }

    fclose(file);
    return true;
}

void ShowGraph(Graph *g)
{
    if (!g || !g->head)
    {
        printf("Empty graph.\n");
        return;
    }

    Vertex *v = g->head;
    int originIndex = 0;
    while (v)
    {
        printf("%c(%.0f,%.0f)", v->resonanceFrequency, v->coordinateX, v->coordinateY);
        Adjacency *adj = v->adjacencies;
        while (adj)
        {
            if (adj->destinationVertexIndex != originIndex)
            {
                Vertex *dest = g->head;
                for (int i = 0; dest && i < adj->destinationVertexIndex; i++)
                    dest = dest->next;
                if (dest && dest->resonanceFrequency == v->resonanceFrequency)
                    printf(" -> %c(%.0f,%.0f)", dest->resonanceFrequency, dest->coordinateX, dest->coordinateY);
            }
            adj = adj->next;
        }
        printf("\n");
        v = v->next;
        originIndex++;
    }
}

float CalculateDistance(Vertex *a, Vertex *b)
{
    return sqrtf(powf(a->coordinateX - b->coordinateX, 2) + powf(a->coordinateY - b->coordinateY, 2));
}

// --------------------- DFT ---------------------

void visitDFT(Vertex *v, bool *visited, Graph *graph, int index)
{
    if (!v)
        return;

    visited[index] = true;
    printf("Antenna reached: %c (%.0f, %.0f)\n", v->resonanceFrequency, v->coordinateX, v->coordinateY);

    // Criar uma lista temporária de adjacências para poder ordenar/controlar a ordem de visita
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

    // Visitar cada adjacência na ordem em que foram encontradas (primeiro -> último)
    adj = adjList;
    while (adj)
    {
        if (!visited[adj->destinationVertexIndex])
        {
            Vertex *next = graph->head;
            for (int i = 0; next && i < adj->destinationVertexIndex; i++)
                next = next->next;

            if (next && next->resonanceFrequency == v->resonanceFrequency)
                visitDFT(next, visited, graph, adj->destinationVertexIndex);
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
}

void DFT_FromCoordinates(float x, float y, Graph *graph)
{
    if (!graph || !graph->head)
    {
        printf("Empty graph.\n");
        return;
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
        printf("Antenna not found at coordinates (%.0f, %.0f)\n", x, y);
        return;
    }

    bool *visited = (bool *)calloc(graph->numVertices, sizeof(bool));
    if (!visited)
    {
        printf("Memory error.\n");
        return;
    }

    printf("DFT starting from antenna %c (%.0f, %.0f):\n", v->resonanceFrequency, x, y);
    visitDFT(v, visited, graph, index);
    free(visited);
}

// --------------------- BFT ---------------------

void Enqueue(Queue *q, int index)
{
    QueueNode *node = (QueueNode *)malloc(sizeof(QueueNode));
    node->index = index;
    node->next = NULL;
    if (q->rear)
        q->rear->next = node;
    else
        q->front = node;
    q->rear = node;
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

void BFT_FromCoordinates(float x, float y, Graph *graph)
{
    if (!graph || !graph->head)
    {
        printf("Empty graph.\n");
        return;
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
        printf("Antenna not found at coordinates (%.0f, %.0f)\n", x, y);
        return;
    }

    char targetResonance = v->resonanceFrequency;
    bool *visited = (bool *)calloc(graph->numVertices, sizeof(bool));
    if (!visited)
    {
        printf("Memory error.\n");
        return;
    }

    Queue q = {NULL, NULL};
    Enqueue(&q, index);
    visited[index] = true;

    printf("BFT starting from antenna %c (%.0f, %.0f):\n", v->resonanceFrequency, x, y);

    while (!IsQueueEmpty(&q))
    {
        int currentIndex = Dequeue(&q);
        Vertex *curr = graph->head;
        for (int i = 0; curr && i < currentIndex; i++)
            curr = curr->next;

        if (curr)
        {
            printf("Antenna reached: %c (%.0f, %.0f)\n", curr->resonanceFrequency, curr->coordinateX, curr->coordinateY);

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
}

void FindAllPathsUtil(Vertex *current, int currentIndex, int endIndex, bool *visited, int *path, int pathIndex, Graph *graph)
{
    visited[currentIndex] = true;
    path[pathIndex] = currentIndex;
    pathIndex++;

    if (currentIndex == endIndex)
    {
        // Caminho encontrado - imprimir
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
                    FindAllPathsUtil(next, adj->destinationVertexIndex, endIndex, visited, path, pathIndex, graph);
                }
            }
            adj = adj->next;
        }
    }

    // Backtrack
    visited[currentIndex] = false;
    pathIndex--;
}

void FindAllPaths(Graph *graph, float startX, float startY, float endX, float endY)
{
    if (!graph || !graph->head)
    {
        printf("Grafo vazio.\n");
        return;
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
        return;
    }

    if (start->resonanceFrequency != end->resonanceFrequency)
    {
        printf("As antenas não tem a mesma frequência.\n");
        return;
    }

    bool *visited = (bool *)calloc(graph->numVertices, sizeof(bool));
    int *path = (int *)malloc(graph->numVertices * sizeof(int));

    printf("Todos os caminhos de %c(%.0f,%.0f) a %c(%.0f,%.0f):\n",
           start->resonanceFrequency, startX, startY,
           end->resonanceFrequency, endX, endY);

    FindAllPathsUtil(start, startIndex, endIndex, visited, path, 0, graph);

    free(visited);
    free(path);
}

void ShowGraphAsGrid(Graph *graph)
{
    if (!graph || !graph->head)
    {
        printf("Grafo vazio.\n");
        return;
    }

    char grid[M][M];
    for (int i = 0; i < M; i++)
        for (int j = 0; j < M; j++)
            grid[i][j] = '.';

    int maxX = 0, maxY = 0;

    Vertex *v = graph->head;
    while (v)
    {
        int x = (int)(v->coordinateX) - 1;
        int y = (int)(v->coordinateY) - 1;

        if (x >= 0 && x < M && y >= 0 && y < M)
        {
            grid[y][x] = v->resonanceFrequency;
            if (x > maxX)
                maxX = x;
            if (y > maxY)
                maxY = y;
        }

        v = v->next;
    }

    printf("\n--- Grafo em grid ---\n");

    // Imprimir de cima para baixo (Y crescente)
    for (int i = 0; i <= maxY; i++)
    {
        for (int j = 0; j <= maxX; j++)
        {
            printf("%c ", grid[i][j]);
        }
        printf("\n");
    }
}
