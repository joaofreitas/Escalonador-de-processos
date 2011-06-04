NAME = escalonaprocessos

CC = gcc

all : submeteProcessos.o escalonaProcessos.o 
	$(CC) -o $(NAME) main.c escalonaProcessos.o submeteProcessos.o -g -Wall -pthread
	make clean

escalonaProcessos.o:
	$(CC) -c escalonaProcessos.c
    
submeteProcessos.o:
	$(CC) -c submeteProcessos.c
		
clean :
	rm -rf *.o
