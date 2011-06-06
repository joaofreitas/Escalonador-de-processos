#include "escalonaProcessos.h"

pthread_t threads_auxiliares[3];
pthread_cond_t executa_submissao;
pthread_mutex_t fila_procs_mutex;   //Mutex que controla o acesso a fila de processos
fila_ready *fila_procs;     //Fila de todos os processos;

fila_ready *criaFila(char *politica_escalonamento) {
    fila_ready *fila;
    
    fila = malloc(sizeof(fila_ready));
    
    if ((strcmp(politica_escalonamento, "RR") == 0) || ((strcmp(politica_escalonamento, "FF") == 0))){
    
        fila->tipo_fila = FILA_SEM_PRIORIDADES;
        fila->fila_union.fila_sem_prior.fila = NULL;
    } else {
        fila->tipo_fila = FILA_COM_PRIORIDADES;
        fila->fila_union.fila_prior.fila0 = NULL;
        fila->fila_union.fila_prior.fila1 = NULL;
        fila->fila_union.fila_prior.fila2 = NULL;
    }

    return fila;
}

fila_processos * mudarOrdemFila(fila_processos **fila) {
    fila_processos *fila_primeiro_elemento, *fila_ultimo_elemento;
    
    fila_primeiro_elemento = *fila;
    

    if (fila_primeiro_elemento != NULL) {
        fila_ultimo_elemento = *fila;

        if (fila_ultimo_elemento->prox != NULL) {
            while (fila_ultimo_elemento->prox != NULL) {
                fila_ultimo_elemento = fila_ultimo_elemento->prox;
            }
            
            fila_ultimo_elemento->prox = fila_primeiro_elemento;
            *fila = fila_primeiro_elemento->prox;
            fila_primeiro_elemento->prox = NULL;
        }
    }
    
    return *fila;
}

fila_processos * removerFila(fila_processos **fila) {
    fila_processos *elemento_removido;
    
    elemento_removido = *fila;
    if (elemento_removido != NULL) {
        *fila = elemento_removido->prox;
        
        free(elemento_removido->p1);
        free(elemento_removido);
    }
    return *fila;
}

//Função que imprime o processo, NAO VAI ESTAR NO TRABALHO FINAL
void imprimeProcesso(fila_processos *fila) {
    int i;
    processo *p1;
    
    while(fila != NULL) {
        p1 = fila->p1;
        printf("Nome: %s |", p1->nome_arquivo);
        printf("PID: %d |", p1->pid);
        printf("Prioridade: %d |", p1->prioridade);
        printf("Num Params: %d |", p1->num_params);
        printf("Parametros: ");
        for (i=0; i < p1->num_params; i++) {
            printf("%s ", p1->parametros[i]);
        }
        printf("\n");
        fila = fila->prox;
    }
}

//Função que imprime a fila, NAO VAI ESTAR NO TRABALHO FINAL
void imprimeFila(fila_ready *fila) {

    if (fila->tipo_fila == FILA_SEM_PRIORIDADES) {
        printf("FILA:\n");
        imprimeProcesso(fila->fila_union.fila_sem_prior.fila);
    } else {
        printf("FILA 0:\n");
        imprimeProcesso(fila->fila_union.fila_prior.fila0);
        printf("FILA 1:\n");
        imprimeProcesso(fila->fila_union.fila_prior.fila1);
        printf("FILA 2:\n");
        imprimeProcesso(fila->fila_union.fila_prior.fila2);
    }
}

/* Função que cria novos processo, porém paralisando todos antes da execução */
void criaProcessos(fila_processos **fila) {
    int pid, status;
    processo *p1;
    fila_processos *fila_aux;
    
    pthread_mutex_lock(&fila_procs_mutex);
    fila_aux = *fila;

    while (fila_aux != NULL) {
        p1 = fila_aux->p1;
        if ((pid = fork()) == 0) {
            status = execv(p1->nome_arquivo, p1->parametros);
            if (status == -1) {
                printf("O programa %s não existe.\n", p1->nome_arquivo);
            }
            exit(1);
        }
        p1->pid = pid;
        kill(pid, SIGTSTP);
        fila_aux = fila_aux->prox;
    }
    pthread_mutex_unlock(&fila_procs_mutex);
}

void *executaProcessos() {
    
    if (fila_procs->tipo_fila == FILA_SEM_PRIORIDADES) {
        criaProcessos(&(fila_procs->fila_union.fila_sem_prior.fila));
    } else {
        criaProcessos(&(fila_procs->fila_union.fila_prior.fila0));
        criaProcessos(&(fila_procs->fila_union.fila_prior.fila1));
        criaProcessos(&(fila_procs->fila_union.fila_prior.fila2));
    }
    imprimeFila(fila_procs);
    pthread_exit(NULL);
}

void criaThreads(pthread_t threads[1], char *politica_escalonamento) {
    int thread_status1, thread_status2;
    pthread_attr_t attr;

    /* Initialize and set thread detached attribute */
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    /* Initialize and set thread detached attribute */
    /*pthread_cond_init (&executa_submissao, NULL);*/

    fila_procs = criaFila(politica_escalonamento);

    thread_status1 = pthread_create(&threads[0], &attr, submeterProcessos, (void *) fila_procs);
    pthread_join(threads[0], NULL);

    thread_status2 = pthread_create(&threads[1], NULL, executaProcessos, NULL);
    

    if (thread_status1 || thread_status2){
        printf("ERROR; return code from pthread_create() is %d %d\n", thread_status1, thread_status2);
        exit(-1);
    }

}

void *escalonamentoFCFS() {
   int i, pid, status;
    processo *p1;
    fila_processos *fila;
    
    pthread_mutex_lock(&fila_procs_mutex);
    fila = fila_procs->fila_union.fila_sem_prior.fila;
    pthread_mutex_unlock(&fila_procs_mutex);

    /* Pega o primeiro elemento da fila e executa */
    while (fila != NULL) {
        p1 = fila->p1;
        pid = p1->pid;
        printf("\nExecutando o processo %s\n", p1->nome_arquivo);
        kill(pid, SIGCONT);
        waitpid(pid ,NULL, 0);
        printf("\nProcesso executado com sucesso %s\n", p1->nome_arquivo);
        
        pthread_mutex_lock(&fila_procs_mutex);
        fila = removerFila(&(fila_procs->fila_union.fila_sem_prior.fila));
        pthread_mutex_unlock(&fila_procs_mutex);
    }
    pthread_exit(NULL);
}

void *escalonamentoRR() {
    int pid, status, childPid;
    processo *p1;
    fila_processos *fila;

    pthread_mutex_lock(&fila_procs_mutex);
    fila = fila_procs->fila_union.fila_sem_prior.fila;
    pthread_mutex_unlock(&fila_procs_mutex);

    /* No momento, ele faz diferente de null. Na verdade, ele deve receber o sinal de outra thread para terminar!*/
    while (fila != NULL) {
        p1 = fila->p1;
        pid = p1->pid;
        if (pid > 0) {  //Essa condição verifica se o processo foi executado. Caso sim, seu pid certamente será maior que 0
            //printf("Executando o processo %s de pid %d\n", p1->nome_arquivo, p1->pid);

            kill(pid, SIGCONT);
            sleep(1);
            kill(pid, SIGTSTP);
            
            //Verifica se houve algum processo parado com -1, o parametro WNOHANG serve para não deixar esperando eternamente.
            childPid = waitpid(-1, &status, WNOHANG);
            if (childPid > 0) {
                printf("\nProcesso %s concluido com sucesso\n", p1->nome_arquivo);
                pthread_mutex_lock(&fila_procs_mutex);
                fila = removerFila(&(fila_procs->fila_union.fila_sem_prior.fila));
                pthread_mutex_unlock(&fila_procs_mutex);
            } else {
                pthread_mutex_lock(&fila_procs_mutex);
                fila = mudarOrdemFila(&(fila_procs->fila_union.fila_sem_prior.fila));
                pthread_mutex_unlock(&fila_procs_mutex);
            }

        }
    }
    pthread_exit(NULL);
}


void *start(void *politica) {
    char *politica_escalonamento;
    pthread_t threads[3];
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    pthread_mutex_init(&fila_procs_mutex, NULL);
    
    politica_escalonamento = (char *) politica;
    criaThreads(threads, politica_escalonamento);

    if (strcmp(politica_escalonamento, "FF") == 0) {
        pthread_create(&threads[2], &attr, escalonamentoFCFS, NULL);
    } else if (strcmp(politica_escalonamento, "RR") == 0) {
        pthread_create(&threads[2], &attr, escalonamentoRR, NULL);
    }

    pthread_join(threads[2], NULL);

    printf("Fim da execução!");
    
    pthread_exit(NULL);
}




















