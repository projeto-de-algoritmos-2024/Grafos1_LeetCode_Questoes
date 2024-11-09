#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NODES 30000

typedef struct {
    int *vizinhos;
    int qntVizinhos;
} GraphNode;

GraphNode graph[MAX_NODES]; //arvore em forma de grafo que vai armazenar cada nó com os vizinhos e qnt de vizinhos
int qntSubArvores[MAX_NODES];
int somaDistancias[MAX_NODES];

void dfs(int node, int parente) {
    qntSubArvores[node] = 1;
    somaDistancias[node] = 0;
    for (int i = 0; i < graph[node].qntVizinhos; i++) {
        int vizinho = graph[node].vizinhos[i];
        if (vizinho == parente) continue;
        dfs(vizinho, node);
        qntSubArvores[node] += qntSubArvores[vizinho];
        somaDistancias[node] += somaDistancias[vizinho] + qntSubArvores[vizinho];
    }
}

void dfs2(int node, int parente, int n) {
    for (int i = 0; i < graph[node].qntVizinhos; i++) {
        int vizinho = graph[node].vizinhos[i];
        if (vizinho == parente) continue;
        somaDistancias[vizinho] = somaDistancias[node] - qntSubArvores[vizinho] + (n - qntSubArvores[vizinho]);
        dfs2(vizinho, node, n);
    }
}

int* sumOfDistancesInTree(int n, int** edges, int edgesSize, int* edgesColSize, int* returnSize) {
    *returnSize = n;
    for (int i = 0; i < n; i++) { // monta a estrutura de cada nó do grafo/arvore
        graph[i].vizinhos = (int*)malloc(n * sizeof(int));
        graph[i].qntVizinhos = 0;
    }

    for (int i = 0; i < edgesSize; i++) { //monta o grafo a partir das arestas obtidas
        int origem = edges[i][0]; // no de origem da aresta
        int final = edges[i][1]; // no final da aresta
        // adiciona como vizinho em cada nó no grafo
        graph[origem].vizinhos[graph[origem].qntVizinhos++] = final;
        graph[final].vizinhos[graph[final].qntVizinhos++] = origem;
    }

    dfs(0, -1);
    dfs2(0, -1, n);

    int* resultado = (int*)malloc(n * sizeof(int)); // cria lista com resultados
    for (int i = 0; i < n; i++) {
        resultado[i] = somaDistancias[i]; //adiciona da lista de resultado cada somatorio
    }

    for (int i = 0; i < n; i++) {
        free(graph[i].vizinhos);
    }

    return resultado;
}
