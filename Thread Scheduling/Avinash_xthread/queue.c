/*
Name: Avinash Kumar, Shivam Joshi
Code Description: A Queue has be defined in proc.h with tabs as elements is an array to store the items, size to get how many elements are present, capacity is the final length of queue, front to get the first value from the queue, rear to get the last value of the queue. createQueue() will be called once for each queue where we will initialized and allocate location for the elements to store and initialized all the other variables. Dequeue() will check the queue is empty or not, if not then it will return the first popped element. Enqueue() will check the queue is full or not, if not then it will insert an element into the queue.
Change Log:	2/24/2020: Implemented the createQueue function
		2/25/2020: Implemented the Enenque and Dequeue function
		2/26/2020: Debugging enqueue and dequeue 
*/
#include <proc.h>

void createQueue(Queue *Q,int maxElements)
{
        // Initialise queue's properties
        Q->elements = (int *)malloc(sizeof(int)*maxElements); //elements is an array which is allocated dynamically of size the the total number of threads
	Q->size = 0;
        Q->capacity = maxElements;
        Q->front = 0;
        Q->rear = -1;
}
void Dequeue(Queue *Q,int *jid)
{
        //If Queue size is zero then it is empty. So we cannot pop
	int data;
	if(Q->size==0)
        {
                printf("Queue is Empty\n");
        }
        // Removing an element is equivalent to incrementing index of front by one 
        else
        {
                Q->size--;
		data=Q->elements[Q->front];	//data variable will have the poped value from the front which will be return
                Q->front++;

                //As we fill elements in circular fashion 
                if(Q->front==Q->capacity)
                {
                        Q->front=0;
                }
        }
        *jid=data;
}

void Enqueue(Queue *Q,int element)
{
        // If the Queue is full, we cannot push an element into it as there is no space for it.
	if(Q->size == Q->capacity)
        {
                printf("Queue is Full\n");
        }
        else
        {
                Q->size++;
                Q->rear = Q->rear + 1;
		Q->elements[Q->rear]=element;
                // As we fill the queue in circular fashion
                if(Q->rear == Q->capacity)
                {
                        Q->rear = 0;
                }
                // Insert the element in its rear side 
        }
}
