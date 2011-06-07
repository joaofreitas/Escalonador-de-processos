#include <stdio.h>
#include <unistd.h>

int main(){
int i = 0, pid;

	while(i<3){
	   if(pid!=0){
		   printf("Criei!\n");
		   pid = fork();
	   }
	   i++;
	}
	return 0;
}
       
