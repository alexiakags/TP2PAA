#include "../include/saidaArquivo.h"

#define MAX_EXECUCOES 1000

void registrarExecucoes(const char *nome_arquivo, int execucoes_count, int execucoes[][2]) {
    char outputArquivo[100];
    sprintf(outputArquivo, "./output/%s", nome_arquivo);
    FILE *arquivo = fopen(outputArquivo, "w");
    if (arquivo) {
        for (int i = 0; i < execucoes_count; i++) {
            fprintf(arquivo, "%d %.4f\n", execucoes[i][0], execucoes[i][1] / 1000.0);  // Converte tempo para segundos
        }
        fclose(arquivo);
    } else {
        printf("Erro ao abrir o arquivo de saída!\n");
    }
}