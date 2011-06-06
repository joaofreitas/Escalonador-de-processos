#include <stdio.h>
#include <stdlib.h>

unsigned int calls;

unsigned int naive_ackermann(unsigned int m, unsigned int n) {
    calls++;
    if (m == 0)
        return n + 1;
    else if (n == 0)
        return naive_ackermann(m - 1, 1);
    else
        return naive_ackermann(m - 1, naive_ackermann(m, n - 1));
}

int main(int argc, char *argv[]) {
    calls = 0;
    unsigned int value = naive_ackermann(( unsigned) atoi(argv[1]), (unsigned) atoi(argv[2]));
    printf("Valor: %u %u", value, calls);
    return 0;
}
