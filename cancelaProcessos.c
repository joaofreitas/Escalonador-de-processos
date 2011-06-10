#include "cancelaProcessos.h"

int percorreFilaRemoveProcesso(fila_processos **fila_prioridades, int pid) {
    fila_processos *fila;
    int status;

    fila = *fila_prioridades;
    
    while (fila != NULL) {
        if (fila->p1->pid == pid) {
            if (pid != 0) {
                printf("Pid %d removido\n", pid);

                fila = fila->prox;
                removerProcessoFila(&(*fila_prioridades), pid);
                status = kill(pid, SIGTERM);
                if (status < -1) {
                    printf("Erro ao terminar processo com pid = %d.\n", pid);
                }
                return 1;
            }
            
        }
        fila = fila->prox;
    }
    
    return 0;
}

void encerraExecucao(int pid) {
    
    if (fila_procs->tipo_fila == FILA_SEM_PRIORIDADES ) {
        percorreFilaRemoveProcesso(&(fila_procs->fila_union.fila_sem_prior.fila), pid);
    } else {
        if (percorreFilaRemoveProcesso(&(fila_procs->fila_union.fila_prior.fila0), pid) == 0) {

            if (percorreFilaRemoveProcesso(&(fila_procs->fila_union.fila_prior.fila1), pid) == 0) {

                percorreFilaRemoveProcesso(&(fila_procs->fila_union.fila_prior.fila2), pid);
            }
        }
        
    }
/*  Fazer para PD  for () {
    
    } */
}

void *cancelarExecucaoProcessos(void *proc_id) {
    int thread_status;
    proccess_id *proccess;
    
    thread_status = pthread_mutex_trylock(&kill_threads_mutex);
    proccess = (proccess_id*) proc_id;
    
    while (thread_status == EBUSY) {
    
        pthread_mutex_lock(&fila_procs_mutex);
        pthread_cond_wait(&fazer_operacao_cancela_proc, &fila_procs_mutex);
        
        //Após a thread esperar por uma resposta, é necessário verificar se ela deve encerrar ou ainda deve inserir mais processos
        if ((thread_status = pthread_mutex_trylock(&kill_threads_mutex)) != EBUSY) { 
            break;
        }
        
        printf("pid: %d\n", proccess->value);
        encerraExecucao(proccess->value);
        pthread_mutex_unlock(&fila_procs_mutex);
        
    }
    
    pthread_exit(NULL);
}
