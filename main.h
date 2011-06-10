#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "escalonaProcessos.h"

pthread_t threads_principais[3];

pthread_mutex_t kill_threads_mutex; //Mutex que controla a morte de todas as threads
pthread_mutex_t fila_procs_mutex; //Mutex que controla a fila de prioridades das threads
pthread_cond_t fazer_operacao_submete_proc = PTHREAD_COND_INITIALIZER;
pthread_cond_t fazer_operacao_cancela_proc = PTHREAD_COND_INITIALIZER;

fila_ready *fila_procs;
