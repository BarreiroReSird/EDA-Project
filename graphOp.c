// File: graphOp.c
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

void findIntersections(Graph *graph, char freqA, char freqB)
{
    printf("\nProcurando intersecções entre %c e %c:\n", freqA, freqB);

    Path pathsA[100], pathsB[100];
    int countA = 0, countB = 0;

    // Coletar caminhos de A (sem duplicados)
    Vertex *current = graph->head;
    while (current != NULL)
    {
        if (current->resonanceFrequency == freqA)
        {
            Adjacency *adj = current->adjacencies;
            while (adj != NULL)
            {
                Vertex *dest = graph->head;
                for (int i = 0; i < adj->destinationVertexIndex && dest != NULL; i++)
                {
                    dest = dest->next;
                }

                if (dest != NULL && dest->resonanceFrequency == freqA)
                {
                    // Verificar se já temos este caminho (em qualquer direção)
                    bool exists = false;
                    for (int i = 0; i < countA; i++)
                    {
                        if ((pathsA[i].start == current && pathsA[i].end == dest) ||
                            (pathsA[i].start == dest && pathsA[i].end == current))
                        {
                            exists = true;
                            break;
                        }
                    }

                    if (!exists)
                    {
                        pathsA[countA].start = current;
                        pathsA[countA].end = dest;
                        pathsA[countA].processed = false;
                        countA++;
                    }
                }
                adj = adj->next;
            }
        }
        current = current->next;
    }

    // Coletar caminhos de B (sem duplicados)
    current = graph->head;
    while (current != NULL)
    {
        if (current->resonanceFrequency == freqB)
        {
            Adjacency *adj = current->adjacencies;
            while (adj != NULL)
            {
                Vertex *dest = graph->head;
                for (int i = 0; i < adj->destinationVertexIndex && dest != NULL; i++)
                {
                    dest = dest->next;
                }

                if (dest != NULL && dest->resonanceFrequency == freqB)
                {
                    // Verificar se já temos este caminho (em qualquer direção)
                    bool exists = false;
                    for (int i = 0; i < countB; i++)
                    {
                        if ((pathsB[i].start == current && pathsB[i].end == dest) ||
                            (pathsB[i].start == dest && pathsB[i].end == current))
                        {
                            exists = true;
                            break;
                        }
                    }

                    if (!exists)
                    {
                        pathsB[countB].start = current;
                        pathsB[countB].end = dest;
                        pathsB[countB].processed = false;
                        countB++;
                    }
                }
                adj = adj->next;
            }
        }
        current = current->next;
    }

    // Verificar intersecções
    bool found = false;
    for (int i = 0; i < countA; i++)
    {
        for (int j = 0; j < countB; j++)
        {
            if (!pathsA[i].processed && !pathsB[j].processed)
            {
                float x1 = pathsA[i].start->coordinateX;
                float y1 = pathsA[i].start->coordinateY;
                float x2 = pathsA[i].end->coordinateX;
                float y2 = pathsA[i].end->coordinateY;

                float x3 = pathsB[j].start->coordinateX;
                float y3 = pathsB[j].start->coordinateY;
                float x4 = pathsB[j].end->coordinateX;
                float y4 = pathsB[j].end->coordinateY;

                // Cálculo de intersecção
                float denom = (y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1);
                if (denom != 0)
                {
                    float ua = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / denom;
                    float ub = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / denom;

                    if (ua >= 0 && ua <= 1 && ub >= 0 && ub <= 1)
                    {
                        printf("Intersecção encontrada:\n");
                        printf("Entre %c(%.0f,%.0f) -> %c(%.0f,%.0f) e ",
                               freqA, x1, y1, freqA, x2, y2);
                        printf("%c(%.0f,%.0f) -> %c(%.0f,%.0f)\n",
                               freqB, x3, y3, freqB, x4, y4);
                        found = true;

                        // Marcar como processado para evitar duplicados
                        pathsA[i].processed = true;
                        pathsB[j].processed = true;
                    }
                }
            }
        }
    }

    if (!found)
    {
        printf("Nenhuma intersecção encontrada.\n");
    }
}