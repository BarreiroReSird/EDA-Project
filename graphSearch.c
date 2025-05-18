// File: graphSearch.c
// Desc:
// Auth: Carlos Barreiro
// Mail: a20360@alunos.ipca.pt
// Date: 2025/05

#include "graph.h"
#include <stdio.h>
#include <stdlib.h> // Para malloc, free

// DFT - Procura em profundidade
// Função auxiliar recursiva para visitar os vértices
int visitDFT(Vertex *v, bool *visited, Graph *graph, int index)
{
    if (!v)
        return 0;

    int count = 1; // Conta o vértice atual
    visited[index] = true;
    printf("Antena: %c (%.0f, %.0f)\n", v->resonanceFrequency, v->coordinateX, v->coordinateY);

    // Criar uma lista temporária de adjacências
    Adjacency *adjList = NULL;
    Adjacency *adj = v->adjacencies;

    // Copiar as adjacências para uma lista temporária
    while (adj)
    {
        Adjacency *newAdj = CreateAdjacency(adj->distance, adj->resonanceFrequency, adj->destinationVertexIndex);
        newAdj->next = adjList;
        adjList = newAdj;
        adj = adj->next;
    }

    // Visitar cada adjacência
    adj = adjList;
    while (adj)
    {
        if (!visited[adj->destinationVertexIndex])
        {
            Vertex *next = graph->head;
            for (int i = 0; next && i < adj->destinationVertexIndex; i++)
                next = next->next;

            if (next && next->resonanceFrequency == v->resonanceFrequency)
                count += visitDFT(next, visited, graph, adj->destinationVertexIndex);
        }
        adj = adj->next;
    }

    // Libertar a lista temporária
    while (adjList)
    {
        Adjacency *temp = adjList;
        adjList = adjList->next;
        free(temp);
    }

    return count;
}

// Função principal para DFT a partir de coordenadas
int DFT_FromCoordinates(float x, float y, Graph *graph)
{
    if (!graph || !graph->head)
    {
        printf("Grafo vazio.\n");
        return 0;
    }

    Vertex *v = graph->head;
    int index = 0;
    while (v && (v->coordinateX != x || v->coordinateY != y))
    {
        v = v->next;
        index++;
    }

    if (!v)
    {
        printf("Antena não encontrada nas coordenadas (%.0f, %.0f)\n", x, y);
        return 0;
    }

    bool *visited = (bool *)calloc(graph->numVertices, sizeof(bool));
    if (!visited)
    {
        printf("Erro de memória.\n");
        return 0;
    }

    printf("DFT a partir da antena %c (%.0f, %.0f):\n", v->resonanceFrequency, x, y);
    int verticesVisited = visitDFT(v, visited, graph, index);
    free(visited);

    printf("Total de vértices visitados: %d\n", verticesVisited);
    return verticesVisited;
}

// BFT - Procura em largura
// Adiciona um nó à fila
int Enqueue(Queue *q, int index)
{
    if (q == NULL)
        return 0;

    QueueNode *newNode = (QueueNode *)malloc(sizeof(QueueNode));
    if (newNode == NULL)
        return 0;

    newNode->index = index;
    newNode->next = NULL;

    if (q->rear == NULL)
    {
        q->front = q->rear = newNode;
    }
    else
    {
        q->rear->next = newNode;
        q->rear = newNode;
    }

    return 1;
}

// Remove um nó da fila
int Dequeue(Queue *q)
{
    if (!q->front)
        return -1;
    QueueNode *temp = q->front;
    int index = temp->index;
    q->front = temp->next;
    if (!q->front)
        q->rear = NULL;
    free(temp);
    return index;
}

// Verifica se a fila está vazia
bool IsQueueEmpty(Queue *q)
{
    return q->front == NULL;
}

// Função principal para BFT a partir de coordenadas
int BFT_FromCoordinates(float x, float y, Graph *graph)
{
    if (!graph || !graph->head)
    {
        printf("Grafo vazio.\n");
        return 0;
    }

    Vertex *v = graph->head;
    int index = 0;
    while (v && (v->coordinateX != x || v->coordinateY != y))
    {
        v = v->next;
        index++;
    }

    if (!v)
    {
        printf("Antena não encontrada nas coordenadas (%.0f, %.0f)\n", x, y);
        return 0;
    }

    char targetResonance = v->resonanceFrequency;
    bool *visited = (bool *)calloc(graph->numVertices, sizeof(bool));
    if (!visited)
    {
        printf("Erro de memória.\n");
        return 0;
    }

    Queue q = {NULL, NULL};
    Enqueue(&q, index);
    visited[index] = true;

    printf("BFT a partir da antena %c (%.0f, %.0f):\n", v->resonanceFrequency, x, y);
    int verticesVisited = 0;

    while (!IsQueueEmpty(&q))
    {
        int currentIndex = Dequeue(&q);
        Vertex *curr = graph->head;
        for (int i = 0; curr && i < currentIndex; i++)
            curr = curr->next;

        if (curr)
        {
            verticesVisited++;
            printf("Antena: %c (%.0f, %.0f)\n", curr->resonanceFrequency, curr->coordinateX, curr->coordinateY);

            Adjacency *adj = curr->adjacencies;
            while (adj)
            {
                if (!visited[adj->destinationVertexIndex])
                {
                    Vertex *neighbor = graph->head;
                    for (int i = 0; neighbor && i < adj->destinationVertexIndex; i++)
                        neighbor = neighbor->next;

                    if (neighbor && neighbor->resonanceFrequency == targetResonance)
                    {
                        Enqueue(&q, adj->destinationVertexIndex);
                        visited[adj->destinationVertexIndex] = true;
                    }
                }
                adj = adj->next;
            }
        }
    }

    free(visited);
    printf("Total de vértices visitados: %d\n", verticesVisited);
    return verticesVisited;
}

// Caminhos entre antenas
// Função auxiliar recursiva para encontrar todos os caminhos entre dois vértices com a mesma frequência de ressonância
int FindAllPathsUtil(Vertex *current, int currentIndex, int endIndex, bool *visited, int *path, int pathIndex, Graph *graph)
{
    int pathCount = 0;

    // Marca o vértice atual como visitado
    visited[currentIndex] = true;

    // Adiciona o índice atual ao caminho
    path[pathIndex] = currentIndex;
    pathIndex++;

    // Se o vértice atual é o destino, mostra o caminho completo
    if (currentIndex == endIndex)
    {
        pathCount++;
        for (int i = 0; i < pathIndex; i++)
        {
            // Navega até o vértice correspondente ao índice armazenado
            Vertex *v = graph->head;
            for (int j = 0; j < path[i]; j++)
                v = v->next;

            // Mostra o vértice no formato: letra(frequência)(x,y)
            printf("%c(%.0f,%.0f)", v->resonanceFrequency, v->coordinateX, v->coordinateY);

            if (i < pathIndex - 1)
                printf(" -> ");
        }
        printf("\n");
    }
    else
    {
        // Percorre todas as adjacências do vértice atual
        Adjacency *adj = current->adjacencies;
        while (adj)
        {
            // Se o vértice destino ainda não foi visitado
            if (!visited[adj->destinationVertexIndex])
            {
                // Encontra o próximo vértice com base no índice de destino
                Vertex *next = graph->head;
                for (int i = 0; i < adj->destinationVertexIndex; i++)
                    next = next->next;

                // Só continua a procura se a frequência for a mesma
                if (next && next->resonanceFrequency == current->resonanceFrequency)
                {
                    // Chamada recursiva para continuar a procura a partir do vértice adjacente
                    pathCount += FindAllPathsUtil(next, adj->destinationVertexIndex, endIndex, visited, path, pathIndex, graph);
                }
            }
            // Avança para a próxima adjacência
            adj = adj->next;
        }
    }

    // Backtracking: desmarca o vértice como visitado e retrocede o caminho
    visited[currentIndex] = false;
    pathIndex--;
    return pathCount;
}

// Função principal que inicia a procura por todos os caminhos entre duas antenas com mesma frequência
int FindAllPaths(Graph *graph, float startX, float startY, float endX, float endY)
{
    if (!graph || !graph->head)
    {
        printf("Grafo vazio.\n");
        return 0;
    }

    Vertex *start = graph->head;
    Vertex *end = graph->head;
    int startIndex = 0, endIndex = 0;
    bool startFound = false, endFound = false;

    // Procura os vértices correspondentes às coordenadas de início e fim
    Vertex *current = graph->head;
    int index = 0;
    while (current)
    {
        if (!startFound && current->coordinateX == startX && current->coordinateY == startY)
        {
            start = current;
            startIndex = index;
            startFound = true;
        }

        if (!endFound && current->coordinateX == endX && current->coordinateY == endY)
        {
            end = current;
            endIndex = index;
            endFound = true;
        }

        current = current->next;
        index++;
    }

    // Verifica se ambos os vértices foram encontrados
    if (!startFound || !endFound)
    {
        printf("Antenas inicial ou final não encontradas.\n");
        return 0;
    }

    // Verifica se ambos os vértices possuem a mesma frequência de ressonância
    if (start->resonanceFrequency != end->resonanceFrequency)
    {
        printf("As antenas não têm a mesma frequência.\n");
        return 0;
    }

    // Aloca memória para controlar os vértices visitados e o caminho atual
    bool *visited = (bool *)calloc(graph->numVertices, sizeof(bool));
    int *path = (int *)malloc(graph->numVertices * sizeof(int));

    // Mostra as informações de início e fim
    printf("Todos os caminhos de %c(%.0f,%.0f) a %c(%.0f,%.0f):\n",
           start->resonanceFrequency, startX, startY,
           end->resonanceFrequency, endX, endY);

    // Inicia a procura pelos caminhos
    int totalPaths = FindAllPathsUtil(start, startIndex, endIndex, visited, path, 0, graph);

    // Mostra o número total de caminhos encontrados
    printf("Total de caminhos encontrados: %d\n", totalPaths);

    // Liberta memória alocada
    free(visited);
    free(path);
    return totalPaths;
}
