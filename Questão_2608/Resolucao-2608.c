#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

//struct de um n� que usa lista encadeada
typedef struct No {
    int vertice;
    struct No* prox;
} No;

//struct do grafo com lista encadeada
typedef struct {
    No** adj;
    int numVertices;
} Grafo;

//struct da fila com os dados, inicio, fim, numero de elementos e capacidade m�xima da fila
typedef struct {
    int* data;
    int inicio;
    int fim;
    int tamanho;
    int capacidade;
} Fila;

//fun��o para criar um grafo
Grafo* novoGrafo(int numVertices) {
    Grafo* grafo = (Grafo*)malloc(sizeof(Grafo));

    grafo->numVertices = numVertices;
    grafo->adj = (No**)malloc(numVertices * sizeof(No*));

    for (int i = 0; i < numVertices; i++) {
        grafo->adj[i] = NULL;
    }

    return grafo;
}

//fun��o para criar um novo n�
No* criaNo(int v) {
    No* novoNo = (No*)malloc(sizeof(No));

    novoNo->vertice = v;
    novoNo->prox = NULL;

    return novoNo;
}

//fun��o para adicionar uma aresta em um grafo passando a origem "ori" � um destino "dest"
void criaAresta(Grafo* grafo, int ori, int dest) {
    //adiciona 'dest' � lista de adjac�ncia de 'ori'
    No* novoNo = criaNo(dest);
    novoNo->prox = grafo->adj[ori];
    grafo->adj[ori] = novoNo;

    //adiciona 'ori' � lista de adjac�ncia de 'dest'
    novoNo = criaNo(ori);
    novoNo->prox = grafo->adj[dest];
    grafo->adj[dest] = novoNo;

}

//fun��o para inicializar a fila
Fila* iniciaFila(int capacidade) {
    Fila* fila = (Fila*)malloc(sizeof(Fila));

    fila->capacidade = capacidade;
    fila->inicio = fila->tamanho = 0;
    fila->fim = capacidade - 1;
    fila->data = (int*)malloc(fila->capacidade * sizeof(int));

    return fila;
}

//fun��o para verificar se a fila est� vazia
bool verificaFila(Fila* fila) {
    return (fila->tamanho == 0);
}

//fun��o para adicionar um item � fila
void enfila(Fila* fila, int item) {
    fila->fim = (fila->fim + 1) % fila->capacidade;
    fila->data[fila->fim] = item;
    fila->tamanho = fila->tamanho + 1;

}

//fun��o para remover um item da fila
int desenfila(Fila* fila) {
    int item = fila->data[fila->inicio];

    fila->inicio = (fila->inicio + 1) % fila->capacidade;
    fila->tamanho = fila->tamanho - 1;

    return item;
}

//fun��o de bfs em um grafo com inicio, arestas e v�rtices que ser�o ignorados
int bfs(Grafo* grafo, int inicio, int ignoreU, int ignoreV) {
    int inf = INT_MAX / 2; // Reduzindo o valor m�ximo para evitar overflow
    int* dist = (int*)malloc(grafo->numVertices * sizeof(int));

    //inicializa todas as dist�ncias com um tamanho m�ximo
    for (int i = 0; i < grafo->numVertices; i++) {
        dist[i] = inf;
    }

    //define a dist�ncia inicial como 0
    Fila* fila = iniciaFila(grafo->numVertices);
    dist[inicio] = 0;
    enfila(fila, inicio);

    //come�a a BFS
    while (!verificaFila(fila)) {
        int u = desenfila(fila);
        No* temp = grafo->adj[u];

        while (temp != NULL) {
            int v = temp->vertice;
            //verifico se a aresta ou v�rtice j� foi visitado
            if ((u == ignoreU && v == ignoreV) || (u == ignoreV && v == ignoreU) || dist[v] != inf) {
                temp = temp->prox;
                continue;
            }

            dist[v] = dist[u] + 1;
            enfila(fila, v);
            temp = temp->prox;
        }
    }

    int result = dist[ignoreV] + 1;
    free(dist);
    free(fila->data);
    free(fila);
    return result;
}

//fun��o para encontrar o menor ciclo
int findShortestCycle(int n, int** edges, int edgesSize, int* edgesColSize) {
    Grafo* grafo = novoGrafo(n);

    //adiciona as arestas ao grafo
    for (int i = 0; i < edgesSize; i++) {
        criaAresta(grafo, edges[i][0], edges[i][1]);
    }

    const int inf = INT_MAX / 2; // Reduzindo o valor m�ximo para evitar overflow
    int res = inf;

    //para cada aresta calcula o menor ciclo excluindo temporariamente a aresta
    for (int i = 0; i < edgesSize; i++) {
        int u = edges[i][0];
        int v = edges[i][1];

        //armazeno o tamanho do ciclo
        int tamCiclo = bfs(grafo, u, u, v);

        //atualizo se um componente desconectado tenha um ciclo maior
        if (tamCiclo < res) {
            res = tamCiclo;
        }
    }

    for (int i = 0; i < n; i++) {
        No* temp = grafo->adj[i];
        while (temp != NULL) {
            No* novo = temp->prox;
            free(temp);
            temp = novo;
        }
    }
    free(grafo->adj);
    free(grafo);


    if (res < inf) {
        return res;
    } else {
        return -1;
    }
}

