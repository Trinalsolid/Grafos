#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_VERTICES 10  // Máximo de vértices
#define PROBABILIDADE_ARESTA 0.4  // Probabilidade de adicionar uma aresta entre dois vértices

// Definição da estrutura do grafo
typedef struct Grafo {
    int adj[MAX_VERTICES][MAX_VERTICES];  // Matriz de adjacência
    int numVertices;
} Grafo;

// Função para inicializar o grafo
void inicializarGrafo(Grafo *g, int vertices) {
    g->numVertices = vertices;
    
    // Inicializar a matriz de adjacência com zeros
    for (int i = 0; i < vertices; i++) {
        for (int j = 0; j < vertices; j++) {
            g->adj[i][j] = 0;
        }
    }
}

// Função para adicionar aresta aleatória no grafo
void adicionarArestasAleatorias(Grafo *g) {
    for (int i = 0; i < g->numVertices; i++) {
        for (int j = i + 1; j < g->numVertices; j++) {
            // Adiciona uma aresta com base na probabilidade definida
            if ((rand() / (float) RAND_MAX) < PROBABILIDADE_ARESTA) {
                g->adj[i][j] = 1;
                g->adj[j][i] = 1;  // Grafo não direcionado
            }
        }
    }
}

// Função para verificar se a permutação forma um ciclo
int verificaCiclo(Grafo *g, int *vetorPermutacao, int n) {
    for (int i = 0; i < n - 1; i++) {
        if (g->adj[vetorPermutacao[i]][vetorPermutacao[i + 1]] == 0) {
            return 0;  // Não é ciclo
        }
    }
    if (g->adj[vetorPermutacao[n - 1]][vetorPermutacao[0]] == 0) {
        return 0;  // Não é ciclo
    }
    return 1;  // É um ciclo
}

// Função para gerar permutações e contar ciclos
/*void permutacao(Grafo *g, int *vetorPermutacao, int start, int n, int *ciclos) {
    if (start == n) {
        if (verificaCiclo(g, vetorPermutacao, n)) {
            (*ciclos)++;
        }
        return;
    }
    for (int i = start; i < n; i++) {
        // Troca
        int temp = vetorPermutacao[start];
        vetorPermutacao[start] = vetorPermutacao[i];
        vetorPermutacao[i] = temp;

        // Chama recursivamente
        permutacao(g, vetorPermutacao, start + 1, n, ciclos);

        // Desfaz a troca
        temp = vetorPermutacao[start];
        vetorPermutacao[start] = vetorPermutacao[i];
        vetorPermutacao[i] = temp;
    }
}*/

// Função para contar ciclos por permutação
/*int contarCiclosPorPermutacao(Grafo *g) {
    int vetorPermutacao[MAX_VERTICES];
    for (int i = 0; i < g->numVertices; i++) {
        vetorPermutacao[i] = i;
    }

    int ciclos = 0;
    permutacao(g, vetorPermutacao, 0, g->numVertices, &ciclos);
    return ciclos;
}*/

// Função para detectar ciclos usando DFS
int DFS(Grafo *g, int vertice, int *visitado, int *pai) {
    visitado[vertice] = 1;

    // Verifica todos os vizinhos
    for (int i = 0; i < g->numVertices; i++) {
        if (g->adj[vertice][i] == 1) {  // Se existe uma aresta
            // Se o vizinho não foi visitado, faz a DFS nele
            if (!visitado[i]) {
                pai[i] = vertice;
                if (DFS(g, i, visitado, pai)) {
                    return 1;  // Ciclo encontrado
                }
            }
            // Se já foi visitado e não é o pai, é um ciclo
            else if (i != pai[vertice]) {
                return 1;
            }
        }
    }
    return 0;
}

// Função para contar ciclos no grafo usando DFS
int contarCiclosPorDFS(Grafo *g) {
    int visitado[MAX_VERTICES] = {0};
    int pai[MAX_VERTICES] = {-1};
    int ciclos = 0;

    // Chama DFS para todos os vértices
    for (int i = 0; i < g->numVertices; i++) {
        if (!visitado[i]) {
            if (DFS(g, i, visitado, pai)) {
                ciclos++;  // Incrementa o contador de ciclos
            }
        }
    }
    return ciclos;
}

void medirTempo(Grafo *g) {
    clock_t start, end;
    double tempoDFS, tempoPermutacao;

    // Medir o tempo para DFS
    start = clock();
    int ciclosDFS = contarCiclosPorDFS(g);
    end = clock();
    tempoDFS = ((double)(end - start)) / CLOCKS_PER_SEC;

    // Medir o tempo para Permutação
    //start = clock();
    //int ciclosPermutacao = contarCiclosPorPermutacao(g);
    //end = clock();
    //tempoPermutacao = ((double)(end - start)) / CLOCKS_PER_SEC;

    // Exibir os resultados
    printf("Ciclos encontrados (DFS): %d\n", ciclosDFS);
    printf("Tempo gasto com DFS: %.6f segundos\n", tempoDFS);

    //printf("Ciclos encontrados (Permutação): %d\n", ciclosPermutacao);
    //printf("Tempo gasto com Permutação: %.6f segundos\n", tempoPermutacao);
}

int main() {
    srand(time(NULL));  // Inicializa o gerador de números aleatórios com o tempo atual
    Grafo g;
    int numVertices = 6;  // Número de vértices do grafo (você pode mudar para 10 ou outro valor)

    inicializarGrafo(&g, numVertices);

    // Adicionando arestas aleatórias
    adicionarArestasAleatorias(&g);

    // Medir o tempo de ambos os métodos
    medirTempo(&g);

    return 0;
}