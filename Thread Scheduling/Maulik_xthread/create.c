/*------------------------------------------------------------------------
 * userret  --  entered when a thread exits by return
 *------------------------------------------------------------------------
*/
#include <stdio.h>
#include <proc.h>

void userret()
{
   /* Initially the whole system was Non-preemtive, when thread returns normally the status changes to XFREE.  
    * After making the system preemtive, every process becomes ZOMBIE first and if a new thred joins ZOMBIE 
	* thread then only it can be XFREE.
    * Hence, here in the below line I changed XFREE to XZOMBIE. 
	*/
	
   xtab[currxid].xstate = XZOMBIE;			
   printf("XT: Old threads never die; they just fade away. (id:%d) \n",currxid);
   /* find the next runnable thread to trun */
   resched();
}

static int newxid()
{
    int i, xid;
    static int nextproc =0;
  
    for(i=0; i<NPROC; i++) { /* find a free process entry */
        xid = nextproc;
        if((++nextproc) >= NPROC)
             nextproc = 0;
        if(xtab[xid].xstate == XFREE)
             return(xid);
    }
    printf("Error: run out of process table ! \n"); 
    exit(1);
}

/*------------------------------------------------------------------------
 *  xthread_create  -  create a process to start running a procedure
 *------------------------------------------------------------------------
 */
int xthread_create(int *procaddr,int nargs, int args)
{
    WORD *saddr;              /* stack address */
    WORD *ap;  
    struct xentry *xptr;
    int xid;

    xid = newxid();
    xptr = &xtab[xid];
    xptr->xstate = XREADY;

    saddr = (WORD *) xptr->xbase;

    ap = (&args) + nargs;
    for(; nargs > 0; nargs--)
        *(--saddr) = *(--ap); /* copy args onto new process' stack */
    *(--saddr)  = (int)userret;   /* sooner or later you will be there */     
    *(--saddr)  = (int)procaddr;
    --saddr;           /* for frame ebp; it's not important !? */
    saddr -= 2;        /* 2 words for si and di */
    xptr->xregs[SP] = (int) saddr ;

    return(xid);
}
