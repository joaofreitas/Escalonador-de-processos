#ifndef SUBMETEPROC_H_
#define SUBMETEPROC_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <sys/time.h>
#include <errno.h>
#include <time.h>
#include "fila.h"

extern pthread_cond_t fazer_operacao_submete_proc;
extern pthread_mutex_t fila_procs_mutex; //Mutex controlará a fila de processos
extern pthread_mutex_t kill_threads_mutex; //Mutex que controla a morte de todas as threads

void *submeterProcessos(void *fila);

#endif
