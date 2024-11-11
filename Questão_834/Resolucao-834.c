#include <stdio.h>
#include <stdlib.h>

#define MAX_NODES 30000

// estrutura de cada vertice do grafo
typedef struct {
    int *vizinhos;
    int qntVizinhos;
} GraphNode;

GraphNode graph[MAX_NODES]; // arvore em forma de grafo que vai armazenar cada nó com os vizinhos e qnt de vizinhos
int qntSubArvores[MAX_NODES]; // variavel com o número de nós na subárvore com raiz no vertice passado
int somaDistancias[MAX_NODES]; // variavel de somatorio de distancias a partir do vertice passado

void dfs(int node, int parente) {
    qntSubArvores[node] = 1; // conta a propria subarvore do no
    somaDistancias[node] = 0; // começa a contagem do somatorio de distancias
    for (int i = 0; i < graph[node].qntVizinhos; i++) { // percorre os vizinhos do vertice passado
        int vizinho = graph[node].vizinhos[i]; // define vizinho
        if (vizinho == parente) continue; // desconsidera o vertice anterior/pai
        dfs(vizinho, node); // recursividade para continuar a busca em profundidade em cada vizinho
        qntSubArvores[node] += qntSubArvores[vizinho]; // soma a subarvore do vizinho com a do vertice
        somaDistancias[node] += somaDistancias[vizinho] + qntSubArvores[vizinho]; // Soma os somatorios das distancias dos vertices
    }
}

// essa funcao propaga as informações para todos os nós, para que cada nó tenha sua própria soma de distâncias.
void dfs2(int node, int parente, int n) {
    for (int i = 0; i < graph[node].qntVizinhos; i++) { // percorre os vizinhos do vertice
        int vizinho = graph[node].vizinhos[i]; // define vizinho
        if (vizinho == parente) continue; // desconsidera o vertice anterior/pai
        somaDistancias[vizinho] = somaDistancias[node] - qntSubArvores[vizinho] + (n - qntSubArvores[vizinho]); // formula de re-rooting para calcular o somatorio de distancia para um vertice filho
        dfs2(vizinho, node, n); // recursividade para continuar a busca em profundidade em cada vizinho
    }
}

int* sumOfDistancesInTree(int n, int** edges, int edgesSize, int* edgesColSize, int* returnSize) {
    *returnSize = n;

    int* qntConexoes = (int*)calloc(n, sizeof(int)); // array auxiliar para contar conexões de cada nó
    for (int i = 0; i < edgesSize; i++) { // conta quantos vizinhos cada nó terá
        qntConexoes[edges[i][0]]++;
        qntConexoes[edges[i][1]]++;
    }

    for (int i = 0; i < n; i++) { // aloca memória para cada nó de acordo com a contagem de conexões
        graph[i].vizinhos = (int*)malloc(qntConexoes[i] * sizeof(int));
        graph[i].qntVizinhos = 0;
    }

    for (int i = 0; i < edgesSize; i++) { // monta o grafo a partir das arestas obtidas
        int origem = edges[i][0]; // no de origem da aresta
        int final = edges[i][1]; // no final da aresta
        // adiciona como vizinho em cada nó no grafo
        graph[origem].vizinhos[graph[origem].qntVizinhos++] = final;
        graph[final].vizinhos[graph[final].qntVizinhos++] = origem;
    }

    free(qntConexoes); // libera memória de qntConexoes após construir o grafo

    dfs(0, -1);
    dfs2(0, -1, n);

    int* resultado = (int*)malloc(n * sizeof(int)); // cria lista com resultados
    for (int i = 0; i < n; i++) {
        resultado[i] = somaDistancias[i]; // adiciona da lista de resultado cada somatorio
    }

    for (int i = 0; i < n; i++) {
        free(graph[i].vizinhos); 
    }

    return resultado;
}
