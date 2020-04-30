/*    proc.h    */
#include <stdio.h>
#include <stdlib.h>

typedef int WORD;

#define PNREGS 5

#define NPROC 10

#define SP     0

/* state */
#define XFREE    0
#define XREADY   1
#define XRUN     2

/*
 * Defined new two states as written below XWAIT & XJOIN
 * As every thread will first becomes ZOMBIE and then If some thread Joins(XWAIT) then It can be FREE
 */ 
#define XWAIT    3
#define XZOMBIE  4

struct Queue  {
	int  front; 	 	// front - pointer to the front of the Queue from where Dequeue happens
	int  rear;		// Rear - pointer to the end of the Queue from where Enqueue happens
	int  size;		// Size - for storing the current size of the Queue
    	int  array[NPROC];  	// Array to store the elements
};

struct xentry  {
       	int  xid;
       	WORD xregs[PNREGS];    /* save SP */
       	WORD xbase;
       	WORD xlimit;
       	int  xstate;
	struct Queue que;	// pointer to the Queue structure
       	int  status;		// for storing the current state of own
	int  jstatus;		// for storing the final state(final word) of the a thread with this(current) thread JOINs
};


#define STKSIZE 8192 

extern struct xentry xtab[];

extern int currxid;

