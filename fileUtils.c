// File: fileUtils.c
// Desc: Este ficheiro contém funções para carregar e guardar dados de antenas em ficheiros, bem como para mostrar a lista de antenas.
// Auth: Carlos Barreiro
// Mail: a20360@alunos.ipca.pt
// Date: 2025/03

#include <stdio.h>
#include <stdlib.h>
#include "fileUtils.h"
#include "aerial.h"

// Função para carregar as antenas de um ficheiro
void loadAerialsFromFile(ED **list, const char *filename)
{
    int y = 1;                  // Para iniciar a coordenada Y
    char line[MAX_LINE_LENGTH]; // Tamanho máximo da linha do ficheiro

    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        return;
    }

    // Lê o ficheiro linha a linha
    while (fgets(line, MAX_LINE_LENGTH, file) != NULL)
    {
        int x = 1; // Para iniciar a coordenada X
        // Percorre a linha
        for (int i = 0; line[i] != '\0' && line[i] != '\n'; i++) // Parar também no \n
        {
            if (line[i] != ' ' && line[i] != '.') // Ignora espaços e pontos
            {
                insertAerialEnd(list, line[i], x, y);
            }
            // Incrementa X apenas para caracteres relevantes
            if (line[i] != ' ') // Espaços não contam como posições
            {
                x++;
            }
        }
        y++; // Incrementa a coordenada Y
    }

    fclose(file);
    printf("Dados carregados do ficheiro %s com sucesso!\n", filename);
}