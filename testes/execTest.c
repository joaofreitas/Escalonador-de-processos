#include <stdio.h>
#include <unistd.h>

int main() {
    printf("Pthread test!\n");
    
    char *args[] = {"/bin/ls", "-r", "-t", "-l", (char *) 0 };
    execv("/bin/ls", args);

    
    return 0;
}
