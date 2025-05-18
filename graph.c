// File: graph.c
// Desc:
// Auth: Carlos Barreiro
// Mail: a20360@alunos.ipca.pt
// Date: 2025/05

#include "graph.h"
#include <stdlib.h> // Para malloc, free
#include <math.h>   // Para sqrtf, powf

// Função para criar um vértice
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

// Função para verificar se existe uma adjacência entre dois vértices
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

// Função para criar uma nova adjacência
Adjacency *CreateAdjacency(float distance, char resonanceFrequency, int destinationIndex)
{
    Adjacency *adj = (Adjacency *)malloc(sizeof(Adjacency));
    if (adj)
    {
        adj->distance = distance;
        adj->resonanceFrequency = resonanceFrequency; // Agora é char
        adj->destinationVertexIndex = destinationIndex;
        adj->next = NULL;
    }
    return adj;
}

// Função para inserir uma nova adjacência na lista de adjacências de um vértice
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

// Função para inserir um novo vértice na lista de vértices
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

// Função para calcular a distância entre dois vértices
// sqrtf é usado para calcular a raiz quadrada
// powf é usado para elevar ao quadrado
float CalculateDistance(Vertex *a, Vertex *b)
{
    return sqrtf(powf(a->coordinateX - b->coordinateX, 2) + powf(a->coordinateY - b->coordinateY, 2));
}

// Liberta a memória alocada para as adjacências
void FreeAdjacencies(Adjacency *adj)
{
    while (adj != NULL)
    {
        Adjacency *temp = adj;
        adj = adj->next;
        free(temp);
    }
}

// Liberta a memória alocada para o grafo e todos os vértices
void FreeGraph(Graph *graph)
{
    if (graph == NULL)
        return;

    Vertex *current = graph->head;
    while (current != NULL)
    {
        Vertex *temp = current;
        current = current->next;
        FreeAdjacencies(temp->adjacencies); // Liberta adjacências do vértice
        free(temp);
    }
    free(graph); // Liberta a estrutura do grafo
}
