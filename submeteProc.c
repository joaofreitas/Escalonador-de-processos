#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct estrutura_processo {
	char *nome_arquivo;
	char *parametros[10];
	int prioridade;
	int num_params;
} processo;

//TODO Melhorar isso!
void insereParametros(processo *p1, char *parametros) {
    char *copy;
    char *parametro;
    int n = 0;
    
    copy = malloc(sizeof(char));
    parametro = malloc(sizeof(char));
    p1->parametros[n] = malloc(sizeof(char));
    
    strcpy(copy, parametros);
    
    parametro = strtok(copy," ");
    strcpy(p1->parametros[n], parametro);
    printf("%s\n", p1->parametros[n]);
    n = 1;

    while ( (parametro = strtok(NULL," ")) != NULL )
    {
        p1->parametros[n] = malloc(sizeof(char));
        strcpy(p1->parametros[n], parametro);
        printf("%s\n", p1->parametros[n]);
        n++;
    }
    p1->num_params = n;
}

processo *cria_processo(char *arquivo, int prioridade) {
    processo *p1;
    
    p1 = malloc(sizeof(processo));
    p1->nome_arquivo = arquivo;
    p1->prioridade = prioridade;
    p1->num_params = 0;
    return p1;
}

int main() {
    char *nome_processo, *parametros, *lista_parametros;
    int prioridade = 0, qtd_chars = 0;
    FILE *fp;
    processo *p1;
    
    nome_processo = malloc(sizeof(char *));
    parametros = malloc(sizeof(char *)); //TODO Refatorar depois!
    lista_parametros = malloc(sizeof(char *));

    fp = fopen("arquivoProcessos", "r");

    while (!feof(fp)) {
        if (fscanf(fp, "%s %d ", nome_processo, &prioridade) != EOF) {
            p1 = cria_processo(nome_processo, prioridade);

            qtd_chars = fscanf(fp, "%[^\n]", parametros);
            if (qtd_chars > 0) {
                insereParametros(p1, parametros);
            }
        }
    }
    printf("Terminou!");
    
    fclose(fp);
    /*
    
    while ((!feof(fp)) && (proximo_caracter != '\n')) {
                fscanf(fp, "%s", parametros);
                proximo_caracter = getc(fp);
                insereParametro(p1, parametros);
            }
            printf("\n");
    
    
    
    %[^\n]
    char *parametros = "-r -t -s -q";
    processo *p1;
    
    p1 = malloc(sizeof(processo));
    insereParametros(p1, parametros);

    return 0;
     */
   

    return 0;
}
