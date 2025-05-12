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
ED *loadAerialsFromFile(ED *list, const char *filename)
{
    int y = 1;
    char line[MAX_LINE_LENGTH];

    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        return list;
    }

    while (fgets(line, MAX_LINE_LENGTH, file) != NULL)
    {
        int x = 1;
        for (int i = 0; line[i] != '\0' && line[i] != '\n'; i++)
        {
            if (line[i] != ' ' && line[i] != '.')
            {
                list = insertAerialEnd(list, line[i], x, y);
            }
            if (line[i] != ' ')
            {
                x++;
            }
        }
        y++;
    }

    fclose(file);
    printf("Dados carregados do ficheiro %s com sucesso!\n", filename);
    return list;
}