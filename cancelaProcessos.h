#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <errno.h>
#include "submeteProcessos.h"
#include "fila.h"

extern pthread_cond_t fazer_operacao_cancela_proc;
extern pthread_mutex_t fila_procs_mutex; //Mutex controlará a fila de processos
extern pthread_mutex_t kill_threads_mutex; //Mutex que controla a morte de todas as threads
extern fila_ready *fila_procs;  //Fila de todos os processos;

/* Estrutura com fila de processos sem prioridades */
typedef struct estrutura_pid {
	int value;
} proccess_id;

extern proccess_id *proc_id;
void *cancelarExecucaoProcessos(void *proccess_id);
