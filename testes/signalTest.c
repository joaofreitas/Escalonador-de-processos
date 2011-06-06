#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int main (int argc, char *argv[])
{
    int pid = atoi(argv[1]);
    while (kill(pid, SIGCHLD) != -1);
    printf("Processo %d parou!", pid);
    
    return 0;
}
