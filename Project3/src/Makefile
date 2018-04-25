CC=gcc
CFLAGS=-g -Wall

run: shell
	make -C ../myls
	make -C ../mycp
	make -C ../mycat
	./shell
shell: mysh.o
	$(CC) -o shell mysh.o
mysh.o: mysh.c
	$(CC) -c mysh.c
clean: 
	rm -rf shell *.o /commands/my*
