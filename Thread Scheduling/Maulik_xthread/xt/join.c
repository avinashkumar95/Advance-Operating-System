#include <stdio.h>
#include <proc.h>

/*---------------------------------------------------
 *  xthread_join  -  waiting to get final word 
 *---------------------------------------------------
 * Author : Maulik Gohil & Neel Kalariya
 * Change log: 
 *	Date Feb 26, 2020 > Created this file with whatever you have explained/write down in the class.
 *	Date Feb 27, 2020 > Added "return -1" and "return -2" for first two if condition
 *			  > Added one line in third if -> *tid_retvalue = xtab[currxid].jstatus;
 *	Date Feb 28, 2020 > Implemented lock for the critical section
 *			  > Implemeted Queue so added -> enqueue(&xque[tid],currxid); 
 *	Date Feb 29, 2020 > Got rid of Sagmentation Fault and Queue Implemtation Started working (Got correct output)
 */

int xthread_join(int tid, int *tid_retvalue)
{
	int usec = ualarm(0,0);    			// beginning of critical section    
	// If a thread waiting for itself	
	if (tid == currxid){
		ualarm (usec,0);   			// end of critical section 
		return -1;
	}
	// If a thread wants to join which is in XFREE state
	if (xtab[tid].xstate == XFREE){
		ualarm (usec,0);   			// end of critical section 	
		return -2;	
	}
	// If non of the above condition & if a thread waiting for another thread who has not become ZOMBIE
	if (xtab[tid].xstate != XZOMBIE){
		xtab[currxid].xstate = XWAIT;		// changing state to XWAIT/XJOIN for the waiting thread
		enqueue(&xtab[tid].que,currxid);	// Enquing the calling(currxid) thread of tid 		
		resched();				// now finding the next ready thread to run
		*tid_retvalue = xtab[currxid].jstatus; 	// giving back the its own pre initialized jstatus to the *tid_retvalue
		ualarm (usec,0);  			// end of critical section 
		return 0;
	}
	// If a thread waiting for for another thread who is already became ZOMBIE
	else{
		*tid_retvalue = xtab[tid].status; 	// giving back the final status (last word) to the *tid_retvalue to JOIN 
		xtab[tid].status = 0;			// clearing the status value
		xtab[tid].xstate = XFREE;		// Changing the state to XFREE
		ualarm (usec,0);			// end of critical section
		return 0;
	}
}


/*---------------------------------------------------
 *  xthread_exit  -  thread exits and becomes Zombie 
 *---------------------------------------------------
 * Author : Maulik Gohil & Neel Kalariya
 * Change log: 
 *	Date Feb 26, 2020 > Created this file with whatever you have explained/write down in the class.
 *	Date Feb 27, 2020 > Wrote if code and its while loop as Pseudo code because We couldn't implement Queue that day.
 *	Date Feb 28, 2020 > Added line -> dqid = dequeue(&xque[currxid]) as we were done with Queue Implementation.
 *			  > Added ualarm for the critical section 
 *			  > Added the same two lines "xtab[currxid].xstate = XZOMBIE;" & "xtab[currxid].status = status;" 
 *		            which are in else block in the end of the if block,	because after dequeue, exiting thread should  
 *     			    become ZOMBIE and its own status should be saved in its status variable.
 * 
 */

void xthread_exit(int status)
{
	int dqid;
	/* if Queue of currxid is not empty */
	int usec = ualarm (0,0); 			// start of critical section
	if (xtab[currxid].que.size != 0){
		while(xtab[currxid].que.size > 0 ){	// loop till Queue size becomes Zero
			dqid = dequeue(&xtab[currxid].que); // dequeue xthread id
			xtab[dqid].jstatus = status;	// Put status of currxid to jstatus of dqid
			xtab[dqid].xstate = XREADY;  	// And put dqid thread to the XREADY state
		}
		xtab[currxid].xstate = XZOMBIE;		// As currxid exiting, changing state to XZOMBIE
		xtab[currxid].status = status;		// Also storing its current status to its status varibale
		ualarm(usec,0);  			// end of critical section
	}
	/* if Queue is already empty */
	else{    			
		xtab[currxid].xstate = XZOMBIE;		// As currxid exiting, changing state to XZOMBIE
		xtab[currxid].status = status;		// Also storing its current status to its status varibale
		ualarm(usec,0);			// end of critical section
	}
}
