#include "escalonaProcessos.h"

/* Função que cria novos processo, porém paralisando-o antes da execução */
void criaProcessos(processo *p1) {
    int pid, status;
    
    if ((pid = fork()) == 0) {
        status = execv(p1->nome_arquivo, p1->parametros);
        if (status == -1) {
            printf("O programa %s não existe.\n", p1->nome_arquivo);
        }
        exit(1);
    }
    p1->pid = pid;
    time(&p1->inicio_execucao);
    kill(pid, SIGTSTP);
}

void escalonamentoFCFS() {
    int i, pid, status, thread_status;
    processo *p1;
    fila_processos *fila;
    
    pthread_mutex_lock(&fila_procs_mutex);
    fila = fila_procs->fila_union.fila_sem_prior.fila;
    pthread_mutex_unlock(&fila_procs_mutex);
    
    thread_status = pthread_mutex_trylock(&kill_threads_mutex);

    /* Enquanto houver processos, executa o primeiro que está na fila*/
    while (thread_status == EBUSY) {
        if (fila != NULL) {
            printf("Fila nao nula\n");
        } else {
            printf("Fila nula\n");
        }
        if (fila != NULL) {
            p1 = fila->p1;
            pid = p1->pid;
            if (pid == 0) { //Essa condição verifica se o processo foi executado. Caso sim, seu pid certamente será maior que 0
                criaProcessos(p1);
            }
            
            printf("Executando processo %s, pid %d", p1->nome_arquivo, p1->pid);
            kill(pid, SIGCONT);
            waitpid(pid ,NULL, 0);
            printf("\nProcesso executado com sucesso %s\n", p1->nome_arquivo);
            
            /*Protege a área de memória onde fica a fila de processos, para a exclusão do processo executado*/
            pthread_mutex_lock(&fila_procs_mutex);
            fila = fila->prox;
            removerProcessoFila(&(fila_procs->fila_union.fila_sem_prior.fila), pid);
            pthread_mutex_unlock(&fila_procs_mutex);
        } else {
            pthread_mutex_lock(&fila_procs_mutex);
            fila = fila_procs->fila_union.fila_sem_prior.fila;
            pthread_mutex_unlock(&fila_procs_mutex);
        }

        //Após a thread esperar por uma resposta, é necessário verificar se ela deve encerrar ou ainda deve inserir mais processos
        if ((thread_status = pthread_mutex_trylock(&kill_threads_mutex)) != EBUSY) {
            break;
        }
    }
    
    printf("Terminando escalonamento FF\n.");
}

void escalonamentoRR() {
    int pid, status, childPid;
    processo *p1;
    fila_processos *fila;
    time_t fim_execucao;

    pthread_mutex_lock(&fila_procs_mutex);
    fila = fila_procs->fila_union.fila_sem_prior.fila;
    pthread_mutex_unlock(&fila_procs_mutex);

    /* No momento, ele faz diferente de null. Na verdade, ele deve receber o sinal de outra thread para terminar!*/
    while (fila != NULL) {
        p1 = fila->p1;
        pid = p1->pid;
        if (pid == 0) {   //Essa condição verifica se o processo foi executado. Caso sim, seu pid certamente será maior que 0
            criaProcessos(p1);
        }
        //printf("Executando o processo %s de pid %d\n", p1->nome_arquivo, p1->pid);

        kill(pid, SIGCONT);
        sleep(1);
        kill(pid, SIGTSTP);
        
        //Verifica se houve algum processo parado com -1, o parametro WNOHANG serve para não deixar esperando eternamente.
        childPid = waitpid(-1, &status, WNOHANG);
        if (childPid > 0) {
        
            time(&fim_execucao);
            printf("\nProcesso %s concluido com sucesso. Tempo total %ld\n", p1->nome_arquivo, (fim_execucao - p1->inicio_execucao));

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

void executaFilaEscalonamentoPE(fila_processos *fila) {
    int pid, status, childPid;
    processo *p1;
    fila_processos *inicio_fila;
    time_t fim_execucao;

    inicio_fila = fila;
    while (fila != NULL) {
        p1 = fila->p1;
        pid = p1->pid;
        if (pid == 0) {   //Essa condição verifica se o processo foi executado. Caso sim, seu pid certamente será maior que 0
            criaProcessos(p1);
        }

        kill(pid, SIGCONT);
        sleep(1);
        kill(pid, SIGTSTP);
        
        //Verifica se houve algum processo parado com -1, o parametro WNOHANG serve para não deixar esperando eternamente.
        childPid = waitpid(-1, &status, WNOHANG);
        if (childPid > 0) {
        
            time(&fim_execucao);
            printf("\nProcesso %s concluido com sucesso. Tempo total %ld\n", p1->nome_arquivo, (fim_execucao - p1->inicio_execucao));

            pthread_mutex_lock(&fila_procs_mutex);
            fila = removerFila(&inicio_fila);
            pthread_mutex_unlock(&fila_procs_mutex);
        } else {
            pthread_mutex_lock(&fila_procs_mutex);
            fila = mudarOrdemFila(&inicio_fila);
            pthread_mutex_unlock(&fila_procs_mutex);
        }

    }
}

void escalonamentoPE() {
    fila_processos *fila;

    //Executa primeiro a fila de prioridade mais alta
    pthread_mutex_lock(&fila_procs_mutex);
    fila = fila_procs->fila_union.fila_prior.fila0;
    pthread_mutex_unlock(&fila_procs_mutex);
    executaFilaEscalonamentoPE(fila);
    
    pthread_mutex_lock(&fila_procs_mutex);
    fila = fila_procs->fila_union.fila_prior.fila1;
    pthread_mutex_unlock(&fila_procs_mutex);
    executaFilaEscalonamentoPE(fila);
    
    //Executa por ultimo a fila de prioridade mais baixa
    pthread_mutex_lock(&fila_procs_mutex);
    fila = fila_procs->fila_union.fila_prior.fila2;
    pthread_mutex_unlock(&fila_procs_mutex);
    executaFilaEscalonamentoPE(fila);
    
}

void executaFilaEscalonamentoPD(fila_processos *fila){
	int pid, status, childPid;
    processo *p1;
    fila_processos *inicio_fila;
    time_t fim_execucao;
    
    inicio_fila = fila;
    
    OrdenaFilaPD(&fila); //Ordena fila pelas prioridades iniciais
    
    while(fila != NULL){
		p1 = fila->p1;
        pid = p1->pid;
        
        if (pid == 0) {   //Essa condição verifica se o processo foi executado. Caso sim, seu pid certamente será maior que 0
            criaProcessos(p1);
        }
		kill(pid, SIGCONT);
		sleep(p1->prioridade);
		kill(pid, SIGTSTP);
            
        //Verifica se houve algum processo parado com -1, o parametro WNOHANG serve para não deixar esperando eternamente.
        childPid = waitpid(-1, &status, WNOHANG);
        if (childPid > 0) {
            
            time(&fim_execucao);
            printf("\nProcesso =%s concluido com sucesso. Tempo total %ld\n", p1->nome_arquivo, (fim_execucao - p1->inicio_execucao));

            pthread_mutex_lock(&fila_procs_mutex);
            fila = removerFila(&inicio_fila);
            pthread_mutex_unlock(&fila_procs_mutex);
        } else {
            pthread_mutex_lock(&fila_procs_mutex);
			p1->prioridade = (int) ((7/3) * random() * p1->prioridade);
			OrdenaFilaPD(&fila);
            pthread_mutex_unlock(&fila_procs_mutex);
        }


	}
}

void escalonamentoPD(){
    fila_processos *fila;
	
    //Executa primeiro a fila de prioridade mais alta
    pthread_mutex_lock(&fila_procs_mutex);
    fila = fila_procs->fila_union.fila_prior.fila0;
    pthread_mutex_unlock(&fila_procs_mutex);
    executaFilaEscalonamentoPD(fila);
}

void *iniciaEscalonamento(void *politica) {
    char *politica_escalonamento;
    
    politica_escalonamento = (char *) politica;
/*    pthread_t threads[3];
    pthread_attr_t attr;

    
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    pthread_mutex_init(&fila_procs_mutex, NULL);  
*/
    printf("Politica: %s\n", politica_escalonamento);
    if (strcmp(politica_escalonamento, "FF") == 0) {
        escalonamentoFCFS();
    } else if (strcmp(politica_escalonamento, "RR") == 0) {
        escalonamentoRR();
    } else if(strcmp(politica_escalonamento, "PE") == 0) {
        escalonamentoPE();
    } else if(strcmp(politica_escalonamento, "PD") == 0) {
        escalonamentoPD();
    } else {
        printf("Politica não reconhecida\n");
        pthread_exit(NULL);
    }

    printf("Fim da execução!");
    
    pthread_exit(NULL);
}

