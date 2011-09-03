#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "submeteProcessos.h"
#include "fila.h"

fila_ready *criaFila(char *politica_escalonamento);

void executaProcessos();

void escalonamentoFCFS();

void escalonamentoRR();

void escalonamentoPE();

void escalonamentoPD();

void *iniciaEscalonamento(void *politica);

extern fila_ready *fila_procs;             //Fila de todos os processos;
extern pthread_mutex_t fila_procs_mutex;    //Mutex que controla o acesso a fila de processos
extern pthread_mutex_t kill_threads_mutex;  //Mutex que controla a morte de todas as threads

