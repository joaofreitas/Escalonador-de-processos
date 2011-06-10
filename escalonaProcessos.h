#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "submeteProcessos.h"

fila_ready *criaFila(char *politica_escalonamento);

void *start(void *politica);

void *executaProcessos();

void *escalonamentoFCFS();

void *escalonamentoRR();

void *escalonamentoPE();

void *start(void *politica);

pthread_t threads_auxiliares[3];
pthread_cond_t executa_submissao;   //Controla a submissão de processos
extern fila_ready *fila_procs;             //Fila de todos os processos;
extern pthread_mutex_t fila_procs_mutex;    //Mutex que controla o acesso a fila de processos

