DFLAGS = -g -Wall -o

main: mycat.o mysh.o
	gcc $(DFLAGS) main mycat.c mysh.c
mycat.o: mycat.c
	gcc $(DFLAGS) mycat.o mycat.c
mysh.o: mysh.c
	gcc $(DFLAGS) mysh.o mysh.c
clean:
	rm -rf mycat *.o

