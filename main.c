// File: main.c
// Desc:
// Auth: Carlos Barreiro
// Mail: a20360@alunos.ipca.pt
// Date: 2025/03

// Project objectives:
//      Pretende-se considerar uma cidade com várias antenas. Cada antena é sintonizada numa frequência específica indicada por um caracter.
//      O mapa das antenas com as suas localizações (coordenadas na matriz) e frequências é representado através de uma matriz.
//      Considere que o sinal de cada antena aplica um efeito nefasto em localizações específicas L com base nas frequências de ressonância das antenas.
//      Em particular, o efeito nefasto ocorre em qualquer localização L que esteja perfeitamente alinhada com duas antenas da mesma frequência,
//      mas apenas quando uma das antenas está duas vezes mais distante que a outra. Isso significa que para qualquer par de antenas com a mesma
//      frequência, existem duas localizações, uma de cada lado das antenas.
//      A título de exemplo, para duas antenas com frequência a, as localizações com efeito nefasto encontram-se representadas com #.
//      Adicionar uma terceira antena com a mesma frequência cria várias localizações adicionais com efeito nefasto.
//      Antenas com frequências diferentes (caracteres diferentes) não criam localizações com efeito nefasto.
//      Localizações com efeito nefasto podem ocorrer em locais que contêm antenas. Uma localização com efeito nefasto pode surgir na sequência das várias
//      combinações de antenas em simultâneo.

// To do list:
// 1. Definição de uma estrutura de dados ED, para a representação das antenas, sob a forma de uma lista ligada simples.
//      Cada registo da lista ligada deverá conter a frequência de ressonância de uma antena e suas coordenadas;
// 2. Carregamento para uma estrutura de dados ED dos dados das antenas constantes num ficheiro de texto. A operação deverá considerar matrizes de caracteres com qualquer dimensão;
// 3. Implementar operações de manipulação da lista ligada do tipo ED, incluindo:
//      a. Inserção de uma nova antena na lista ligada;
//      b. Remoção de uma antena constante na lista ligada;
//      c. Dedução automática das localizações com efeito nefasto e respetiva representação sob a forma de uma lista ligada;
//      d. Listagem de forma tabular na consola das antenas e localizações com efeito nefasto.

// Errors to fix:
//      - 1. Quando o ficheiro a ser carregado não existe, ou não é possível abrir, o programa não deixa tentar novamente nem termina a execução.

// Otimizations:
//      - 1. Dividir melhor o projeto, em diversos ficheiros, para facilitar a leitura e manutenção do código.
//      - 2. Bloquear a adição de antenas com a frequência de ressonância igual a "#", e pontuações.

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
    char resonanceFrequency;
    char loadChoice;
    ED *list = NULL;

    // Carregar de ficheiro ou não
    printf("Carregar a posição das antenas de um ficheiro? (s/n): ");
    scanf(" %c", &loadChoice);

    if (loadChoice == 's' || loadChoice == 'S')
    {
        char filename[100];
        printf("Qual o nome do ficheiro: ");
        scanf("%s", filename);
        loadAerialsFromFile(&list, filename);
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