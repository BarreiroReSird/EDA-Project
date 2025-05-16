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

static Adjacency *newAdjacency(ED *dst, float weight)
{
    Adjacency *new = (Adjacency *)malloc(sizeof(Adjacency));
    if (!new)
        return NULL;

    new->dst = dst;
    new->weight = weight;
    new->next = NULL;
    return new;
}

GR *createGR(ED *aerialList)
{
    if (!aerialList)
        return NULL;

    int count = 0;
    for (ED *temp = aerialList; temp != NULL; temp = temp->next)
        count++;

    GR *graph = (GR *)malloc(sizeof(GR));
    if (!graph)
        return NULL;

    graph->vertexNum = count;
    graph->vertexList = (Vertex *)calloc(count, sizeof(Vertex));
    if (!graph->vertexList)
    {
        free(graph);
        return NULL;
    }

    ED *current = aerialList;
    for (int i = 0; i < count && current != NULL; i++)
    {
        graph->vertexList[i].aerial = current;
        graph->vertexList[i].adjList = NULL;
        current = current->next;
    }

    return graph;
}

bool addEdge(GR *graph, ED *src, ED *dst, float weight)
{
    if (!graph || !src || !dst)
        return false;

    int srcIndex = -1, dstIndex = -1;
    for (int i = 0; i < graph->vertexNum; i++)
    {
        if (graph->vertexList[i].aerial == src)
            srcIndex = i;
        if (graph->vertexList[i].aerial == dst)
            dstIndex = i;
    }

    if (srcIndex == -1 || dstIndex == -1)
        return false;

    Adjacency *newAdj = newAdjacency(dst, weight);
    newAdj->next = graph->vertexList[srcIndex].adjList;
    graph->vertexList[srcIndex].adjList = newAdj;

    newAdj = newAdjacency(src, weight);
    newAdj->next = graph->vertexList[dstIndex].adjList;
    graph->vertexList[dstIndex].adjList = newAdj;

    return true;
}

bool buildResonanceGR(GR *graph, ED *aerialList)
{
    if (!graph || !aerialList)
        return false;

    // Agrupa antenas por frequência
    typedef struct FrequencyGroup
    {
        char freq;
        ED *first;
        ED *last;
        struct FrequencyGroup *next;
    } FrequencyGroup;

    FrequencyGroup *groups = NULL;

    // Cria grupos de frequências
    ED *current = aerialList;
    while (current)
    {
        FrequencyGroup *group = groups;
        while (group)
        {
            if (group->freq == current->resonanceFrequency)
            {
                break;
            }
            group = group->next;
        }

        if (!group)
        {
            // Novo grupo
            group = (FrequencyGroup *)malloc(sizeof(FrequencyGroup));
            if (!group)
                return false;
            group->freq = current->resonanceFrequency;
            group->first = current;
            group->last = current;
            group->next = groups;
            groups = group;
        }
        else
        {
            // Adiciona ao grupo existente
            group->last->next = current; // Temporariamente usa 'next' para ligar
            group->last = current;
        }
        current = current->next;
    }

    // Constrói o grafo (conecta cada grupo em uma árvore)
    bool hasConnections = false;
    FrequencyGroup *group = groups;
    while (group)
    {
        if (group->first != group->last)
        { // Se houver mais de uma antena no grupo
            ED *prev = group->first;
            ED *curr = prev->next; // Usamos o campo 'next' temporário

            while (curr)
            {
                if (addEdge(graph, prev, curr, 1.0f))
                {
                    hasConnections = true;
                }
                prev = curr;
                curr = curr->next;
            }
        }

        // Limpa o 'next' temporário
        ED *aerial = group->first;
        while (aerial && aerial != group->last)
        {
            ED *next = aerial->next;
            aerial->next = next; // Restaura o próximo nó original (se necessário)
            aerial = next;
        }

        FrequencyGroup *temp = group;
        group = group->next;
        free(temp);
    }

    return hasConnections;
}

int printGR(const GR *graph)
{
    if (!graph)
        return 0;

    printf("\nGrafo de Ressonância:\n");
    int count = 0;
    for (int i = 0; i < graph->vertexNum; i++)
    {
        ED *aerial = graph->vertexList[i].aerial;
        printf("Antena '%c' (%d, %d) -> ",
               aerial->resonanceFrequency,
               aerial->coordinateX,
               aerial->coordinateY);

        for (Adjacency *adj = graph->vertexList[i].adjList; adj; adj = adj->next)
        {
            printf("[%c (%.1f)] ", adj->dst->resonanceFrequency, adj->weight);
            count++;
        }
        printf("\n");
    }
    return count;
}

bool freeGR(GR *graph)
{
    if (!graph)
        return false;

    for (int i = 0; i < graph->vertexNum; i++)
    {
        Adjacency *adj = graph->vertexList[i].adjList;
        while (adj)
        {
            Adjacency *temp = adj;
            adj = adj->next;
            free(temp);
        }
    }

    free(graph->vertexList);
    free(graph);
    return true;
}

// Função auxiliar para encontrar índice de uma antena pelas coordenadas
static int findVertexIndex(GR *graph, int x, int y)
{
    for (int i = 0; i < graph->vertexNum; i++)
    {
        if (graph->vertexList[i].aerial->coordinateX == x &&
            graph->vertexList[i].aerial->coordinateY == y)
            return i;
    }
    return -1;
}

static void dfsVisit(GR *graph, int index, bool *visited)
{
    visited[index] = true;
    ED *a = graph->vertexList[index].aerial;
    printf("(%d, %d)\n", a->coordinateX, a->coordinateY);

    for (Adjacency *adj = graph->vertexList[index].adjList; adj; adj = adj->next)
    {
        int neighborIndex = findVertexIndex(graph, adj->dst->coordinateX, adj->dst->coordinateY);
        if (neighborIndex != -1 && !visited[neighborIndex])
        {
            dfsVisit(graph, neighborIndex, visited);
        }
    }
}

void dfsGR(GR *graph, int startX, int startY)
{
    if (!graph)
        return;

    int startIndex = findVertexIndex(graph, startX, startY);
    if (startIndex == -1)
    {
        printf("Antena inicial não encontrada.\n");
        return;
    }

    bool *visited = (bool *)calloc(graph->vertexNum, sizeof(bool));
    printf("DFS a partir de (%d, %d):\n", startX, startY);
    dfsVisit(graph, startIndex, visited);
    free(visited);
}

#include <string.h>

void bfsGR(GR *graph, int startX, int startY)
{
    if (!graph)
        return;

    int startIndex = findVertexIndex(graph, startX, startY);
    if (startIndex == -1)
    {
        printf("Antena inicial não encontrada.\n");
        return;
    }

    bool *visited = (bool *)calloc(graph->vertexNum, sizeof(bool));
    int *queue = (int *)malloc(sizeof(int) * graph->vertexNum);
    int front = 0, rear = 0;

    visited[startIndex] = true;
    queue[rear++] = startIndex;

    printf("BFS a partir de (%d, %d):\n", startX, startY);

    while (front < rear)
    {
        int currentIndex = queue[front++];
        ED *a = graph->vertexList[currentIndex].aerial;
        printf("(%d, %d)\n", a->coordinateX, a->coordinateY);

        for (Adjacency *adj = graph->vertexList[currentIndex].adjList; adj; adj = adj->next)
        {
            int neighborIndex = findVertexIndex(graph, adj->dst->coordinateX, adj->dst->coordinateY);
            if (neighborIndex != -1 && !visited[neighborIndex])
            {
                visited[neighborIndex] = true;
                queue[rear++] = neighborIndex;
            }
        }
    }

    free(queue);
    free(visited);
}

// Função para criar o grafo de ressonância (sem imprimir)
GR *createResonanceGraph(ED *aerialList)
{
    if (!aerialList)
        return NULL;

    GR *graph = createGR(aerialList);
    if (!graph)
        return NULL;

    if (!buildResonanceGR(graph, aerialList))
    {
        freeGR(graph);
        return NULL;
    }

    return graph;
}
