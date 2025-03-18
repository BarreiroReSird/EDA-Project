// File: aerial.c
// Desc: Implementation of the aerial class
// Auth: Carlos Barreiro (a20360)
// Date: 2025/03

#include "aerial.h"
#include <stdlib.h>
#include <stdio.h>

// Função para criar as antenas
ED *createAerial(char resonanceFrequency, int coordinateX, int coordinateY)
{
    ED *newAerial = (ED *)malloc(sizeof(ED));
    if (newAerial == NULL)
    {
        return NULL; // Erro na alocação de memória
    }
    newAerial->resonanceFrequency = resonanceFrequency;
    newAerial->coordinateX = coordinateX;
    newAerial->coordinateY = coordinateY;
    newAerial->next = NULL;
    return newAerial;
}