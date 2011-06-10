#include "cancelaProcessos.h"

void encerraExecucao(int pid) {
    fila_processos *fila;
    int status;
    
    if (fila_procs->tipo_fila == FILA_SEM_PRIORIDADES ) {
        fila = fila_procs->fila_union.fila_sem_prior.fila;
        while (fila != NULL) {
            if (fila->p1->pid == pid) {
                printf("Pid %d removido\n", pid);
                //Remover aqui
/*                status = kill(pid, SIGTERM);
                if (status < -1) {
                    printf("Erro ao terminar processo com pid = %d.\n", pid);
                }
*/            }
            fila = fila->prox;
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
        
        printf("pid: %d", proccess->value);
        encerraExecucao(proccess->value);
        pthread_mutex_unlock(&fila_procs_mutex);
        
    }
    
    pthread_exit(NULL);
}
