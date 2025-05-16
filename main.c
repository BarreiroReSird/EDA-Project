// File: main.c
// Desc: Este ficheiro contém a função principal do programa, onde são geridas as opções do utilizador e a interação com o utilizador.
// Auth: Carlos Barreiro
// Mail: a20360@alunos.ipca.pt
// Date: 2025/05

#include <stdio.h>
#include <stdlib.h>
#include <windows.h> // Biblioteca para manipular a codificação no Windows
#include "aerial.h"
#include "fileUtils.h"
#include "gridUtils.h"
#include "interference.h"
#include "graph.h"

int main()
{
    SetConsoleOutputCP(65001); // Define a codificação do terminal para UTF-8

    int choice, coordinateX, coordinateY;
    char resonanceFrequency, loadChoice, retryChoice;
    ED *list = NULL;
    GR *resonanceGraph = NULL;

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

            list = loadAerialsFromFile(list, filename);

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
                break;
            }
        } while (1);
    }
    else
    {
        printf("A criar uma nova lista vazia...\n");
    }

    do
    {
        printf("\n\t0 - Sair\n\t1 - Inserir na lista\n\t2 - Mostrar a lista\n\t3 - Remover da lista\n\t4 - Mostrar interferências\n\t5 - Mostrar grafo de ressonância\n\t6 - DFS\n\t7 - BFS\n\tEscolha uma opção: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 0:
            printf("\nA encerrar o programa...\n");
            break;
        case 1:
            printf("Insira a frequência de ressonância da antena: ");
            scanf(" %c", &resonanceFrequency);
            printf("Insira a coordenada X da antena: ");
            scanf("%d", &coordinateX);
            printf("Insira a coordenada Y da antena: ");
            scanf("%d", &coordinateY);
            list = insertAerialEnd(list, resonanceFrequency, coordinateX, coordinateY);
            break;
        case 2:
            showAerialList(list);
            break;
        case 3:
            printf("Insira a coordenada X da antena a remover: ");
            scanf("%d", &coordinateX);
            printf("Insira a coordenada Y da antena a remover: ");
            scanf("%d", &coordinateY);
            list = removeAerial(list, coordinateX, coordinateY);
            break;
        case 4:
            interferencesED(list);
            break;
        case 5:
        {
            if (resonanceGraph != NULL)
            {
                freeGR(resonanceGraph);
            }

            resonanceGraph = createResonanceGraph(list);
            if (!resonanceGraph)
            {
                printf("Erro ao criar grafo ou nenhuma conexão encontrada.\n");
                break;
            }

            int verticesPrinted = printGR(resonanceGraph);
            printf("Mostradas %d conexões no grafo.\n", verticesPrinted);
            break;
        }
        case 6:
            // Libertar o grafo anterior se existir
            if (resonanceGraph != NULL)
            {
                freeGR(resonanceGraph);
            }

            // Criar um novo grafo de ressonância
            resonanceGraph = createResonanceGraph(list);
            if (!resonanceGraph)
            {
                printf("Erro ao criar o grafo ou nenhuma conexão encontrada.\n");
                break;
            }

            printf("Coordenada X da antena inicial: ");
            scanf("%d", &coordinateX);
            printf("Coordenada Y da antena inicial: ");
            scanf("%d", &coordinateY);
            dfsGR(resonanceGraph, coordinateX, coordinateY);
            break;

        case 7:
            // Libertar o grafo anterior se existir
            if (resonanceGraph != NULL)
            {
                freeGR(resonanceGraph);
            }

            // Criar um novo grafo de ressonância
            resonanceGraph = createResonanceGraph(list);
            if (!resonanceGraph)
            {
                printf("Erro ao criar o grafo ou nenhuma conexão encontrada.\n");
                break;
            }

            printf("Coordenada X da antena inicial: ");
            scanf("%d", &coordinateX);
            printf("Coordenada Y da antena inicial: ");
            scanf("%d", &coordinateY);
            bfsGR(resonanceGraph, coordinateX, coordinateY);
            break;
        default:
            printf("\nOpção inválida!\n");
            break;
        }
    } while (choice != 0);

    // Libertar memória
    if (resonanceGraph != NULL)
    {
        freeGR(resonanceGraph);
    }

    return 0;
}