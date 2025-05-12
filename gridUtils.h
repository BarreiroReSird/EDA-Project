// File: gridUtils.h
// Desc: Declarações para funções relacionadas ao grid
// Auth: Carlos Barreiro
// Mail: a20360@alunos.ipca.pt
// Date: 2025/03

#ifndef GRID_UTILS_H
#define GRID_UTILS_H

#include "aerial.h" // Necessário para a definição de ED

// Função para saber o tamanho do grid (/tabela)
int getGridSize(ED *list, int *xMax, int *yMax);

// Função para imprimir a lista de antenas
int showAerialList(ED *ED);

#endif // GRID_UTILS_H