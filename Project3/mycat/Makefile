DFLAGS = -g -Wall -o

mycat: mycat.o 
	gcc $(DFLAGS) mycat mycat.c
mycat.o: mycat.c
	gcc $(DFLAGS) mycat.o mycat.c
clean:
	rm -rf mycat *.o

