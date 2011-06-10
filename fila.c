#include "fila.h"

fila_ready *criaFila(char *politica_escalonamento) {
    fila_ready *fila;
    
    fila = malloc(sizeof(fila_ready));
    
    if ((strcmp(politica_escalonamento, "RR") == 0) || ((strcmp(politica_escalonamento, "FF") == 0))){
    
        fila->tipo_fila = FILA_SEM_PRIORIDADES;
        fila->fila_union.fila_sem_prior.fila = NULL;
    } else {
        fila->tipo_fila = FILA_COM_PRIORIDADES;
        fila->fila_union.fila_prior.fila0 = NULL;
        fila->fila_union.fila_prior.fila1 = NULL;
        fila->fila_union.fila_prior.fila2 = NULL;
    }

    return fila;
}

fila_processos * mudarOrdemFila(fila_processos **fila) {
    fila_processos *fila_primeiro_elemento, *fila_ultimo_elemento;
    
    fila_primeiro_elemento = *fila;
    

    if (fila_primeiro_elemento != NULL) {
        fila_ultimo_elemento = *fila;

        if (fila_ultimo_elemento->prox != NULL) {
            while (fila_ultimo_elemento->prox != NULL) {
                fila_ultimo_elemento = fila_ultimo_elemento->prox;
            }
            
            fila_ultimo_elemento->prox = fila_primeiro_elemento;
            *fila = fila_primeiro_elemento->prox;
            fila_primeiro_elemento->prox = NULL;
        }
    }
    
    return *fila;
}

fila_processos * removerFila(fila_processos **fila) {
    fila_processos *elemento_removido;
    
    elemento_removido = *fila;
    if (elemento_removido != NULL) {
        *fila = elemento_removido->prox;
        
        free(elemento_removido->p1);
        free(elemento_removido);
    }
    return *fila;
}

//Função que imprime o processo, NAO VAI ESTAR NO TRABALHO FINAL
void imprimeProcesso(fila_processos *fila) {
    int i;
    processo *p1;
    
    while(fila != NULL) {
        p1 = fila->p1;
        printf("Nome: %s |", p1->nome_arquivo);
        printf("PID: %d |", p1->pid);
        printf("Prioridade: %d |", p1->prioridade);
        printf("Num Params: %d |", p1->num_params);
        printf("Parametros: ");
        for (i=0; i < p1->num_params; i++) {
            printf("%s ", p1->parametros[i]);
        }
        printf("\n");
        fila = fila->prox;
    }
}

//Função que imprime a fila, NAO VAI ESTAR NO TRABALHO FINAL
void imprimeFila(fila_ready *fila) {

    if (fila->tipo_fila == FILA_SEM_PRIORIDADES) {
        printf("FILA:\n");
        imprimeProcesso(fila->fila_union.fila_sem_prior.fila);
    } else {
        printf("FILA 0:\n");
        imprimeProcesso(fila->fila_union.fila_prior.fila0);
        printf("FILA 1:\n");
        imprimeProcesso(fila->fila_union.fila_prior.fila1);
        printf("FILA 2:\n");
        imprimeProcesso(fila->fila_union.fila_prior.fila2);
    }
}
