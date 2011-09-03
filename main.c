#include "main.h"

void *menuPrincipal() {
    int op;
    int termina_escalonador = 0;

    while (termina_escalonador == 0) {
        printf("Digite a opção desejada\n");
        printf("1. Carregar arquivos de processos a serem escalonados.\n");
        printf("2. Parar um processo.\n");
        printf("3. Encerrar o escalonador.\n");
        printf("4. Imprimir Fila Atual.\n");
        scanf("%d", &op);
       
        switch(op) {
            case 1:
                pthread_mutex_lock(&fila_procs_mutex);
                pthread_cond_signal(&fazer_operacao_submete_proc);
                pthread_mutex_unlock(&fila_procs_mutex);
                break;
            case 2 :
                pthread_mutex_lock(&fila_procs_mutex);
                printf("Digite um pid: ");
                scanf("%d", &proc_id->value);
                pthread_cond_signal(&fazer_operacao_cancela_proc);
                pthread_mutex_unlock(&fila_procs_mutex);
                printf("Operação concluída\n");
                break;
            case 3 :
                pthread_mutex_unlock(&kill_threads_mutex);
                printf("\nVou cancelar a execução do escalonador.\n");
                
                //Preciso alertar minhas threads, embora não vão realizar essas ações
                pthread_cond_signal(&fazer_operacao_cancela_proc);
                pthread_cond_signal(&fazer_operacao_submete_proc);
                
                termina_escalonador = 1;
                break;
            case 4 :
                imprimeFila(fila_procs);
                break;
        }
    }
    printf("Encerrando thread do menu\n");
    pthread_exit(NULL);
}

// Encapsulada a função de criação de thread por que aqui ela já faz o teste se deu certo ou não a criação
void criarThread(int thread_id, pthread_attr_t attr, void *(*rotina_inicio)(), void * parametros) {
    int thread_status;
    
    thread_status = pthread_create(&threads_principais[thread_id], &attr, rotina_inicio, parametros);

    if (thread_status) {
        printf("ERROR; return code from pthread_create() is %d\n", thread_status);
        exit(-1);
    }
}

void iniciarMutexes() {
    pthread_mutex_init(&fila_procs_mutex, NULL);
    pthread_mutex_init(&kill_threads_mutex, NULL);
}

void criaThreadsPrincipais(char *politica_escalonamento) {
    pthread_attr_t attr;
    
    
    iniciarMutexes();
    //Esse semáforo vai controlar o encerramento de todas as threads. Por isso, deve iniciar antes de todo o código.
    pthread_mutex_lock(&kill_threads_mutex);

    pthread_attr_init(&attr);
//    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    criarThread(0, attr, menuPrincipal, NULL);

    fila_procs = criaFila(politica_escalonamento);
    pthread_attr_init(&attr);
    criarThread(1, attr, submeterProcessos, (void *) fila_procs);
    
    pthread_attr_init(&attr);
    proc_id = malloc(sizeof(proccess_id));
    proc_id->value = 0;
    criarThread(2, attr, cancelarExecucaoProcessos, (void *) proc_id);
    
    criarThread(3, attr, iniciaEscalonamento, (void *) politica_escalonamento);
    
    
    
//    pthread_join(threads_principais[0], NULL);

}

//Função que imprime o processo, NAO VAI ESTAR NO TRABALHO FINAL
void imprimeProcessoDetalhado(fila_processos *fila) {
    int i;
    processo *p1;
    
    while(fila != NULL) {
        p1 = fila->p1;
        if (p1->pid == 0) {
            printf("O processo abaixo não terminou a execução\n");
        }
            
        fila = fila->prox;
        printf("Nome: %s |", p1->nome_arquivo);
        printf("\t\tPID: %d |", p1->pid);
        printf("\t\tPrioridade: %d |", p1->prioridade);
        printf("\t\tNum Params: %d |", p1->num_params);
        printf("\t\tParametros: ");
        for (i=0; i < p1->num_params; i++) {
            printf("%s ", p1->parametros[i]);
        }
        printf("\n");
        fila = fila->prox;
    }
}

//Função que imprime a fila, NAO VAI ESTAR NO TRABALHO FINAL
void relatorio(fila_ready *fila) {
    
    printf("Nome:|");
    printf("\tPID:|");
    printf("\tPrioridade:|");
    printf("\tNum Params:|");
    printf("\tParametros: ");
    if (fila->tipo_fila == FILA_SEM_PRIORIDADES) {
        printf("FILA:\n");
         imprimeProcessoDetalhado(fila->fila_union.fila_sem_prior.fila);
    } else {
        printf("FILA 0:\n");
         imprimeProcessoDetalhado(fila->fila_union.fila_prior.fila0);
        printf("FILA 1:\n");
         imprimeProcessoDetalhado(fila->fila_union.fila_prior.fila1);
        printf("FILA 2:\n");
         imprimeProcessoDetalhado(fila->fila_union.fila_prior.fila2);
    }
}

int main(int argc, char *argv[]) {
    void * status;
    
    if ((argc != 2)) {
        printf("Por favor, execute novamente informando o tipo do escalonamento:\n\n");
        printf("FORMA DE EXECUÇÃO:\n./escalonaprocessos <tipo escalonamento>\n\n");
        printf("TIPOS DE ESCALONAMENTO:\n");
        printf("FF\tFirst Come, First Served\n");
        printf("RR\tRound Robin\n");
        printf("PE\tPrioridade Estática\n");
        printf("PD\tPrioridade Dinâmica\n");
        exit(0);
    }
    
    criaThreadsPrincipais(argv[1]);
    pthread_join(threads_principais[0], &status);
    relatorio(fila_procs);    
    return 0;
}
