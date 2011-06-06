#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "escalonaProcessos.h"

int main(int argc, char *argv[]) {
    int rc;
    
    pthread_t thread_principal[1];

    rc = pthread_create(&thread_principal[0], NULL, start, (void *) argv[1]);

    if (rc){
        printf("ERROR; return code from pthread_create() is %d\n", rc);
        exit(-1);
    }
    
    pthread_exit(NULL);
    return 0;
}
