#include "../include/caverna.h"
#include "../include/saidaArquivo.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define MAX_EXECUCOES 1000

int main() {

    int qtdExecucoes = 0;
    int execucoes_count = 0;
    int execucoes[MAX_EXECUCOES][2];
    int qtdMovimentos = 0;

    const char *nome_arquivo_saida = "saida.txt";

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
            printf("__________________________________________________________\n");
            printf("\n");
            printf("Estrutura da caverna:\n");
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
                printf("__________________________________________________________\n");
                printf("\n");
                printf("Estrutura da caverna:\n");
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
            clock_t start_time = clock();
            resolverCaverna(caverna, "output/resultado.txt");
            clock_t end_time = clock();
            double tempo_execucao = (double)(end_time - start_time) / CLOCKS_PER_SEC;
            qtdExecucoes++;
            if (execucoes_count < MAX_EXECUCOES) {
                        execucoes[execucoes_count][0] = caverna->colunas * caverna->linhas;
                        execucoes[execucoes_count][1] = (int)(tempo_execucao * 1000); 
                        execucoes_count++;
            }
            liberarCaverna(caverna);
            caverna = NULL;
        }
    }
    
    registrarExecucoes(nome_arquivo_saida, execucoes_count, execucoes);
    return 0;
}
