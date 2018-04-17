#ifndef QUEUE_H_
#define QUEUE_H_ "queue_c.h"

class queue_c{
	private:
	int front, rear;
	int * buffer;
	static const int BUFFER_SIZE = 10;
	public:
	
	queue_c();

	int insert_item(int);
	int remove_item();
	void print();
	bool isFull();
	bool isEmpty();
};

#endif
