#include "escalonaProcessos.h"

#define FF 1
#define RR 2
#define PE 3
#define PD 4
#define escalonamento 1

pthread_t threads_auxiliares[3];
pthread_cond_t executa_submissao;
fila_prioridades *fila_prior;

fila_prioridades *criaFila() {
    fila_prioridades *fila;
    
    fila = malloc(sizeof(fila_prioridades));
    
    fila->fila1 = NULL;
    fila->fila2 = NULL;
    fila->fila3 = NULL;
    
    return fila;
}


//Função que imprime o processo, NAO VAI ESTAR NO TRABALHO FINAL
void imprimeProcesso(fila_processos *fila) {
    processo *p1;
    
    while(fila != NULL) {
        p1 = fila->p1;
        printf("Nome: %s |", p1->nome_arquivo);
        printf("Prioridade: %d |", p1->prioridade);
        printf("Num Params: %d |", p1->num_params);
        printf("Primeiro Param: %s\n", p1->parametros[0]);
        fila = fila->prox;
    }
}

//Função que imprime a fila, NAO VAI ESTAR NO TRABALHO FINAL
void imprimeFila(fila_prioridades *fila_prior) {
    
    printf("FILA 1:\n");
    imprimeProcesso(fila_prior->fila1);
    printf("FILA 2:\n");
    imprimeProcesso(fila_prior->fila2);
    printf("FILA 3:\n");
    imprimeProcesso(fila_prior->fila3);
}

void criaThreads(pthread_t threads[1]) {
    int rc;
    pthread_attr_t attr;

    /* Initialize and set thread detached attribute */
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    
    /* Initialize and set thread detached attribute */
    /*pthread_cond_init (&executa_submissao, NULL);*/
   
    fila_prior = criaFila();
    rc = pthread_create(&threads[1], &attr, submeterProcessos, (void *) fila_prior);
    pthread_join(threads[1], NULL);
    imprimeFila(fila_prior);

    if (rc){
        printf("ERROR; return code from pthread_create() is %d\n", rc);
        exit(-1);
    }
}

void executaProcessos() {
    int i, pid, status;
    processo *p1;
    fila_processos *fila;
    
    fila = fila_prior->fila1;
    while (fila != NULL) {
        if ((pid = fork()) == 0) {
            p1 = fila->p1;
            status = execv(p1->nome_arquivo, p1->parametros);
            if (status == -1) {
                printf("O programa %s não existe.\n", p1->nome_arquivo);
                exit(-1);
            }
            exit(0);
            break;
        }
        fila = fila->prox;
    }
}

void *start() {
    pthread_t threads[1];
    printf("Criei!\n");

    criaThreads(threads);
    executaProcessos();
    
    printf("Fim da execução!");
    
    pthread_exit(NULL);
}




















