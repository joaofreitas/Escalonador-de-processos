#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>

int main() {
   time_t start, finish;

   time(&start);
    sleep(3);
   /* section to be timed */
   time(&finish);

   printf("Elapsed time: %ld\n",finish - start);
    return 0;
}
