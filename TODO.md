# Otimizar

    - Remover todos os 'printf' (fora do main.c);
    - Antes de adicionar uma aresta, verifique se ela já existe para evitar duplicação;
    - Atualmente, o peso é fixo em 1.0;

# Erros

    - Letras no switch-case;

# Tarefas

    - Enunciado: https://elearning.ipca.pt/2425/pluginfile.php/831554/mod_resource/content/1/Projeto_EDA.pdf

## Fase 2

    (Feito)
    - Definir um tipo de dados GR para representação de grafos, permitindo suportar um número qualquer de vértices. Cada vértice deverá representar uma antena e a sua localização numa cidade. As arestas deverão interligar somente antenas com frequências de ressonância iguais.
    - Carregamento dos dados referentes às antenas de uma cidade, constantes num ficheiro de texto, devendo dar origem a um grafo com tipo de dados GR. A título de exemplo, o ficheiro de texto deverá respeitar o formato seguinte: https://prnt.sc/8crVKs3lBjn9
    - Implementar operações de manipulação de grafos do tipo GR, incluindo:
        - a. Procura em profundidade a partir de uma determinada antena, listando na consola as coordenadas das antenas alcançadas;
        - b. Procura em largura a partir de uma determinada antena, listando na consola as coordenadas das antenas alcançadas;
        - c. Identificar todos os caminhos possíveis entre duas determinadas antenas, listando na consola as sequência de arestas interligando as antenas;
        (Por fazer:)
        - d. Dadas duas frequências de ressonância distintas A e B, listar na consola todas as intersecções de pares de antenas com ressonância A e B, indicando as respectivas coordenadas.
