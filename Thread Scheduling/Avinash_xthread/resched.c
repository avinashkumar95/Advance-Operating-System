/* resched.c  -  resched 
Name: Avinash Kumar, Shivam Joshi
*/
#include <stdio.h>
#include <proc.h>

/*------------------------------------------------------------------------
 * resched  --  find a live thread to run
 *
 *------------------------------------------------------------------------
 */
void resched()
{
    register struct  xentry  *cptr;  /* pointer to old thread entry */
    register struct  xentry  *xptr;  /* pointer to new thread entry */
    int i,next;

    cptr = &xtab[currxid];

    next = currxid ;
    for(i=0; i<NPROC; i++) { 
        if( (++next) >= NPROC)
             next = 0;
        if(xtab[next].xstate == XREADY) {
            xtab[next].xstate = XRUN;
            xptr = &xtab[next];
            currxid = next;
            ualarm(2000,0);	//waiting for 0.02 sec and then for context switch
	    ctxsw(cptr->xregs,xptr->xregs);
            return;
        }
    }
    printf("XT: no threads to run!\n");
    exit(0);
}



