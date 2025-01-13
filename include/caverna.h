#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int x; // Linha
    int y; // Coluna
} Coordenada;

typedef struct {
    int linhas;       
    int colunas;    
    int vida;
    int **valores;    // Matriz com os valores da caverna
    Coordenada inicio; 
    Coordenada fim;   
} Caverna;

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
int gerarValor(int min, int max);
void gerarCaverna(const char* caminho_arquivo, int linhas, int colunas, int vida, int densidadeMonstros, int densidadePocoes);
int exibirMenu();