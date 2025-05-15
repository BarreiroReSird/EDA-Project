// File: aerial.c
// Desc: Este ficheiro contém funções para inserir e remover antenas de uma lista ligada, bem como para calcular a distância entre antenas.
// Auth: Carlos Barreiro
// Mail: a20360@alunos.ipca.pt
// Date: 2025/03

#include <stdlib.h>
#include <stdio.h>
#include "aerial.h"

// Função para inserir uma antena no fim da lista
ED *insertAerialEnd(ED *list, char resonanceFrequencyTmp, int coordinateXTmp, int coordinateYTmp)
{
    ED *aux, *new = malloc(sizeof(ED));
    if (new == NULL)
    {
        printf("Erro ao alocar memória para a nova antena\n");
        return list;
    }

    new->resonanceFrequency = resonanceFrequencyTmp;
    new->coordinateX = coordinateXTmp;
    new->coordinateY = coordinateYTmp;
    new->next = NULL;

    if (list == NULL)
    {
        return new;
    }
    else
    {
        aux = list;
        while (aux->next)
        {
            aux = aux->next;
        }
        aux->next = new;
        return list;
    }
}

// Função para remover uma antena da lista
ED *removeAerial(ED *list, int coordinateX, int coordinateY)
{
    if (list == NULL)
    {
        printf("Lista de antenas vazia. Nada para remover.\n");
        return NULL;
    }

    ED *current = list;
    ED *previous = NULL;

    while (current != NULL)
    {
        if (current->coordinateX == coordinateX && current->coordinateY == coordinateY)
        {
            if (previous == NULL)
            {
                list = current->next;
            }
            else
            {
                previous->next = current->next;
            }

            free(current);
            printf("Antena na posição (%d, %d) removida com sucesso.\n", coordinateX, coordinateY);
            return list;
        }

        previous = current;
        current = current->next;
    }

    printf("Antena na posição (%d, %d) não encontrada.\n", coordinateX, coordinateY);
    return list;
}

// Função para ...
ED *findAerial(ED *list, int coordinateX, int coordinateY)
{
    ED *current = list;
    while (current != NULL)
    {
        if (current->coordinateX == coordinateX && current->coordinateY == coordinateY)
        {
            return current;
        }
        current = current->next;
    }
    return NULL;
}