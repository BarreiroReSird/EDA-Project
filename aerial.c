// File: aerial.c
// Desc: Este ficheiro contém funções para inserir e remover antenas de uma lista ligada, bem como para calcular a distância entre antenas.
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
    new->next = NULL;

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

// Função para remover uma antena da lista
void removeAerial(ED **list, int coordinateX, int coordinateY)
{
    if (*list == NULL) // Verifica se a lista está vazia
    {
        printf("Lista de antenas vazia. Nada para remover.\n");
        return;
    }

    ED *current = *list; // Variável auxiliar para percorrer a lista
    ED *previous = NULL; // Variável auxiliar para guardar o registo anterior

    // Percorre a lista à procura da antena
    while (current != NULL)
    {
        if (current->coordinateX == coordinateX && current->coordinateY == coordinateY) // Verifica se a antena foi encontrada
        {
            // Remove a antena da lista
            if (previous == NULL)
            {
                // Se for o primeiro elemento
                *list = current->next;
            }
            else
            {
                // Se for um elemento do meio ou fim
                previous->next = current->next;
            }

            free(current); // Liberta a memória alocada para a antena
            printf("Antena na posição (%d, %d) removida com sucesso.\n", coordinateX, coordinateY);
            return;
        }

        previous = current;      // Guarda o registo anterior
        current = current->next; // Avança para o próximo registo
    }

    // A antena não foi encontrada
    printf("Antena na posição (%d, %d) não encontrada.\n", coordinateX, coordinateY);
}
