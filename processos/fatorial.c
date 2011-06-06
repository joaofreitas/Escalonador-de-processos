#include<stdio.h>
#include <stdlib.h>

#include <unistd.h>
 
int fatorial(unsigned int number) {
    if(number <= 1)
        return 1;
    sleep(1);
    return number * fatorial(number - 1);
}
int main(int argc, char *argv[]) {
    unsigned int x = (unsigned) atoi(argv[1]);
    
    printf("Fatorial de %d e %d", x, fatorial(x));
    exit(1);
}
