#include "queue_c.h"
#include <iostream>


// circle queue initializer
queue_c::queue_c(){
	front = rear = -1;
	buffer = new int[BUFFER_SIZE];
}

/* If the buffer is not full; add 'x' to the end of the queue
   returns 1 on success and 0 if the buffer is full */
int queue_c::insert_item(int x){
	if(isFull())
		return 0;
	if(front == -1)
		front = rear = 0;
	else if(rear == BUFFER_SIZE - 1)
		rear = 0;
	else
		rear++;
	buffer[rear] = x;

	return 1;	
}

/* Remove the element from the front of the queue,and return 
   its value. If the buffer is empty returns -1 */
int queue_c::remove_item(){
	if(front == -1)
		return -1;
	int x = buffer[front];
	if(front == rear)
		front = rear = -1;
	else if(front == BUFFER_SIZE - 1)
		front = 0;
	else
		front++;

	return x;
}

/* Check if the buffer is full
   returns true if buffer is full, and false if buffer is not full */
bool queue_c::isFull(){
	if((rear == front - 1) || 
	   (front == 0 && rear == BUFFER_SIZE - 1))
		return true;
	else 
		return false; 
}

/* Check if buffer is empty, return true if empty false if not */
bool queue_c::isEmpty(){
	if(front == -1)
		return true;
	else 
		return false;
}

// print the queue in order from front to rear
void queue_c::print(){
	int f = front, r = rear;
	if(f <= r)
		while (f <= r){
			std::cout << buffer[f] << " ";
			f++;
		}
	else
	{
		for (int i = front; i < BUFFER_SIZE; i++)
			std::cout << buffer[i] << " ";
		for (int i = 0; i <= r; i++)
			std::cout << buffer[i] << " ";
	}
	std::cout << std::endl;
}
