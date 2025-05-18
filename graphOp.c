// File: graphOp.c
// Desc:
// Auth: Carlos Barreiro
// Mail: a20360@alunos.ipca.pt
// Date: 2025/05

#include "graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

// Função que encontra intersecções entre caminhos de vértices com duas frequências distintas
int findIntersections(Graph *graph, char freqA, char freqB)
{
    printf("\nProcura de intersecções entre %c e %c:\n", freqA, freqB);

    Path pathsA[100], pathsB[100]; // Arrays para armazenar caminhos das frequências A e B
    int countA = 0, countB = 0;    // Contadores de caminhos únicos encontrados
    int intersectionCount = 0;     // Contador de intersecções encontradas

    // Recolha de caminhos entre vértices com frequência freqA (sem duplicação)
    Vertex *current = graph->head;
    while (current != NULL)
    {
        if (current->resonanceFrequency == freqA)
        {
            Adjacency *adj = current->adjacencies;
            while (adj != NULL)
            {
                // Achar o vértice de destino com base no índice
                Vertex *dest = graph->head;
                for (int i = 0; i < adj->destinationVertexIndex && dest != NULL; i++)
                    dest = dest->next;

                // Se destino é válido e tem a mesma frequência
                if (dest != NULL && dest->resonanceFrequency == freqA)
                {
                    // Verificar se o caminho já foi adicionado (em qualquer direção)
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

                    // Se não existe, adicionar ao array de caminhos
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

    // Recolha de caminhos entre vértices com frequência freqB (sem duplicação)
    current = graph->head;
    while (current != NULL)
    {
        if (current->resonanceFrequency == freqB)
        {
            Adjacency *adj = current->adjacencies;
            while (adj != NULL)
            {
                // Achar o vértice de destino com base no índice
                Vertex *dest = graph->head;
                for (int i = 0; i < adj->destinationVertexIndex && dest != NULL; i++)
                    dest = dest->next;

                if (dest != NULL && dest->resonanceFrequency == freqB)
                {
                    // Verificar duplicação de caminho
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

                    // Adicionar caminho novo
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

    // Verificação de intersecções entre os caminhos de freqA e freqB
    bool found = false;
    for (int i = 0; i < countA; i++)
    {
        for (int j = 0; j < countB; j++)
        {
            // Apenas verificar caminhos que ainda não foram processados
            if (!pathsA[i].processed && !pathsB[j].processed)
            {
                // Coordenadas do primeiro segmento (freqA)
                float x1 = pathsA[i].start->coordinateX;
                float y1 = pathsA[i].start->coordinateY;
                float x2 = pathsA[i].end->coordinateX;
                float y2 = pathsA[i].end->coordinateY;

                // Coordenadas do segundo segmento (freqB)
                float x3 = pathsB[j].start->coordinateX;
                float y3 = pathsB[j].start->coordinateY;
                float x4 = pathsB[j].end->coordinateX;
                float y4 = pathsB[j].end->coordinateY;

                // Cálculo da intersecção usando determinantes (fórmulas padrão)
                float denom = (y4 - y3) * (x2 - x1) - (x4 - x3) * (y2 - y1);

                // Se as retas não forem paralelas
                if (denom != 0)
                {
                    float ua = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / denom;
                    float ub = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / denom;

                    // Verifica se a intersecção está dentro dos segmentos (0 ≤ u ≤ 1)
                    if (ua >= 0 && ua <= 1 && ub >= 0 && ub <= 1)
                    {
                        printf("Intersecção encontrada:\n");
                        printf("Entre %c(%.0f,%.0f) -> %c(%.0f,%.0f) e ",
                               freqA, x1, y1, freqA, x2, y2);
                        printf("%c(%.0f,%.0f) -> %c(%.0f,%.0f)\n",
                               freqB, x3, y3, freqB, x4, y4);

                        found = true;
                        intersectionCount++;

                        // Marca os caminhos como processados para evitar recontagem
                        pathsA[i].processed = true;
                        pathsB[j].processed = true;
                    }
                }
            }
        }
    }

    // Mensagem caso nenhuma intersecção tenha sido encontrada
    if (!found)
    {
        printf("Nenhuma intersecção encontrada.\n");
    }

    return intersectionCount;
}
