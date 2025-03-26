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

// Função para carregar as antenas de um ficheiro
void loadAerialsFromFile(ED **list, const char *filename)
{
    int y = 1;       // Para iniciar a coordenada Y
    char line[1000]; // Linha do ficheiro

    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Erro ao abrir o ficheiro %s\n", filename);
        return;
    }

    // Lê o ficheiro linha a linha
    while (fgets(line, sizeof(line), file) != NULL)
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

// Função para remover uma antena da lista
void removeAerial(ED **list, int coordinateX, int coordinateY)
{
    if (*list == NULL) // Verifica se a lista está vazia
    {
        printf("Lista de antenas vazia. Nada para remover.\n");
        return;
    }

    ED *current = *list; // Variável auxiliar para percorrer a lista
    ED *previous = NULL; // Variável auxiliar para guardar o registo anterior

    // Percorre a lista à procura da antena
    while (current != NULL)
    {
        if (current->coordinateX == coordinateX && current->coordinateY == coordinateY) // Verifica se a antena foi encontrada
        {
            // Remove a antena da lista
            if (previous == NULL)
            {
                // Se for o primeiro elemento
                *list = current->next;
            }
            else
            {
                // Se for um elemento do meio ou fim
                previous->next = current->next;
            }

            free(current); // Liberta a memória alocada para a antena
            printf("Antena na posição (%d, %d) removida com sucesso.\n", coordinateX, coordinateY);
            return;
        }

        previous = current;      // Guarda o registo anterior
        current = current->next; // Avança para o próximo registo
    }

    // A antena não foi encontrada
    printf("Antena na posição (%d, %d) não encontrada.\n", coordinateX, coordinateY);
}