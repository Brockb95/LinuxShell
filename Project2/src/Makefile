CC=g++
LIBS=-lpthread


main: main.o queue_c.o
	$(CC) -o main main.o queue_c.o $(LIBS)
main.o: main.cpp
	$(CC) -c main.cpp
queue_c.o: queue_c.cpp
	$(CC) -c queue_c.cpp
clean:
	rm -rf main *.o
