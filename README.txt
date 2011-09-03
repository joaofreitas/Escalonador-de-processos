/*
Universidade de Brasília
Instituto de Ciẽncias Exatas
Departamento de Ciência da Computação

Disciplina: Sistemas Operacionais
Professora: Alba Melo

Implementação de um escalonador de processos em C usando a biblioteca pthreads para os sistemas operacionais baseados em UNIX.

Alunos:

    João Paulo de Freitas Matos		09/47636
    Tércio Cassiano Silva			09/39072
    
Numero de threads: 5 threads

Nomes das threads: submeterProcessos, cancelarExecucaoProcessos, iniciaEscalonamento, 

Numero de processos: 1 processo

Mecanismos(threads e processos):
Locks, Sinais, Variáveis de condição(thread)

Ordem de lançamento:
./escalonaProcessos PR

PR é a prioridade: FF, RR, PE, PD

Compilador:  
gcc 4.5.2
gcc 4.3.3

Sistema Operacional:
Linux 2.6.38-8-generic #42-Ubuntu Si686 i686 i386 GNU/Linux
Linux 2.6.32-32-generic #62-Ubuntu i686 GNU/Linux

Calculo da Prioridade Dinâmica:
O escalonador dobra a prioridade do processo sempre que ele usa todo o tempo de CPU e divide pela metade quando não o usa.

Programa usuário:

Compilação:
Segue um arquivo makefile em anexo.
make all para contruir tudo.
make clean para remover os objetos.

*/
