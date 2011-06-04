#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct estrutura_processo {
	char *nome_arquivo;
	char *parametros[10];
	int prioridade;
	int num_params;
} processo;

typedef struct estrutura_fila {
	processo *p1;
	struct estrutura_fila *prox;
} fila_processos;

typedef struct estrutura_fila_prioridades {
	fila_processos *fila1;     //Processo de prioridade 1
	fila_processos *fila2;     //Processo de prioridade 2
	fila_processos *fila3;     //Processo de prioridade 3
} fila_prioridades;

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
    n = 1;

    while ( (parametro = strtok(NULL," ")) != NULL )
    {
        p1->parametros[n] = malloc(sizeof(char));
        strcpy(p1->parametros[n], parametro);
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

fila_prioridades *criaFila() {
    fila_prioridades *fila;
    
    fila = malloc(sizeof(fila_prioridades));
    
    fila->fila1 = NULL;
    fila->fila2 = NULL;
    fila->fila3 = NULL;
    
    return fila;
    
}

void insereProcesso(fila_processos **fila, processo *proccess) {
    fila_processos *filaAux, *proximo_elemento;

    if (*fila == NULL) {
        filaAux = malloc(sizeof(fila_processos));
        filaAux->p1 = proccess;
        filaAux->prox = NULL;
        *fila = filaAux;
    } else {
        filaAux = *fila;
        while (filaAux->prox != NULL) {
            filaAux = filaAux->prox;
        }
        proximo_elemento = malloc(sizeof(fila_processos));
        proximo_elemento->p1 = proccess;
        proximo_elemento->prox = NULL;
        filaAux->prox = proximo_elemento;
    }
}

void insereProcessoFilaPrioridades(fila_prioridades *fila, processo *proccess) {
    if (proccess->prioridade == 1) {
        insereProcesso(&(fila->fila1), proccess);
    } else {
        if (proccess->prioridade == 2) {
            insereProcesso(&(fila->fila2), proccess);
        } else {
            insereProcesso(&(fila->fila3), proccess);
        }
    }
}

//Função que imprime o processo, NAO VAI ESTAR NO TRABALHO FINAL
void imprimeProcesso(fila_processos *fila) {
    processo *p1;
    
    while(fila != NULL) {
        p1 = fila->p1;
        printf("Nome: %s |", p1->nome_arquivo);
        printf("Prioridade: %d |", p1->prioridade);
        printf("Num Params: %d |", p1->num_params);
        printf("Primeiro Param: %s\n", p1->parametros[0]);
        fila = fila->prox;
    }
}

//Função que imprime a fila, NAO VAI ESTAR NO TRABALHO FINAL
void imprimeFila(fila_prioridades *fila_prior) {
    
    printf("FILA 1:\n");
    imprimeProcesso(fila_prior->fila1);
    printf("FILA 2:\n");
    imprimeProcesso(fila_prior->fila2);
    printf("FILA 3:\n");
    imprimeProcesso(fila_prior->fila3);
}

int main() {
    char *nome_processo, *parametros, *lista_parametros;
    fila_prioridades *fila_prior;
    int prioridade = 0, qtd_chars = 0;
    FILE *fp;
    processo *p1;
    
    fila_prior = criaFila();

    fp = fopen("arquivoProcessos", "r");

    while (!feof(fp)) {
        nome_processo = malloc(sizeof(char *));
        parametros = malloc(sizeof(char *)); 
        lista_parametros = malloc(sizeof(char *));

        if (fscanf(fp, "%s %d ", nome_processo, &prioridade) != EOF) {
            p1 = cria_processo(nome_processo, prioridade);

            qtd_chars = fscanf(fp, "%[^\n]", parametros);
            if (qtd_chars > 0) {
                insereParametros(p1, parametros);
            }
            
            insereProcessoFilaPrioridades(fila_prior, p1);
        }
    }
    
    imprimeFila(fila_prior);
    fclose(fp);

    return 0;
}
