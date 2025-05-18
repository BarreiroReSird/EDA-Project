// File: main.c
// Desc:
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

// Função para executar a Fase 1 ()
int runPart1()
{
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
        printf("\n\t0 - Sair\n\t1 - Inserir na lista\n\t2 - Mostrar a lista\n\t3 - Remover da lista\n\t4 - Mostrar interferências\n\tEscolha uma opção: ");
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

        default:
            printf("\nOpção inválida!\n");
            break;
        }
    } while (choice != 0);
    return 1;
}

// Função para executar a Fase 2 (Grafos)
int runPart2()
{
    Graph *graph = (Graph *)malloc(sizeof(Graph));
    if (graph == NULL)
    {
        printf("Erro: Falha ao alocar memória para o grafo.\n");
        return 0;
    }

    graph->numVertices = 0;
    graph->head = NULL;

    int choice, result;
    char resonance;
    float x, y;
    char fileName[100];

    do
    {
        printf("\n--- Menu ---\n");
        printf("1. Adicionar antena\n");
        printf("2. Mostrar o grafo\n");
        printf("3. Mostrar grafo atraves de grid\n");
        printf("4. Carregar ficheiro\n");
        printf("5. DFT\n");
        printf("6. BFT\n");
        printf("7. Mostrar todos os caminhos entre duas antenas\n");
        printf("8. Mostrar interseções entre duas frequências\n");
        printf("0. Sair\n");
        printf("Escolha: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            printf("Frequência da antena: ");
            scanf(" %c", &resonance);
            printf("Coordenada X: ");
            scanf("%f", &x);
            printf("Coordenada Y: ");
            scanf("%f", &y);

            {
                Vertex *v = CreateVertex(resonance, x, y);
                int res;
                graph->head = InsertVertex(v, graph->head, &res);
                if (res)
                {
                    graph->numVertices++;
                    printf("A antena foi adicionada.\n");
                }
                else
                {
                    printf("A antena não foi adicionada.\n");
                }
            }
            break;

        case 2:
            if (graph == NULL)
            {
                printf("Erro: Grafo não inicializado.\n");
            }
            else
            {
                result = ShowGraph(graph);
                if (result == 0)
                {
                    printf("Nenhum vértice foi mostrado (erro).\n");
                }
                else
                {
                    printf("Total de %d vértices mostrados.\n", result);
                }
            }
            break;

        case 3:
            if (graph == NULL)
            {
                printf("Erro: Grafo não inicializado.\n");
            }
            else
            {
                result = ShowGraphAsGrid(graph);
                if (result == 0)
                {
                    printf("Nenhum vértice foi mostrado no grid (erro).\n");
                }
                else
                {
                    printf("Grid mostrado com %d vértices.\n", result);
                }
            }
            break;

        case 4:
            printf("Nome do ficheiro e formato: ");
            scanf("%s", fileName);
            if (LoadGraph(fileName, graph))
            {
                printf("Grafo carregado com sucesso.\n");
            }
            else
            {
                printf("Grafo não carregado.\n");
            }
            break;

        case 5:
            printf("Coordenada X: ");
            scanf("%f", &x);
            printf("Coordenada Y: ");
            scanf("%f", &y);
            DFT_FromCoordinates(x, y, graph);
            break;

        case 6:
            printf("Coordenada X: ");
            scanf("%f", &x);
            printf("Coordenada Y: ");
            scanf("%f", &y);
            BFT_FromCoordinates(x, y, graph);
            break;

        case 7:
            printf("Coordenada X da antena inicial: ");
            scanf("%f", &x);
            printf("Coordenada Y da antena inicial: ");
            scanf("%f", &y);
            float startX = x, startY = y;

            printf("Coordenada X da antena final: ");
            scanf("%f", &x);
            printf("Coordenada Y da antena final: ");
            scanf("%f", &y);
            FindAllPaths(graph, startX, startY, x, y);
            break;

        case 8:
        {
            char freqA, freqB;
            printf("Primeira frequência: ");
            scanf(" %c", &freqA);
            printf("Segunda frequência: ");
            scanf(" %c", &freqB);
            int result = findIntersections(graph, freqA, freqB);
            printf("Total de %d interseções encontradas.\n", result);
            break;
        }

        case 0:
            printf("A encerrar...\n");
            break;

        default:
            printf("Escolha inválida.\n");
            break;
        }
    } while (choice != 0);
    FreeGraph(graph); // Liberta toda a memória alocada
    return 1;
}

int main()
{
    SetConsoleOutputCP(65001); // Define a codificação do terminal para UTF-8

    int partChoice;

    printf("\nProjecto de EDA\n");
    printf("Escolha a fase que deseja executar:\n");
    printf("1 - Fase 1: Gestão de antenas\n");
    printf("2 - Fase 2: Teste de grafo\n");
    printf("Escolha: ");
    scanf("%d", &partChoice);

    switch (partChoice)
    {
    case 0:
        printf("\nA encerrar...\n");
        break;

    case 1:
        runPart1();
        break;

    case 2:
        runPart2();
        break;

    default:
        printf("\nOpção inválida! A encerrar...\n");
        break;
    }

    return 0;
}