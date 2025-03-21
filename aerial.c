// File: aerial.c
// Desc:
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
    new->next = NULL;                                //

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

// Função para saber o tamanho do grid
void getGridSize(ED *list, int *xMax, int *yMax)
{
    *xMax = 0;      // Inicia o maior valor de X
    *yMax = 0;      // Inicia o maior valor de Y
    ED *aux = list; // Variável auxiliar para percorrer a lista

    while (aux != NULL)
    {
        if (aux->coordinateX > *xMax)
        {
            *xMax = aux->coordinateX; // Atualiza o maior valor de X
        }
        if (aux->coordinateY > *yMax)
        {
            *yMax = aux->coordinateY; // Atualiza o maior valor de Y
        }
        aux = aux->next; // Avança para o próximo registo da lista
    }
}

// Função para mostrar a lista de antenas
void showAerialList(ED *list)
{
    // Verifica o tamanho do grid
    int xMax, yMax;
    getGridSize(list, &xMax, &yMax); // Calcula o tamanho do grid

    // Cria uma matriz para representar o grid
    char grid[yMax][xMax];

    // Inicializa o grid com pontos: '.'
    for (int y = 0; y < yMax; y++)
    {
        for (int x = 0; x < xMax; x++)
        {
            grid[y][x] = '.';
        }
    }

    // Preenche o grid com as frequências de ressonância das antenas
    ED *aux = list; // Variável auxiliar para percorrer a lista
    while (aux != NULL)
    {
        if (aux->coordinateX <= xMax && aux->coordinateY <= yMax && aux->coordinateX >= 1 && aux->coordinateY >= 1) // Verifica se as coordenadas estão dentro do grid
        {
            grid[aux->coordinateY - 1][aux->coordinateX - 1] = aux->resonanceFrequency; // Preenche o grid com a frequência de ressonância da antena
        }
        aux = aux->next; // Avança para o próximo registo da lista
    }

    // Mostra o grid
    printf("\n\tAntenas:\n");
    for (int y = 0; y < yMax; y++) // Percorre as linhas do grid
    {
        printf("\t");
        for (int x = 0; x < xMax; x++) // Percorre as colunas do grid
        {
            printf("%c ", grid[y][x]); // Mostra o conteúdo do grid
        }
        printf("\n");
    }
    printf("\n");
}