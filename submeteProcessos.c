#include "submeteProcessos.h"

int copiaParametro(char **parametros, char *parametro, int n) {
    
    *parametros = malloc(sizeof(char *));
    strcpy(*parametros, parametro);
    n++;
    return n;
}

processo *cria_processo(char *arquivo, int prioridade) {
    processo *p1;
    
    p1 = malloc(sizeof(processo));
    
    p1->nome_arquivo = arquivo;
    p1->prioridade = prioridade;
    p1->num_params = 0;
    
    return p1;
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

void *submeterProcessos(void *fila) {
    char *nome_processo, *parametro, proximo_char;
    int prioridade = 0, n, i;
    FILE *fp;
    fila_prioridades *fila_prior;
    processo *p1;
    
/*    while (thread_executando)
    
    pthread_cond_wait(&executa_submissao, (int *)1);*/
    
    fila_prior = (fila_prioridades *) fila;
    fp = fopen("arquivoProcessos", "r");
    
    nome_processo = malloc(sizeof(char *));
    while ((fscanf(fp, "%s %d", nome_processo, &prioridade)) != EOF) {
        n = 0;
        p1 = cria_processo(nome_processo, prioridade);
        n = copiaParametro(&(p1->parametros[n]), nome_processo, n);

        while ((proximo_char = getc(fp)) != '\n') {
            parametro = malloc(sizeof(char *)); 
            fscanf(fp, "%s", parametro);
            n = copiaParametro(&(p1->parametros[n]), parametro, n);\
        }
        p1->num_params = n;
        p1->parametros[n] = (char *)0;
        insereProcessoFilaPrioridades(fila_prior, p1);

        nome_processo = malloc(sizeof(char *));
    }
    
    fclose(fp);
    pthread_exit(NULL);
}
