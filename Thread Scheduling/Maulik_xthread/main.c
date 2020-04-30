/*
 * Here we have made few changes to initialize the queue variables.
 * Author : Maulik Gohil & Neel Kalariya
 * Change log: 
 *		Date Feb 26, 2020 > Edited this file and implemted handler for SIGALRM Signal
 *						  > Declared and initialization of signal.
 *			 			  > Added xtab[currxid].xstate = XREADY; (came to know due to incorrect output)
 *		Date Feb 28, 2020 > declared an array of Queue of size 10.
 *						  > Defined a temporary poiter of Queue to store base address of structure
 *						  > Initialized all the variables for each queue.
 *
*/

#include <stdio.h>
#include <proc.h>
#include <signal.h>
#include <unistd.h> 

extern void xmain();

struct xentry xtab[10]; 
struct Queue xque[10]; 				//Array of structure Queue which has been defined proc.h
int currxid = 0; 

/* Here handler has been defined for SIGINT signal to make this whole system preemtive, 
every 0.02 sec this will be executed */
void handler(){
   	sigset_t set;
   	sigemptyset(&set);
	sigaddset(&set,SIGALRM);
	sigprocmask(SIG_UNBLOCK, &set, NULL);
	xtab[currxid].xstate = XREADY;	// Before the context switch, the state of running thread needs to be changed
	resched();   
}

void main(int argc, char *argv[])
{
   register struct xentry *xptr;
   register struct Queue *qptr; 	// A poiter to Queue for further use
   struct xentry m;
   int i;
   int xidxmain;

   for(i=0 ; i < NPROC; i++){
      xptr = &xtab[i];
      xptr->xid = i;
      xptr->xlimit =  (WORD) malloc(STKSIZE);
      xptr->xbase = xptr->xlimit + STKSIZE - sizeof(WORD);
      xptr->xstate = XFREE;
      /*below code added for initialization of the Each of the Queue variables*/
      qptr = &xque[i];  			// getting the base address of the Queue of i(th) thread
      qptr->front = 0;
      qptr->size = 0;  
      qptr->rear = 9;
   }

   /* the first thread runs user's xmain with id 0*/
   signal(SIGALRM, handler);		// decalring the SIGALRM signal
   xidxmain = xthread_create(xmain, 2, argc, argv);
   xtab[xidxmain].xstate = XRUN; 
   ualarm(20000,0);					// use of the Signal
   ctxsw(m.xregs, xtab[xidxmain].xregs);
   /* never be here */
}


