#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "escalonaProcessos.h"

pthread_t threads_principais[3];

pthread_mutex_t kill_threads_mutex; //Mutex que controla a morte de todas as threads
