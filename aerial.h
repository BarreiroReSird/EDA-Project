// File: aerial.h
// Desc:
// Auth: Carlos Barreiro (a20360)
// Date: 2025/03

#ifndef AERIAL_H

#define AERIAL_H

// Estrutura de dados ED, representa as antenas
typedef struct ED
{
    char resonanceFrequency; // Frequência de ressonância da antena
    int coordinateX;         // Coordenada X da antena
    int coordinateY;         // Coordenada Y da antena
    struct ED *next;         // Ponteiro para a próxima antena na lista
} ED;

// Declarações das funções:

// Cria uma nova antena
ED *createAerial(char resonanceFrequency, int coordinateX, int coordinateY);

#endif // AERIAL_H