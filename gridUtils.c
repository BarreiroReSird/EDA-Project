// File: gridUtils.c
// Desc: Este ficheiro contém funções para calcular o tamanho da matriz, mostrar a lista de antenas e verificar se há interferências entre antenas.
// Auth: Carlos Barreiro
// Mail: a20360@alunos.ipca.pt
// Date: 2025/03

#include <stdio.h>
#include <stdlib.h>
#include "gridUtils.h"
#include "aerial.h"

// Função para saber o tamanho do grid
int getGridSize(ED *list, int *xMax, int *yMax)
{
    *xMax = 0;
    *yMax = 0;
    ED *aux = list;

    while (aux != NULL)
    {
        if (aux->coordinateX > *xMax)
        {
            *xMax = aux->coordinateX;
        }
        if (aux->coordinateY > *yMax)
        {
            *yMax = aux->coordinateY;
        }
        aux = aux->next;
    }
    return 0;
}

// Função para mostrar a lista de antenas
int showAerialList(ED *list)
{
    int xMax, yMax;
    getGridSize(list, &xMax, &yMax);

    char grid[yMax][xMax];

    for (int y = 0; y < yMax; y++)
    {
        for (int x = 0; x < xMax; x++)
        {
            grid[y][x] = '.';
        }
    }

    ED *aux = list;
    while (aux != NULL)
    {
        if (aux->coordinateX <= xMax && aux->coordinateY <= yMax && aux->coordinateX >= 1 && aux->coordinateY >= 1)
        {
            grid[aux->coordinateY - 1][aux->coordinateX - 1] = aux->resonanceFrequency;
        }
        aux = aux->next;
    }

    printf("\n\tAntenas:\n");
    for (int y = 0; y < yMax; y++)
    {
        printf("\t");
        for (int x = 0; x < xMax; x++)
        {
            printf("%c ", grid[y][x]);
        }
        printf("\n");
    }
    printf("\n");
    return 0;
}