#include <stdio.h>
#include <unistd.h>

int main() {
    int pid;
    printf("Pthread test!\n");
    
    pid = fork();
    
    if (pid < 0) {
        printf("Processo filho não foi criado!\n");
    }
    if (pid == 0) {
        printf("Processo filho! pid=%d\n", pid);
    } else {
        printf("Processo pai! pid=%d\n", pid);
    }
    
    char *args[] = {"/bin/ls", "-r", "-t", "-l", (char *) 0 };
    execv("/bin/ls", args);

    
    return 0;
}
