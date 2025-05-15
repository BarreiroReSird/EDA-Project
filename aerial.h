// File: aerial.h
// Desc: Este ficheiro contém a definição da estrutura de dados ED e as funções para inserir e remover antenas de uma lista ligada.
// Auth: Carlos Barreiro
// Mail: a20360@alunos.ipca.pt
// Date: 2025/03

#ifndef AERIAL_H
#define AERIAL_H

#define MAX_LINE_LENGTH 1000 // Constante para o tamanho máximo da linha do ficheiro

// Estrutura de dados ED, representa as antenas
typedef struct ED
{
    char resonanceFrequency; // Frequência de ressonância da antena
    int coordinateX;         // Coordenada X da antena
    int coordinateY;         // Coordenada Y da antena
    struct ED *next;         // Ponteiro para a próxima antena na lista
} ED;

// Função para adicionar uma antena (fim da lista)
ED *insertAerialEnd(ED *list, char resonanceFrequencyTmp, int coordinateXTmp, int coordinateYTmp);

// Função para remover uma antena da lista
ED *removeAerial(ED *list, int coordinateX, int coordinateY);

// Função para ...
ED *findAerial(ED *list, int coordinateX, int coordinateY);

#endif // AERIAL_H