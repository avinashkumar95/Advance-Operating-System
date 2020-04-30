/*
Nam: Avinash Kumar, Shivam Joshi
Code Description: There is a handler function which will be called in every 0.02 sec.
Change Log:	2/20/2020: Implement the handler function for the part 1
		2/25/2020: Implemented the queue and created the queue function
*/
#include <stdio.h>
#include <proc.h>
#include <signal.h>
#include <unistd.h>

extern void xmain();

struct xentry xtab[10]; 
int currxid = 0;

/* Handler function will call when there will be an interrupt after 0.02sec */ 
void handler()
{
   sigset_t set;
   sigemptyset(&set);
   sigaddset(&set,SIGALRM);
   sigprocmask(SIG_UNBLOCK,&set,NULL);
   xtab[currxid].xstate = XREADY;
   resched();
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
      createQueue(&xptr->Q,NPROC);	//queue is created and initialized for all the xtab entries. createQueue function is defined at queue.c
   }

   /* the first thread runs user's xmain with id 0*/
   xidxmain = xthread_create(xmain, 2, argc, argv);
   signal(SIGALRM,handler);		//signalling to switch the or resched thread after 0.02sec.
   xtab[xidxmain].xstate = XRUN;
   ualarm(20000,0);
   ctxsw(m.xregs, xtab[xidxmain].xregs);
   /* never be here */
}



