#ifndef FILA_H_
#define FILA_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <sys/time.h>
#include <errno.h>

#define FILA_COM_PRIORIDADES 0
#define FILA_SEM_PRIORIDADES 1

typedef struct estrutura_processo {
    int pid;
	time_t inicio_execucao;
	time_t ultima_execucao;
	int prioridade;
	int num_params;
	char *nome_arquivo;
	char *parametros[40];
} processo;

/* Estrutura com fila de processos */
typedef struct estrutura_fila {
	processo *p1;
	struct estrutura_fila *prox;
} fila_processos;

/* Estrutura com fila de processos sem prioridades */
typedef struct estrutura_fila_sem_prioridades {
	fila_processos *fila;     //Processos de alta prioridade
} fila_sem_prioridades;

/* Estrutura com fila de processos com prioridades são apenas três filas normais */
typedef struct estrutura_fila_prioridades {
	fila_processos *fila0;     //Processos de alta prioridade
	fila_processos *fila1;     //Processos de media prioridade
	fila_processos *fila2;     //Processos de baixa prioridade
} fila_prioridades;

typedef struct estrutura_fila_ready {
	int tipo_fila;     //Essa variável irá determinar o tipo da fila a ser usada: Com ou sem prioridades
	union {
	    fila_sem_prioridades fila_sem_prior;
	    fila_prioridades fila_prior;
	} fila_union;
} fila_ready;

fila_ready *criaFila(char *politica_escalonamento);
fila_processos * mudarOrdemFila(fila_processos **fila);
fila_processos * removerFila(fila_processos **fila);
void removerProcessoFila(fila_processos **fila, int pid);

void imprimeFila(fila_ready *fila);
fila_ready *criaFila(char *politica_escalonamento);
fila_processos * mudarOrdemFila(fila_processos **fila);

int compara(processo *x, processo *y);
void OrdenaFilaPD(fila_processos **fila);

#endif
