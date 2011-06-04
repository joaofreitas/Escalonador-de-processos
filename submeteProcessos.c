#include "submeteProcessos.h"

//TODO Melhorar isso!
void insereParametros(processo *p1, char *nome_programa, char *parametros) {
    char *copy;
    char *parametro;
    int n = 0;
    
    copy = malloc(sizeof(char));
    parametro = malloc(sizeof(char));
    p1->parametros[n] = malloc(sizeof(char));
    
    strcpy(copy, parametros);
    strcpy(p1->parametros[n], nome_programa);

    n = 1;
    
    parametro = strtok(copy," ");
    p1->parametros[n] = malloc(sizeof(char));
    strcpy(p1->parametros[n], parametro);

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
    char *nome_processo, *parametros, *lista_parametros;
    fila_prioridades *fila_prior;
    int prioridade = 0, qtd_chars = 0;
    FILE *fp;
    processo *p1;
    
/*    while (thread_executando)
    
    pthread_cond_wait(&executa_submissao, (int *)1);*/
    
    fila_prior = (fila_prioridades *) fila;
    fp = fopen("arquivoProcessos", "r");

    while (!feof(fp)) {
        nome_processo = malloc(sizeof(char *));
        parametros = malloc(sizeof(char *)); 
        lista_parametros = malloc(sizeof(char *));

        if (fscanf(fp, "%s %d ", nome_processo, &prioridade) != EOF) {
            p1 = cria_processo(nome_processo, prioridade);

            qtd_chars = fscanf(fp, "%[^\n]", parametros);
            if (qtd_chars > 0) {
                insereParametros(p1, nome_processo, parametros);
            }
            
            insereProcessoFilaPrioridades(fila_prior, p1);
        }
    }
    
    //imprimeFila(fila_prior);
    
    fclose(fp);
    pthread_exit(NULL);
}
