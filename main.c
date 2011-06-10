#include "main.h"

void *menuPrincipal() {
    int termina_escalonador = 0;
    int op;

    pthread_mutex_lock(&kill_threads_mutex);
    
    while (termina_escalonador == 0) {
        printf("Digite a opção desejada\n");
        printf("1. Carregar arquivos de processos a serem escalonados.\n");
        printf("2. Parar um processo.\n");
        printf("3. Encerrar o escalonador\n");
        scanf("%d", &op);
       
        switch(op) {
            case 1:
                pthread_mutex_lock(&fila_procs_mutex);
                printf("Vou carregar mais processos.\n");
                pthread_cond_signal(&fazer_operacao_submete_proc);
                pthread_mutex_unlock(&fila_procs_mutex);
                printf("Operação concluída\n");
                break;
            case 2 :
                pthread_mutex_lock(&fila_procs_mutex);
                printf("Vou parar um processo.\n");
                pthread_cond_signal(&fazer_operacao_cancela_proc);
                pthread_mutex_unlock(&fila_procs_mutex);
                printf("Operação concluída\n");
                break;
            case 3 :
                printf("Vou cancelar a execução do escalonador.\n");
                pthread_mutex_unlock(&kill_threads_mutex);
                
                //Preciso alertar minhas threads, embora não vão realizar essas ações
                pthread_cond_signal(&fazer_operacao_cancela_proc);
                pthread_cond_signal(&fazer_operacao_submete_proc);
                
                termina_escalonador = 1;
                break;
        }
    }
    
    pthread_exit(NULL);
}

// Encapsulei a função de criação de thread por que aqui ela já faz o teste se deu certo ou não a criação
void criarThread(int thread_id, pthread_attr_t attr, void *(*rotina_inicio)(), void * parametros) {
    int thread_status;
    
    thread_status = pthread_create(&threads_principais[thread_id], &attr, rotina_inicio, parametros);

    if (thread_status) {
        printf("ERROR; return code from pthread_create() is %d\n", thread_status);
        exit(-1);
    }
}

void criaThreadsPrincipais(char *politica_escalonamento) {
    pthread_attr_t attr;

    /* Initialize and set thread detached attribute */
    pthread_attr_init(&attr);
//    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    
    criarThread(0, attr, menuPrincipal, NULL);

    fila_procs = criaFila(politica_escalonamento);    
    criarThread(1, attr, submeterProcessos, (void *) fila_procs);
    
//    pthread_join(threads_principais[0], NULL);
//    thread_status = pthread_create(&threads_principais[1], NULL, executaProcessos, NULL);
    

    
//    pthread_create(&threads_principais[0], NULL, start, (void *) politica_escalonamento);

}

int main(int argc, char *argv[]) {
    
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

    pthread_exit(NULL);
    return 0;
}
