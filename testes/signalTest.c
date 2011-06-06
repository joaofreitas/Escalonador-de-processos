#include <stdio.h>
#include <signal.h>

int main (int argc, char *argv[])
{
    
    if (kill(2760, 0) == -1) {
        printf("Kill deu pau!");
    };
    
    return 0;
}
