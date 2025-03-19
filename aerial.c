// File: aerial.c
// Desc:
// Auth: Carlos Barreiro
// Mail: a20360@alunos.ipca.pt
// Date: 2025/03

#include "aerial.h"
#include <stdlib.h>
#include <stdio.h>

// Função para criar as antenas
ED *createAerial(char resonanceFrequency, int coordinateX, int coordinateY)
{
    // Alocação de memória para a nova antena
    ED *newAerial = (ED *)malloc(sizeof(ED));
    if (newAerial == NULL)
    {
        return NULL;
    }

    // Guarda dados
    newAerial->resonanceFrequency = resonanceFrequency;
    newAerial->coordinateX = coordinateX;
    newAerial->coordinateY = coordinateY;
    newAerial->next = NULL;
    return newAerial;
}

// Função para adicionar uma nova antena no final da lista
void addAerial(ED *head, char resonanceFrequency, int coordinateX, int coordinateY)
{
    // Cria o novo nó
    ED *newAerial = createAerial(resonanceFrequency, coordinateX, coordinateY);
    if (newAerial == NULL)
    {
        printf("Erro ao alocar memória para a nova antena.\n");
        return;
    }

    // Se a lista estiver vazia, o novo nó se torna o início da lista
    if (head == NULL)
    {
        head = newAerial;
    }
    else
    {
        // Caso contrário, percorre a lista até o último nó e adiciona
        ED *current = head;
        while (current->next != NULL)
        {
            current = current->next;
        }

        // Faz o último nó apontar para o novo nó
        current->next = newAerial;
    }
}