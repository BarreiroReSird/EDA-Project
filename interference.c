// File: fileUtils.c
// Desc: Este ficheiro contém funções para carregar e guardar dados de antenas em ficheiros, bem como para mostrar a lista de antenas.
// Auth: Carlos Barreiro
// Mail: a20360@alunos.ipca.pt
// Date: 2025/03

#include <stdio.h>
#include <stdlib.h>
#include <math.h> // Para abs() que calcula o valor absoluto
#include "interference.h"
#include "aerial.h"
#include "gridUtils.h"

void interferencesED(ED *list)
{
    // Cria uma lista ligada interferencesED para guardar as interferencias
    ED *interferencesED = NULL;

    // Verifica se há frequências de ressonância repetidas
    int hasDuplicates = 0;
    ED *current = list;
    while (current != NULL)
    {
        ED *temp = current->next; // Aponta para o próximo registo da lista
        while (temp != NULL)
        {
            // Compara a frequência atual com a próxima
            if (temp->resonanceFrequency == current->resonanceFrequency)
            {
                // Frequência repetida encontrada
                hasDuplicates = 1;
                break;
            }
            temp = temp->next; // Avança para o próximo registo temp
        }
        if (hasDuplicates) // Se encontrar uma frequência repetida, sai do loop
            break;
        current = current->next; // Avança para o próximo registo current
    }

    if (!hasDuplicates) // Se não houver frequências repetidas, mostra a lista normal
    {
        printf("Não há frequências de ressonância repetidas.\n");
        showAerialList(list);
        return;
    }

    // Copia toda a lista ligada ED para a lista ligada interferencesED
    current = list;
    while (current != NULL)
    {
        // Adiciona a antena à lista de interferências
        insertAerialEnd(&interferencesED, current->resonanceFrequency, current->coordinateX, current->coordinateY);
        current = current->next;
    }

    // Processa cada frequência
    current = list;
    while (current != NULL)
    {
        ED *other = current->next;
        while (other != NULL)
        {
            // Verifica se as frequências são iguais
            if (current->resonanceFrequency == other->resonanceFrequency)
            {
                // Calcula a diferença nas coordenadas
                int diffX = other->coordinateX - current->coordinateX;
                int diffY = other->coordinateY - current->coordinateY;

                // Verifica se a diferença é pelo menos +2 ou -2 em X ou Y
                if (abs(diffX) >= 2 || abs(diffY) >= 2)
                {
                    // Calcula a primeira interferência (troca sinais do caminho)
                    int interferenceX1 = current->coordinateX - diffX;
                    int interferenceY1 = current->coordinateY - diffY;

                    // Verifica se as coordenadas são positivas
                    if (interferenceX1 > 0 && interferenceY1 > 0)
                    {
                        // Verifica se já existe uma antena nesta posição
                        int isAerial = 0;
                        ED *check = list;
                        while (check != NULL)
                        {
                            if (check->coordinateX == interferenceX1 && check->coordinateY == interferenceY1)
                            {
                                isAerial = 1;
                                break;
                            }
                            check = check->next;
                        }

                        if (!isAerial)
                        {
                            insertAerialEnd(&interferencesED, '#', interferenceX1, interferenceY1);
                        }
                    }

                    // Calcula a segunda interferência (troca sinais do caminho)
                    int interferenceX2 = other->coordinateX + diffX;
                    int interferenceY2 = other->coordinateY + diffY;

                    // Verifica se as coordenadas são positivas
                    if (interferenceX2 > 0 && interferenceY2 > 0)
                    {
                        // Verifica se já existe uma antena nesta posição
                        int isAerial = 0;
                        ED *check = list;
                        while (check != NULL)
                        {
                            if (check->coordinateX == interferenceX2 && check->coordinateY == interferenceY2)
                            {
                                isAerial = 1;
                                break;
                            }
                            check = check->next;
                        }

                        if (!isAerial)
                        {
                            insertAerialEnd(&interferencesED, '#', interferenceX2, interferenceY2);
                        }
                    }
                }
            }
            other = other->next;
        }
        current = current->next;
    }

    // Mostra o grid com as interferências
    printf("\nInterferências calculadas:\n");
    showAerialList(interferencesED);

    // Liberta a memória da lista de interferências
    while (interferencesED != NULL)
    {
        ED *temp = interferencesED;
        interferencesED = interferencesED->next;
        free(temp);
    }
}