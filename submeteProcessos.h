#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

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

extern pthread_cond_t executa_submissao;
extern pthread_cond_t encerra_thread;

void *submeterProcessos(void *fila);
