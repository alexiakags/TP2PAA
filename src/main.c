#include "../include/caverna.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// Função para gerar um valor aleatório entre dois limites
int gerarValor(int min, int max) {
    return rand() % (max - min + 1) + min;
}

// Função para gerar o arquivo de entrada da caverna
void gerarCaverna(const char* caminho_arquivo, int linhas, int colunas, int vida, int densidadeMonstros, int densidadePocoes) {
    FILE *arquivo = fopen(caminho_arquivo, "w");
    if (!arquivo) {
        perror("Erro ao criar o arquivo de saída");
        return;
    }

    // Escreve as dimensões e os pontos de vida iniciais
    fprintf(arquivo, "%d %d %d\n", linhas, colunas, vida);

    // Gera a matriz da caverna
    for (int i = 0; i < linhas; i++) {
        for (int j = 0; j < colunas; j++) {
            if (i == 0 && j == 0) {
                fprintf(arquivo, "I "); // Posição inicial
            } else if (i == linhas - 1 && j == colunas - 1) {
                fprintf(arquivo, "F "); // Posição final
            } else {
                int chance = gerarValor(1, 100);
                if (chance <= densidadeMonstros) {
                    fprintf(arquivo, "%d ", gerarValor(-50, -10)); // Monstro
                } else if (chance <= densidadeMonstros + densidadePocoes) {
                    fprintf(arquivo, "%d ", gerarValor(10, 50)); // Poção
                } else {
                    fprintf(arquivo, "0 "); // Espaço vazio
                }
            }
        }
        fprintf(arquivo, "\n");
    }

    fclose(arquivo);
    printf("Arquivo gerado com sucesso: %s\n", caminho_arquivo);
}

// Função para exibir o menu e retornar a opção escolhida
int exibirMenu() {
    int opcao;
    printf("=== Menu Principal ===\n");
    printf("1. Gerar caverna\n");
    printf("2. Resolver caverna de um arquivo\n");
    printf("3. Sair\n");
    printf("Escolha uma opção: ");
    scanf("%d", &opcao);
    return opcao;
}

int main() {
    srand(time(NULL)); // Inicializa o gerador de números aleatórios

    Caverna *caverna = NULL;

    while (1) {
        int opcao = exibirMenu();

        if (opcao == 1) {
            // Opção de gerar a caverna
            char caminho_arquivo[150] = "input/";
            char nome_arquivo[100];

            printf("Digite o nome do arquivo a ser gerado (exemplo: caverna_gerada.txt): ");
            scanf("%s", nome_arquivo);
            strcat(caminho_arquivo, nome_arquivo);

            int linhas, colunas, vida, densidadeMonstros, densidadePocoes;

            printf("Digite o número de linhas: ");
            scanf("%d", &linhas);
            printf("Digite o número de colunas: ");
            scanf("%d", &colunas);
            printf("Digite os pontos de vida iniciais: ");
            scanf("%d", &vida);
            printf("Digite a densidade de monstros (0-100%%): ");
            scanf("%d", &densidadeMonstros);
            printf("Digite a densidade de poções (0-100%%): ");
            scanf("%d", &densidadePocoes);

            gerarCaverna(caminho_arquivo, linhas, colunas, vida, densidadeMonstros, densidadePocoes);

            caverna = carregarArquivo(caminho_arquivo);
            imprimirCaverna(caverna);
        } else if (opcao == 2) {
            // Opção de resolver uma caverna de um arquivo
            char nome_arquivo[100];
            char caminho_arquivo[150] = "input/";

            printf("Digite o nome do arquivo a ser resolvido (exemplo: caverna1.txt): ");
            scanf("%s", nome_arquivo);
            strcat(caminho_arquivo, nome_arquivo);

            caverna = carregarArquivo(caminho_arquivo);
            if (caverna) {
                imprimirCaverna(caverna);
            } else {
                printf("Erro ao carregar o arquivo!\n");
            }
        } else if (opcao == 3) {
            // Sair do programa
            printf("Encerrando o programa.\n");
            break;
        } else {
            printf("Opção inválida! Tente novamente.\n");
        }

        if (caverna) {
            resolverCaverna(caverna, "output/resultado.txt");
            liberarCaverna(caverna);
            caverna = NULL;
        }
    }

    return 0;
}
