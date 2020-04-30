/*----------------------------------------------------
 * queue.c  -  enqueue and dequeue function for Queue 
 *----------------------------------------------------
 * Author : Maulik Gohil & Neel Kalariya
 * Change log: 
 *		Date Feb 28, 2020 > Created this file with reference to online source and made few changes as per	
 *							our system requirements like queue size will be 10 at max in our case.
 *		Date Mar 01, 2020 > Implemted checks whether Queue is full or empty and printout the error message.
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include "proc.h"

/* function to enqueue the current thread-id in the waiting queue */
void enqueue(struct Queue* queue, int id) 
{ 
    queue->rear = (queue->rear + 1)%NPROC; 
    queue->array[queue->rear] = id; 
    queue->size = queue->size + 1;  
} 
/* function to dequeue the thread-id from the waiting queue  */
int dequeue(struct Queue* queue) 
{ 
    int dqid = queue->array[queue->front]; 
    queue->front = (queue->front + 1)%NPROC; 
    queue->size = queue->size - 1; 
    return dqid; 
} 
