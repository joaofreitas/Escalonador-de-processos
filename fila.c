#include "fila.h"

int compara(processo *p1, processo *p2){
	if ((*p1).prioridade > (*p2).prioridade){
		return 1;
	}else if ((*p1).prioridade == (*p2).prioridade){
		return 0;
	}else if ((*p1).prioridade < (*p2).prioridade){
		return -1;
	}
}



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
        if (elemento_removido->prox == NULL) {
            *fila = NULL;
        } else {
            *fila = elemento_removido->prox;
        }

        free(elemento_removido);
    }
    return *fila;
}

int numeroElementos(fila_processos *fila) {
    fila_processos *elemento;
    int count;
    
    if (fila == NULL) {
        count = 0;
    } else {
        count = 1;
        elemento = fila;
        while ((elemento = elemento->prox) != NULL) {
            count++;
        }
    }
    return count;
}

void removerProcessoFila(fila_processos **fila, int pid) {
    fila_processos *fila_percorre,*fila_aux;
    
    fila_percorre = *fila;
    

    if (fila_percorre != NULL) {

        if (fila_percorre->p1->pid != pid) {
            while ((fila_percorre->prox != NULL) && (fila_percorre->prox->p1->pid != pid)) {
                fila_percorre = fila_percorre->prox;
            }
            printf("Encontrei o pid %d\n", fila_percorre->p1->pid);
            
            if (fila_percorre->prox != NULL) {
                fila_aux = fila_percorre->prox;
                free(fila_percorre->prox);
                fila_percorre->prox = fila_aux->prox;
            }
        } else {
            printf("Primeiro pid %d", fila_percorre->p1->pid);
            fila_aux = (*fila);
            (*fila) = (*fila)->prox;
//            free(fila_aux);
        }
    }
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

void OrdenaFila(fila_processos **fila){
	fila_processos *f1;
	processo *aux;
	int troca;
	
	f1 = *fila;
	while(f1 != NULL){
		if(f1->prox != NULL){
			if(f1->p1->prioridade < f1->prox->p1->prioridade){
				aux = f1->p1;
				f1 = f1->prox;
				f1->prox->p1 = aux;
				troca = 1; 
			}
		}
		f1 = f1->prox;
	}
}

