#include "submeteProcessos.h"

int copiaParametro(char **parametros, char *parametro, int n) {
    
    *parametros = malloc(sizeof(char *)*50); // Nome dos parâmetros pode ter no máximo 50 caracteres
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

void insereProcessoFila(fila_ready *fila, processo *proccess) {
    if (fila->tipo_fila == FILA_SEM_PRIORIDADES) { 
        insereProcesso(&(fila->fila_union.fila_sem_prior.fila), proccess);
    } else {
        if (proccess->prioridade == 1) {
            insereProcesso(&(fila->fila_union.fila_prior.fila0), proccess);
        } else {
            if (proccess->prioridade == 2) {
                insereProcesso(&(fila->fila_union.fila_prior.fila1), proccess);
            } else {
                insereProcesso(&(fila->fila_union.fila_prior.fila2), proccess);
            }
        }
    }
}

void *submeterProcessos(void *fila) {
    char *nome_processo, *parametro, proximo_char;
    int prioridade = 0, tipo_fila, n;
    FILE *fp;
    processo *p1;

    fila = (fila_ready *) fila;
    fp = fopen("arquivoProcessos", "r");

    nome_processo = malloc(sizeof(char *)* 50); // Nome do processo pode ter no máximo 50 caracteres
    while ((fscanf(fp, "%s %d", nome_processo, &prioridade)) != EOF) {
        n = 0;
        p1 = cria_processo(nome_processo, prioridade);
        n = copiaParametro(&(p1->parametros[n]), nome_processo, n);

        while ((proximo_char = getc(fp)) != '\n') {
            parametro = malloc(sizeof(char *) * 50); 
            fscanf(fp, "%s", parametro);
            n = copiaParametro(&(p1->parametros[n]), parametro, n);
        }
        p1->num_params = n;
        p1->parametros[n] = (char *)0;
        
        /*
            Esse mutex é responsável por controlar o acesso na fila de processos.
             Preciso fazer esse controle por que a fila vai estar sendo acessada
            pelo escalonador também.
        */
        pthread_mutex_lock(&fila_procs_mutex);
        insereProcessoFila(fila, p1);
        pthread_mutex_unlock(&fila_procs_mutex);

        nome_processo = malloc(sizeof(char *)* 50); // Nome do processo pode ter no máximo 50 caracteres
    }
    
    fclose(fp);
    pthread_exit(NULL);
}
