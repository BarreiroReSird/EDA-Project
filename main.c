// File: main.c
// Desc:
// Auth: Carlos Barreiro
// Mail: a20360@alunos.ipca.pt
// Date: 2025/03

// Errors to fix:
//      - 1. Quando o ficheiro a ser carregado não existe, ou não é possível abrir, o programa não deixa tentar novamente nem termina a execução.

// Otimizations:
//      - 1. Bloquear a adição de antenas com a frequência de ressonância sejam simbolos, numeros e pontuações. (Aceitar apenas letras)
//      - 2. Adicionar verificação das coordenadas para não serem negativas ou nulas.
//      - 3. Libertar a memória alocada em todo o programa, no final do mesmo.

#include <stdio.h>
#include <stdlib.h>
#include <windows.h> // Biblioteca para manipular a codificação no Windows
#include "aerial.h"
#include "fileUtils.h"
#include "gridUtils.h"
#include "interference.h"

int main()
{
    SetConsoleOutputCP(65001); // Define a codificação do terminal para UTF-8

    int choice, coordinateX, coordinateY;
    char resonanceFrequency, loadChoice, retryChoice;
    ED *list = NULL;

    // Carregar o ficheiro ou não
    printf("Carregar a posição das antenas de um ficheiro? (s/n): ");
    scanf(" %c", &loadChoice);

    if (loadChoice == 's' || loadChoice == 'S')
    {
        char filename[100];
        do
        {
            printf("Qual o nome do ficheiro: ");
            scanf("%s", filename);

            // Tenta carregar o arquivo diretamente
            loadAerialsFromFile(&list, filename);

            // Verifica se a lista foi preenchida (arquivo carregado com sucesso)
            if (list == NULL)
            {
                printf("Erro ao abrir o ficheiro %s. Deseja tentar novamente? (s/n): ", filename);
                scanf(" %c", &retryChoice);
                if (retryChoice == 'n' || retryChoice == 'N')
                {
                    break;
                }
            }
            else
            {
                break; // Sai do loop se o arquivo foi carregado com sucesso
            }
        } while (1);
    }
    else
    {
        printf("A criar uma nova lista vazia...\n");
    }

    do
    {
        printf("\n\t0 - Sair\n\t1 - Inserir na lista\n\t2 - Mostrar a lista\n\t3 - Remover da lista\n\t4 - Mostrar interferências\n\tEscolha uma opção: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            printf("Insira a frequência de ressonância da antena: ");
            scanf(" %c", &resonanceFrequency);
            printf("Insira a coordenada X da antena: ");
            scanf("%d", &coordinateX);
            printf("Insira a coordenada Y da antena: ");
            scanf("%d", &coordinateY);
            insertAerialEnd(&list, resonanceFrequency, coordinateX, coordinateY);
            break;
        case 2:
            showAerialList(list);
            break;
        case 3:
            printf("Insira a coordenada X da antena a remover: ");
            scanf("%d", &coordinateX);
            printf("Insira a coordenada Y da antena a remover: ");
            scanf("%d", &coordinateY);
            removeAerial(&list, coordinateX, coordinateY);
            break;
        case 4:
            interferencesED(list);
            break;
        default:
            break;
        }
    } while (choice != 0);

    return 0;
}