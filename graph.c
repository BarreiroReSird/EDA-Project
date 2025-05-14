// File: graph.c
// Desc:
// Auth: Carlos Barreiro
// Mail: a20360@alunos.ipca.pt
// Date: 2025/05

#include "graph.h"
#include <stdio.h>
#include <stdlib.h>

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

    bool hasConnections = false;
    ED *outer = aerialList;
    while (outer)
    {
        ED *inner = outer->next;
        while (inner)
        {
            if (outer->resonanceFrequency == inner->resonanceFrequency)
            {
                if (addEdge(graph, outer, inner, 1.0f))
                {
                    hasConnections = true;
                }
            }
            inner = inner->next;
        }
        outer = outer->next;
    }
    return hasConnections;
}

int printGR(const GR *graph)
{
    if (!graph)
        return 0;

    printf("\nGrafo de Ressonância (Antenas conectadas pela mesma frequência):\n");
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