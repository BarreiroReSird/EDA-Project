// File: fileUtils.h
// Desc: Declarações para funções de manipulação de ficheiros
// Auth: Carlos Barreiro
// Mail: a20360@alunos.ipca.pt
// Date: 2025/03

#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include "aerial.h" // Necessário para a definição de ED

// Função para carregar as antenas de um ficheiro
ED *loadAerialsFromFile(ED *list, const char *filename);

#endif // FILE_UTILS_H