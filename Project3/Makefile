CC=gcc
CFLAGS=-g -Wall

run: shell
	./shell
shell: mysh.o
	$(CC) -o shell mysh.o
mysh.o: mysh.c
	$(CC) -c mysh.c
clean: 
	rm -rf shell *.o
