// File: aerial.h
// Desc:
// Auth: Carlos Barreiro
// Mail: a20360@alunos.ipca.pt
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

// Função para inserir uma antena no fim da lista
void insertAerialEnd(ED **list, char resonanceFrequencyTmp, int coordinateXTmp, int coordinateYTmp);

// Função para saber o tamanho do grid
void getGridSize(ED *list, int *xMax, int *yMax);

// Função para imprimir a lista de antenas
void showAerialList(ED *ED);

// Função para carregar as antenas de um ficheiro
void loadAerialsFromFile(ED **list, const char *filename);

#endif // AERIAL_H