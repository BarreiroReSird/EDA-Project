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

float CalculateDistance(Vertex *a, Vertex *b)
{
    return sqrtf(powf(a->coordinateX - b->coordinateX, 2) + powf(a->coordinateY - b->coordinateY, 2));
}
