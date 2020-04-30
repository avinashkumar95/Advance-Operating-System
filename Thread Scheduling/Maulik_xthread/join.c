/*---------------------------------------------------
 *  xthread_join  -  waiting to get final word 
 *---------------------------------------------------
 * Author : Maulik Gohil & Neel Kalariya
 * Change log: 
 *		Date Feb 26, 2020 > Created this file with whatever you have explained/write down in the class.
 *		Date Feb 27, 2020 > Added "return -1" and "return -2" for first two if condition
 *						  > Added one line in third if -> *tid_retvalue = xtab[currxid].jstatus;
 *		Date Feb 28, 2020 > Implemented lock for the critical section
 *						  > Implemeted Queue so added -> enqueue(&xque[tid],currxid); 
 *		Date Feb 29, 2020 > Got rid of Sagmentation Fault and Queue Implemtation Started working (Got correct output)
 */


#include <stdio.h>
#include <proc.h>

int xthread_join(int tid, int *tid_retvalue)
{
	int usec = ualarm(0,0);    				// beginning of critical section    
	/* If a thread waiting for itself */	
	if (tid == currxid){
		ualarm (usec,0);   					// end of critical section 
		return -1;
	}
	/* If a thread wants to join which is in XFREE state */
	if (xtab[tid].xstate == XFREE){
		ualarm (usec,0);   					// end of critical section 	
		return -2;	
	}
	/* If non of the above condition & if a thread waiting for another thread who has not become ZOMBIE */
	if (xtab[tid].xstate != XZOMBIE){
		xtab[currxid].xstate = XWAIT;		// changing state to XWAIT/XJOIN for the waiting thread
		enqueue(&xque[tid],currxid);		// Enquing the calling(currxid) thread of tid 		
		resched();							// now finding the next ready thread to run
		*tid_retvalue = xtab[currxid].jstatus; 	// giving back the its own pre initialized jstatus to the *tid_retvalue
		ualarm (usec,0);  					// end of critical section 
		return 0;
	}
	/* If a thread waiting for for another thread who is already became ZOMBIE */
	else{
		int usec1 = ualarm(0,0);       		// beginning of critical section
		*tid_retvalue = xtab[tid].status; 	// giving back the final status (last word) to the *tid_retvalue to JOIN 
		xtab[tid].status = 0;				// clearing the status value
		xtab[tid].xstate = XFREE;			// Changing the state to XFREE
		ualarm (usec1,0);					// end of critical section
		return 0;
	}
}

