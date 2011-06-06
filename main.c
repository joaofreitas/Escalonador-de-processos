#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "escalonaProcessos.h"

int main(int argc, char *argv[]) {
    int rc;
    
    pthread_t thread_principal[1];
    
    if ((argc != 2)) {
        printf("Por favor, execute novamente informando a forma do escalonamento:\n");
        printf("FF\tFirst Come, First Served\n");
        printf("RR\tRound Robin\n");
        exit(0);
    }

    rc = pthread_create(&thread_principal[0], NULL, start, (void *) argv[1]);

    if (rc){
        printf("ERROR; return code from pthread_create() is %d\n", rc);
        exit(-1);
    }
    
    pthread_exit(NULL);
    return 0;
}
