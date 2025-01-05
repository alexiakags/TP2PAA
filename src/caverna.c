#include "../include/caverna.h"
#include <limits.h>
#include <math.h>

// Função para alocar a matriz de valores
int** alocarMatriz(int linhas, int colunas) {
    int **matriz = (int**)malloc(linhas * sizeof(int*));
    for (int i = 0; i < linhas; i++) {
        matriz[i] = (int*)malloc(colunas * sizeof(int));
    }
    return matriz;
}

// Função para liberar a matriz de valores
void liberarMatriz(int **matriz, int linhas) {
    for (int i = 0; i < linhas; i++) {
        free(matriz[i]);
    }
    free(matriz);
}

// Função para inicializar a estrutura Caverna
Caverna* inicializarCaverna(int linhas, int colunas, int vida) {
    Caverna *caverna = (Caverna*)malloc(sizeof(Caverna));
    if (!caverna) {
        perror("Erro ao alocar memória para a caverna");
        exit(EXIT_FAILURE);
    }

    caverna->linhas = linhas;
    caverna->colunas = colunas;
    caverna->vida = vida;
    caverna->valores = alocarMatriz(linhas, colunas);
    caverna->inicio.x = -1; // Posição indefinida inicialmente
    caverna->inicio.y = -1;
    caverna->fim.x = -1;    // Posição indefinida inicialmente
    caverna->fim.y = -1;

    return caverna;
}

// Função para liberar memória da estrutura Caverna
void liberarCaverna(Caverna *caverna) {
    liberarMatriz(caverna->valores, caverna->linhas);
    free(caverna);
}

Caverna* carregarArquivo(const char* caminho_arquivo) {
    FILE *arquivo = fopen(caminho_arquivo, "r");
    if (!arquivo) {
        perror("Erro ao abrir o arquivo");
        return NULL;
    }

    // Ler dimensões e pontos iniciais
    int linhas, colunas, vida;
    fscanf(arquivo, "%d %d %d", &linhas, &colunas, &vida);

    // Inicializar a estrutura da caverna
    Caverna *caverna = inicializarCaverna(linhas, colunas, vida);

    // Ler os valores da caverna
    char valor[10];
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            fscanf(arquivo, "%s", valor);

            if (strcmp(valor, "I") == 0) {
                caverna->inicio.x = i;
                caverna->inicio.y = j;
                caverna->valores[i][j] = 0; // Valor da célula inicial é zero
            } else if (strcmp(valor, "F") == 0) {
                caverna->fim.x = i;
                caverna->fim.y = j;
                caverna->valores[i][j] = 0; // Valor da célula final é zero
            } else {
                caverna->valores[i][j] = atoi(valor); // Conversão de string para int
            }
        }
    }

    fclose(arquivo);
    return caverna;
}

void imprimirCaverna(Caverna *caverna) {
    printf("Linhas: %d, Colunas: %d, Vida Inicial: %d\n", caverna->linhas, caverna->colunas, caverna->vida);
    printf("Posicao Inicial: (%d, %d)\n", caverna->inicio.x, caverna->inicio.y);
    printf("Posicao Final: (%d, %d)\n", caverna->fim.x, caverna->fim.y);

    for (int i = 0; i < caverna->linhas; i++) {
        for (int j = 0; j < caverna->colunas; j++) {
            printf("%d ", caverna->valores[i][j]);
        }
        printf("\n");
    }
}


// Inicializar as matrizes dp e parent
ProgramacaoDinamica* inicializarDp(Caverna *caverna) {
    ProgramacaoDinamica *pd = (ProgramacaoDinamica*)malloc(sizeof(ProgramacaoDinamica));
    if (!pd) {
        perror("Erro ao alocar memória para programação dinâmica");
        return NULL;
    }

    pd->dp = (int**)malloc(caverna->linhas * sizeof(int*));
    pd->parent = (Coordenada**)malloc(caverna->linhas * sizeof(Coordenada*));
    for (int i = 0; i < caverna->linhas; i++) {
        pd->dp[i] = (int*)malloc(caverna->colunas * sizeof(int));
        pd->parent[i] = (Coordenada*)malloc(caverna->colunas * sizeof(Coordenada));
        for (int j = 0; j < caverna->colunas; j++) {
            pd->dp[i][j] = INT_MIN; // Inicializar como valor mínimo
            pd->parent[i][j].x = -1;
            pd->parent[i][j].y = -1;
        }
    }

    return pd;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

// Algoritmo principal
void resolverCaverna(Caverna *caverna, const char* arquivo_saida) {
    ProgramacaoDinamica *pd = inicializarDp(caverna);

    // Inicializar ponto inicial
    pd->dp[caverna->inicio.x][caverna->inicio.y] = caverna->vida;
    printf("Ponto inicial: (%d, %d), vida: %d\n", caverna->inicio.x, caverna->inicio.y, pd->dp[caverna->inicio.x][caverna->inicio.y]);

    // Preenchimento da matriz de DP
    for (int i = 0; i < caverna->linhas; i++) {
        for (int j = 0; j < caverna->colunas; j++) {
            
            if (i == caverna->inicio.x && j == caverna->inicio.y) continue;

            int de_cima = (i > 0 && pd->dp[i-1][j] == INT_MIN) ? pd->dp[i-1][j] + caverna->valores[i][j] : INT_MIN;
            int da_esquerda = (j > 0 && pd->dp[i][j-1] == INT_MIN) ? pd->dp[i][j-1] + caverna->valores[i][j] : INT_MIN;
            printf("Atualizando dp[%d][%d]: de_cima=%d, da_esquerda=%d\n", i, j, pd->dp[i][j-1], da_esquerda);

            // Selecionar o melhor valor
            pd->dp[i][j] = max(de_cima, da_esquerda);

            // Atualizar o parent apenas se o valor for maior que INT_MIN
            if (pd->dp[i][j] != INT_MIN) {
                if (de_cima >= da_esquerda) {
                    pd->parent[i][j].x = i-1;
                    pd->parent[i][j].y = j;
                } else {
                    pd->parent[i][j].x = i;
                    pd->parent[i][j].y = j-1;
                }
            }

            // Se a vida cair abaixo de 0, tornar a célula inalcançável
            if (pd->dp[i][j] <= 0) {
                pd->dp[i][j] = INT_MIN;
            }
        }
    }

    // Imprimir matrizes para debug
    printf("Matriz dp:\n");
    for (int i = 0; i < caverna->linhas; i++) {
        for (int j = 0; j < caverna->colunas; j++) {
            printf("%d ", pd->dp[i][j]);
        }
        printf("\n");
    }

    printf("Matriz parent:\n");
    for (int i = 0; i < caverna->linhas; i++) {
        for (int j = 0; j < caverna->colunas; j++) {
            printf("(%d,%d) ", pd->parent[i][j].x, pd->parent[i][j].y);
        }
        printf("\n");
    }

    // Verificar se é possível alcançar o destino
    if (pd->dp[caverna->fim.x][caverna->fim.y] <= 0) {
        FILE *saida = fopen(arquivo_saida, "w");
        fprintf(saida, "impossível\n");
        fclose(saida);
        return;
    }

    // Reconstruir o caminho
    Coordenada atual = caverna->fim;
    Coordenada caminho[1000]; // Suporte para um caminho grande
    int passos = 0;

    while (atual.x != -1 && atual.y != -1) {
        caminho[passos++] = atual;
        atual = pd->parent[atual.x][atual.y];
    }

    // Gravar o caminho no arquivo
    FILE *saida = fopen(arquivo_saida, "w");
    for (int i = passos - 1; i >= 0; i--) {
        fprintf(saida, "%d %d\n", caminho[i].x, caminho[i].y);
    }
    fclose(saida);
}
