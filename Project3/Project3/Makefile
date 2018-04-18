CC=gcc
CFLAGS=-g -Wall

run: shell
	./shell
shell: mysh.o #mycat.o
	$(CC) -o shell mysh.o #mycat.o
#mycat.o: mycat.c
#	$(CC) -c mycat.c
mysh.o: mysh.c
	$(CC) -c mysh.c
clean: 
	rm -rf shell *.o
