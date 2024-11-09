#include <stdbool.h>
#include <stdlib.h>

bool isBipartite(int** graph, int graphSize, int* graphColSize) {
    int* verticeColors = (int*)calloc(graphSize, sizeof(int));  // Cria um array de todos os nós como não coloridos
    
    for (int i = 0; i < graphSize; i++) { // for para passar por todos os vertices, caso tenha vertices desconexos
        if (verticeColors[i] == 0) { // if para ver se o vertice não foi explorado e está sem cor
            verticeColors[i] = 1;  // Define cor inicial como 1
            int fila[graphSize]; // Cria fila de inteiros com a ordem de exploração dos vertices
            int frente = 0, final = 0;
            fila[final++] = i; // Adiciona o não explorado no final da fila
            
            while (frente != final) { // Verifica se a fila não acabou
                int vertice = fila[frente++]; // Pega o vertice da frente da fila
                
                for (int j = 0; j < graphColSize[vertice]; j++) { // Percorre os vizinhos do vertice atual
                    int vizinho = graph[vertice][j]; // Define o primeiro vizinho a ser marcado
                    
                    if (verticeColors[vizinho] == 0) { // Veriifica se o vizinho ja foi marcado com alguma cor
                        verticeColors[vizinho] = (verticeColors[vertice] == 1) ? 2 : 1; // marca o vizinho com a cor oposta ao vertice
                        fila[final++] = vizinho; // adiciona o vizinho no final da fila
                    } else if (verticeColors[vizinho] == verticeColors[vertice]) { // verifica se o vizinho ja foi marcado e passou a mesma cor do vertice
                        free(verticeColors);
                        return false; // retorna grafico nao bipartido
                    }
                }
            }
        }
    }
    free(verticeColors);
    return true;
}
