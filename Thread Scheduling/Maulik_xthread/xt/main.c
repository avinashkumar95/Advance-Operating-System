/*
 * Here we have made few changes to initialize the queue variables.
 * Author : Maulik Gohil & Neel Kalariya
 * Change log: 
 *	Date Feb 26, 2020 > Edited this file and implemted handler for SIGALRM Signal
 *			  > Declared and initialization of signal.
 *			  > Added "xtab[currxid].xstate = XREADY;" (came to know due to incorrect output)
 *	Date Feb 28, 2020 > Used "xptr->que->front = 0;" instead of "xptr->que.front = 0;"
 *			  > Initialized all the variables for each queue.
 *
*/

#include <stdio.h>
#include <proc.h>
#include <signal.h>
#include <unistd.h> 


extern void xmain();

struct xentry xtab[10]; 
int currxid = 0; 

/* Here handler has been defined for SIGINT signal to make this whole system preemtive, every 0.02 sec this will be executed */
void handler(){
   	sigset_t set;
   	sigemptyset(&set);
	sigaddset(&set,SIGALRM);
	sigprocmask(SIG_UNBLOCK, &set, NULL);
	xtab[currxid].xstate = XREADY;		// changing the state to XREADY 
	resched();   				// find next thread to run
}

void main(int argc, char *argv[])
{
   register struct xentry *xptr;
   struct xentry m;
   int i;
   int xidxmain;

   for(i=0 ; i < NPROC; i++){
      xptr = &xtab[i];
      xptr->xid = i;
      xptr->xlimit =  (WORD) malloc(STKSIZE);
      xptr->xbase = xptr->xlimit + STKSIZE - sizeof(WORD);
      xptr->xstate = XFREE;
      /* below code added for initialization of the Each of the Queue variables */
      xptr->que.front = 0;
      xptr->que.size = 0;  
      xptr->que.rear = 9;
   }

   /* the first thread runs user's xmain with id 0*/
   signal(SIGALRM, handler);			// decalring the SIGALRM signal
   xidxmain = xthread_create(xmain, 2, argc, argv);
   xtab[xidxmain].xstate = XRUN; 
   ualarm(20000,0);				// initializing the Signal
   ctxsw(m.xregs, xtab[xidxmain].xregs);
   /* never be here */
}


