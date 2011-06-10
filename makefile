NAME = escalonaprocessos

CC = gcc

all : fila.o submeteProcessos.o escalonaProcessos.o cancelaProcessos.o
	$(CC) -o $(NAME) main.c fila.o escalonaProcessos.o submeteProcessos.o cancelaProcessos.o -g -Wall -pthread
	make clean

fila.o:
	$(CC) -c fila.c

escalonaProcessos.o:
	$(CC) -c escalonaProcessos.c

submeteProcessos.o:
	$(CC) -c submeteProcessos.c

cancelaProcessos.o:
	$(CC) -c cancelaProcessos.c


clean :
	rm -rf *.o
