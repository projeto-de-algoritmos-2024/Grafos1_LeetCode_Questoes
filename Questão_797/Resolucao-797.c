#include <stdio.h>
#include <stdlib.h>

#define MAX_CAM 100000 //define o número máximo de caminhos
#define MAX_TAM_CAM 100000  //define o tamanho máximo de um caminho

void dfs(int** graph, int no, int graphSize, int* graphColSize, int* caminho, int Ind_caminho, int*** caminhos, int* cont_caminho, int** tam_cam) {
    caminho[Ind_caminho] = no; //adiciona o nó que está sendo visitado ao caminho
    Ind_caminho++; //passa para o próximo nó

    if (no == graphSize - 1) { //verifica se o nó não é o último nó, que no caso de um aciclico direcionado o último não retorna em nenhum nó
        (*caminhos)[*cont_caminho] = (int*)malloc(Ind_caminho * sizeof(int)); //aloca espaço para o caminho até o nó atual
        for (int i = 0; i < Ind_caminho; i++) { //passo por todos os nós até o atual para adicionar ao caminho
            (*caminhos)[*cont_caminho][i] = caminho[i]; //adiciono o nó no caminho
        }
        (*tam_cam)[*cont_caminho] = Ind_caminho; //adiciono o tamanho do caminho atual
        (*cont_caminho)++; //adiciono mais 1 caminho no contador
    } else {
        for (int i = 0; i < graphColSize[no]; i++) {
            dfs(graph, graph[no][i], graphSize, graphColSize, caminho, Ind_caminho, caminhos, cont_caminho, tam_cam);
            //chamo a dfs recursivamente passando os novos dados para verificar se chega ao final e indo adicionando novos caminhos
        }
    }
}

int** allPathsSourceTarget(int** graph, int graphSize, int* graphColSize, int* returnSize, int** returnColumnSizes) {
    int** caminhos = (int**)malloc(MAX_CAM * sizeof(int*));
    int* caminho = (int*)malloc(MAX_TAM_CAM * sizeof(int));
    int* tam_cam = (int*)malloc(MAX_CAM * sizeof(int));
    //aloco espaço na memória para caber os caminhos, tamanhos e utilizarei do vetor caminho para armazenar o caminho que estará sendo verificado

    int cont_caminho = 0; //inicializo um contador de quantidade de caminhos

    dfs(graph, 0, graphSize, graphColSize, caminho, 0, &caminhos, &cont_caminho, &tam_cam); //chamo a dfs no nó 0

    *returnSize = cont_caminho; //coloco o número de caminhos em returnSize
    *returnColumnSizes = tam_cam; //coloco o tamanho dos caminhos em returnColumnSizes

    free(caminho); //libero o espaço que caminho está sendo alocado
    return caminhos; //retorno os caminhos utilizados
}

int main() {
    int graphSize = 4;
    int* graphColSize = (int*)malloc(graphSize * sizeof(int));
    int** graph = (int**)malloc(graphSize * sizeof(int*));

    graphColSize[0] = 2;
    graphColSize[1] = 1;
    graphColSize[2] = 1;
    graphColSize[3] = 0;

    graph[0] = (int*)malloc(2 * sizeof(int));
    graph[1] = (int*)malloc(1 * sizeof(int));
    graph[2] = (int*)malloc(1 * sizeof(int));
    graph[3] = NULL;

    graph[0][0] = 1;
    graph[0][1] = 2;
    graph[1][0] = 3;
    graph[2][0] = 3;

    int returnSize;
    int* returnColumnSizes;
    int** caminhos = allPathsSourceTarget(graph, graphSize, graphColSize, &returnSize, &returnColumnSizes);

    printf("Todos os caminhos de 0 para %d:\n", graphSize - 1);
    for (int i = 0; i < returnSize; i++) {
        for (int j = 0; j < returnColumnSizes[i]; j++) {
            printf("%d ", caminhos[i][j]);
        }
        printf("\n");
        free(caminhos[i]);
    }

    free(caminhos);
    free(returnColumnSizes);
    for (int i = 0; i < graphSize; i++) {
        free(graph[i]);
    }
    free(graph);
    free(graphColSize);

    return 0;
}