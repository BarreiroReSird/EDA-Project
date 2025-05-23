// File: graphIO.c
// Desc:
// Auth: Carlos Barreiro
// Mail: a20360@alunos.ipca.pt
// Date: 2025/05

#include "graph.h"
#include <stdio.h>
#include <stdlib.h> // Para malloc, free
#include <ctype.h>  // Para isalpha

// Função para carregar o grafo a partir de um ficheiro
bool LoadGraph(char *fileName, Graph *graph)
{
    FILE *file = fopen(fileName, "r");
    if (!file)
    {
        printf("Erro a carregar o ficheiro '%s'.\n", fileName);
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

// Função para mostrar o grafo em formato de lista
int ShowGraph(Graph *g)
{
    if (!g || !g->head)
    {
        printf("Grafo vazio.\n");
        return 0;
    }

    int count = 0;
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
        count++;
    }
    return count;
}

// Função para mostrar o grafo em formato de grid
int ShowGraphAsGrid(Graph *graph)
{
    if (!graph || !graph->head)
    {
        printf("Grafo vazio.\n");
        return 0;
    }

    char grid[M][M];
    for (int i = 0; i < M; i++)
        for (int j = 0; j < M; j++)
            grid[i][j] = '.';

    int maxX = 0, maxY = 0;
    int count = 0;

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
            count++;
        }

        v = v->next;
    }

    printf("\n--- Grafo em grid ---\n");

    for (int i = 0; i <= maxY; i++)
    {
        for (int j = 0; j <= maxX; j++)
        {
            printf("%c ", grid[i][j]);
        }
        printf("\n");
    }

    return count;
}