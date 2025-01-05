#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura para armazenar coordenadas (usada para rastrear caminhos)
typedef struct {
    int x; // Linha
    int y; // Coluna
} Coordenada;

// Estrutura para representar o mapa da caverna
typedef struct {
    int linhas;       // Número de linhas da caverna
    int colunas;      // Número de colunas da caverna
    int vida; // Pontos de vida iniciais
    int **valores;    // Matriz com os valores da caverna
    Coordenada inicio;  // Coordenada da posição inicial (I)
    Coordenada fim;     // Coordenada da saída (F)
} Caverna;

// Estrutura para armazenar resultados da programação dinâmica
typedef struct {
    int **dp;         // Matriz com os valores máximos de pontos de vida
    Coordenada **parent; // Matriz para rastrear os predecessores
} ProgramacaoDinamica;

int** alocarMatriz(int linhas, int colunas);
void liberarMatriz(int **matriz, int linhas);
Caverna* inicializarCaverna(int linhas, int colunas, int vida);
void liberarCaverna(Caverna *caverna);
Caverna* carregarArquivo(const char* caminho_arquivo);
void imprimirCaverna(Caverna *caverna);
void resolverCaverna(Caverna *caverna, const char* arquivo_saida);
ProgramacaoDinamica* inicializarDp(Caverna *caverna);
int max(int a, int b);
void liberarDp(ProgramacaoDinamica *pd, int linhas);