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


// Inicializar estrutura de programação dinâmica
ProgramacaoDinamica* inicializarDp(Caverna *caverna) {
    ProgramacaoDinamica *pd = (ProgramacaoDinamica*)malloc(sizeof(ProgramacaoDinamica));
    pd->dp = alocarMatriz(caverna->linhas, caverna->colunas);
    pd->parent = (Coordenada**)malloc(caverna->linhas * sizeof(Coordenada*));
    for (int i = 0; i < caverna->linhas; i++) {
        pd->parent[i] = (Coordenada*)malloc(caverna->colunas * sizeof(Coordenada));
    }

    // Inicializar as matrizes
    for (int i = 0; i < caverna->linhas; i++) {
        for (int j = 0; j < caverna->colunas; j++) {
            pd->dp[i][j] = INT_MIN; // Valor inicial muito baixo
            pd->parent[i][j].x = -1; // Sem predecessor
            pd->parent[i][j].y = -1;
        }
    }

    return pd;
}

// Liberar estrutura de programação dinâmica
void liberarDp(ProgramacaoDinamica *pd, int linhas) {
    liberarMatriz(pd->dp, linhas);
    for (int i = 0; i < linhas; i++) {
        free(pd->parent[i]);
    }
    free(pd->parent);
    free(pd);
}

// Resolver a caverna utilizando programação dinâmica
void resolverCaverna(Caverna *caverna, const char* arquivo_saida) {
    ProgramacaoDinamica *pd = inicializarDp(caverna);

    // Configurar o ponto inicial
    int x_inicial = caverna->inicio.x;
    int y_inicial = caverna->inicio.y;
    pd->dp[x_inicial][y_inicial] = caverna->vida;

    // Preencher a matriz DP
    for (int i = x_inicial; i >= 0; i--) {
        for (int j = y_inicial; j >= 0; j--) {
            if (pd->dp[i][j] <= 0) continue;

            // Movimento para cima
            if (i > 0) {
                int novo_valor = pd->dp[i][j] + caverna->valores[i - 1][j];
                if (novo_valor > pd->dp[i - 1][j]) {
                    pd->dp[i - 1][j] = novo_valor;
                    pd->parent[i - 1][j] = (Coordenada){i, j};
                }
            }

            // Movimento para a esquerda
            if (j > 0) {
                int novo_valor = pd->dp[i][j] + caverna->valores[i][j - 1];
                if (novo_valor > pd->dp[i][j - 1]) {
                    pd->dp[i][j - 1] = novo_valor;
                    pd->parent[i][j - 1] = (Coordenada){i, j};
                }
            }
        }
    }

    FILE *saida = fopen(arquivo_saida, "w");
    if (!saida) {
        perror("Erro ao abrir arquivo de saída");
        liberarDp(pd, caverna->linhas);
        return;
    }

    int x = caverna->fim.x;
    int y = caverna->fim.y;

    if (pd->dp[x][y] <= 0) {
        fprintf(saida, "impossível\n");
    } else {
        // Reconstruir o caminho
        Coordenada caminho[caverna->linhas * caverna->colunas];
        int passos = 0;

        while (x != -1 && y != -1) {
            caminho[passos++] = (Coordenada){x, y};
            Coordenada anterior = pd->parent[x][y];
            x = anterior.x;
            y = anterior.y;
        }

        // Escrever os passos no arquivo
        for (int i = passos - 1; i >= 0; i--) {
            fprintf(saida, "%d %d\n", caminho[i].x, caminho[i].y);
        }
    }

    fclose(saida);
    liberarDp(pd, caverna->linhas);
}
