#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    FILE *fp;
    char proximo_char;
    char *nome_arquivo;
    char *parametros[10];
    int n = 0, i, prioridade;
  
    fp = fopen("../arquivoProcessos", "r");

    if (fp == NULL) {
        printf("Arquivo não aberto\n");
    }
    
    nome_arquivo = malloc(sizeof(char *) * 50);
    while ((fscanf(fp, "%s %d", nome_arquivo, &prioridade)) != EOF) {
        n = 0;
        parametros[n] = malloc(sizeof(char *) * 50);
        strcpy(parametros[n], nome_arquivo);
        n = 1;
        while ((proximo_char = getc(fp)) != '\n') {
            fscanf(fp, "%s", nome_arquivo);
            parametros[n] = malloc(sizeof(char *));
            strcpy(parametros[n], nome_arquivo);
            n++;
        }
        i=0;
        printf("Prioridade %d\n", prioridade);
        while (i < n) {
            printf("%s\n", parametros[i]);
            i++;
        }
    }
    return 0;
}

