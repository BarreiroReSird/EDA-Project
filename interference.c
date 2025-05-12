// File: interference.c
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

int interferencesED(ED *list)
{
    ED *interferencesED = NULL;
    int hasDuplicates = 0;

    ED *current = list;
    while (current != NULL)
    {
        ED *temp = current->next;
        while (temp != NULL)
        {
            if (temp->resonanceFrequency == current->resonanceFrequency)
            {
                hasDuplicates = 1;
                break;
            }
            temp = temp->next;
        }
        if (hasDuplicates)
            break;
        current = current->next;
    }

    if (!hasDuplicates)
    {
        printf("Não há frequências de ressonância repetidas.\n");
        showAerialList(list);
        return 0;
    }

    current = list;
    while (current != NULL)
    {
        interferencesED = insertAerialEnd(interferencesED, current->resonanceFrequency, current->coordinateX, current->coordinateY);
        current = current->next;
    }

    current = list;
    while (current != NULL)
    {
        ED *other = current->next;
        while (other != NULL)
        {
            if (current->resonanceFrequency == other->resonanceFrequency)
            {
                int diffX = other->coordinateX - current->coordinateX;
                int diffY = other->coordinateY - current->coordinateY;

                if (abs(diffX) >= 2 || abs(diffY) >= 2)
                {
                    int interferenceX1 = current->coordinateX - diffX;
                    int interferenceY1 = current->coordinateY - diffY;

                    if (interferenceX1 > 0 && interferenceY1 > 0)
                    {
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
                            interferencesED = insertAerialEnd(interferencesED, '#', interferenceX1, interferenceY1);
                        }
                    }

                    int interferenceX2 = other->coordinateX + diffX;
                    int interferenceY2 = other->coordinateY + diffY;

                    if (interferenceX2 > 0 && interferenceY2 > 0)
                    {
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
                            interferencesED = insertAerialEnd(interferencesED, '#', interferenceX2, interferenceY2);
                        }
                    }
                }
            }
            other = other->next;
        }
        current = current->next;
    }

    printf("\nInterferências calculadas:\n");
    showAerialList(interferencesED);

    while (interferencesED != NULL)
    {
        ED *temp = interferencesED;
        interferencesED = interferencesED->next;
        free(temp);
    }
    return 0;
}