// File: main.c
// Desc:
// Auth: Carlos Barreiro
// Mail: a20360@alunos.ipca.pt
// Date: 2025/03

// To do list:
// 1. Definição de uma estrutura de dados ED, para a representação das antenas, sob a forma de uma lista ligada simples.
//      Cada registo da lista ligada deverá conter a frequência de ressonância de uma antena e suas coordenadas;
// 2. Carregamento para uma estrutura de dados ED dos dados das antenas constantes num ficheiro de texto. A operação deverá considerar matrizes de caracteres com qualquer dimensão;
// 3. Implementar operações de manipulação da lista ligada do tipo ED, incluindo:
//      a. Inserção de uma nova antena na lista ligada;
//      b. Remoção de uma antena constante na lista ligada;
//      c. Dedução automática das localizações com efeito nefasto e respetiva representação sob a forma de uma lista ligada;
//      d. Listagem de forma tabular na consola das antenas e localizações com efeito nefasto.

#include <stdio.h>
#include <stdlib.h>
#include <windows.h> // Biblioteca para manipular a codificação no Windows
#include "aerial.h"

int main()
{
    SetConsoleOutputCP(65001); // Define a codificação do terminal para UTF-8

    int choice, coordinateX, coordinateY;
    char resonanceFrequency;
    ED *list = NULL;

    do
    {
        printf("\n\t0 - Sair\n\t1 - Inserir na lista\n\t2 - Mostrar a lista\n");
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
        default:
            break;
        }
    } while (choice != 0);

    return 0;
}