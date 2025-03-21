// File: aerial.c
// Desc:
// Auth: Carlos Barreiro
// Mail: a20360@alunos.ipca.pt
// Date: 2025/03

#include <stdlib.h>
#include <stdio.h>
#include "aerial.h"

// Função para inserir uma antena no fim da lista
void insertAerialEnd(ED **list, char resonanceFrequencyTmp, int coordinateXTmp, int coordinateYTmp)
{
    ED *aux, *new = malloc(sizeof(ED)); // Alocação de memória para a nova antena
    if (new == NULL)                    // Verificação da alocação de memória
    {
        printf("Erro ao alocar memória para a nova antena\n");
        return;
    }

    new->resonanceFrequency = resonanceFrequencyTmp; // Atribuição da frequência de ressonância
    new->coordinateX = coordinateXTmp;               // Atribuição da coordenada X
    new->coordinateY = coordinateYTmp;               // Atribuição da coordenada Y
    new->next = NULL;                                //

    if (*list == NULL) // Se o conteúdo da lista for nulo, então a nova antena passa a ser a primeira da lista
    {
        *list = new;
        return;
    }
    else // Se a lista não for nula, então a nova antena é inserida no fim da lista
    {
        aux = *list;      // Variável auxiliar para percorrer a lista
        while (aux->next) // Enquanto aux for diferente de nulo, ou seja, enquanto não chegar ao fim da lista
        {
            aux = aux->next; // Avança para o próximo registo da lista
        }
        aux->next = new; // O último registo da lista aponta para a nova antena
    }
}

// Função para mostrar a lista de antenas
void showAerialList(ED *ED)
{
    printf("\n\tLista de antenas:\n");
    while (ED != NULL)
    {
        printf("\n\tFrequência de ressonância: %c\n", ED->resonanceFrequency);
        printf("\tCoordenada X: %d\n", ED->coordinateX);
        printf("\tCoordenada Y: %d\n", ED->coordinateY);
        ED = ED->next;
    }
    printf("\n\n");
}