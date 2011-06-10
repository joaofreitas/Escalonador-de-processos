#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>

pthread_t thread1, thread2;
pthread_mutex_t kill_thread_mutex     = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t operacao_mutex     = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t fazer_operacao = PTHREAD_COND_INITIALIZER;

void *functionCount1() {
    int thread_status = 0;
    
    thread_status = pthread_mutex_trylock(&kill_thread_mutex);
    
    while (thread_status == EBUSY) {
        pthread_mutex_lock(&operacao_mutex);
        pthread_cond_wait(&fazer_operacao, &operacao_mutex);
        
        //Após a thread esperar por uma resposta, é necessário verificar se ela deve encerrar ou ainda deve inserir mais processos
        if ((thread_status = pthread_mutex_trylock(&kill_thread_mutex)) != EBUSY) {
            break;
        }

        pthread_mutex_unlock(&operacao_mutex);
        
    }
    
    pthread_mutex_unlock(&kill_thread_mutex);
    printf("Desligando thread!\n");
    sleep(2);
    printf("Thread Desligada!\n");
    pthread_exit(NULL);
}

void *functionThreadMenu() {
    int termina_escalonador = 0;
    int op;

    pthread_mutex_lock(&kill_thread_mutex);
    pthread_create( &thread2, NULL, functionCount1, NULL);
    
    while (termina_escalonador == 0) {
        printf("Digite a opção\n");
        scanf("%d", &op);
       
        switch(op) {
            case 0:
                printf("%d", op);
                break;
            case 1 :
                pthread_mutex_lock(&operacao_mutex);
                printf("%d. Vou fazer a operação.\n", op);
                pthread_cond_signal(&fazer_operacao);
                pthread_mutex_unlock(&operacao_mutex);
                printf("Operação concluída\n");
                break;
            case 2 :
                printf("%d. Vou cancelar a execução.\n", op);
                pthread_mutex_unlock(&kill_thread_mutex);
                pthread_cond_signal(&fazer_operacao);
                termina_escalonador = 1;
                break;
        }
    }
    
    pthread_join( thread2, NULL);
    pthread_exit(NULL);
}

int main() {
   pthread_create( &thread1, NULL, functionThreadMenu, NULL);

   pthread_join( thread1, NULL);

   printf("Threads terminadas com sucesso!");

   return 0;
}
