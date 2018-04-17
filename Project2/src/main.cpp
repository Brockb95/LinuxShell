#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include "queue_c.h"


void * producer(void *);
void * consumer(void *);

sem_t empty;
sem_t full;
sem_t mutex;

int NEXT_ITEM = 0;

void cleanup(){
	sem_destroy(&mutex);
	sem_destroy(&empty);
	sem_destroy(&full);

}

int main(int argc, char * argv[]){
	if(argc != 4){
		std::cout << "Usage: ./foo <int> sleep <int> producers <int> consumers\n";
		return -1;
	}

	queue_c * q = new queue_c();	

	if((sem_init(&empty, 0, 10) == -1) || (sem_init(&full, 0, 0) == -1) ||
	(sem_init(&mutex, 0, 1) == -1))
		std::cout << "Error in sem_init()";	
	
	pthread_t thread_p[atoi(argv[2])];
	pthread_t thread_c[atoi(argv[3])];
	
	for(int i = 0; i < atoi(argv[2]); i++){
		pthread_create(&thread_p[i], NULL, &producer, (void *) q);
	}
	for(int i = 0; i < atoi(argv[3]); i++){
		pthread_create(&thread_c[i], NULL, &consumer, (void *) q);
	}
	
	sleep(atoi(argv[1]));
	
	atexit(cleanup);
	return 0;
}


// function run by producer threads
void * producer(void * buffer){
	queue_c * q = (queue_c *) buffer;
	while(true){

		sem_wait(&empty);
		sem_wait(&mutex);
		
		std::cout << "item inserted: " << NEXT_ITEM << std::endl;	
		q->insert_item(NEXT_ITEM++);

		sem_post(&mutex);
		sem_post(&full);

	}
}
// function run by consumer threads
void * consumer(void * buffer){
	queue_c * q = (queue_c *) buffer;
	while(true){
		sem_wait(&full);
		sem_wait(&mutex);

		std::cout << "item removed: " << q->remove_item() << std::endl;

		sem_post(&mutex);
		sem_post(&empty);
	}
}
