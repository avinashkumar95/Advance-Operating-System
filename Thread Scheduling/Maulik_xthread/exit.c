/*---------------------------------------------------
 *  xthread_exit  -  thread exits and becomes Zombie 
 *---------------------------------------------------
 * Author : Maulik Gohil & Neel Kalariya
 * Change log: 
 *		Date Feb 26, 2020 > Created this file with whatever you have explained/write down in the class.
 *		Date Feb 27, 2020 > Wrote if code and its while loop as Pseudo code because We couldn't implement Queue that day.
 *		Date Feb 28, 2020 > Added line -> dqid = dequeue(&xque[currxid]) as we were done with Queue Implementation.
 *						  > Added ualarm for the critical section 
 *						  > Added the same two lines "xtab[currxid].xstate = XZOMBIE;" & "xtab[currxid].status = status;" 
 *							which are in else block in the end of the if block,	because after dequeue, exiting thread should  *							 become ZOMBIE and its own status should be saved in its status variable.
 * 
 */

#include <stdio.h>
#include <proc.h>

void xthread_exit(int status)
{
	int dqid;
	/* if Queue of currxid is not empty */
	if (xque[currxid].size != 0){	
		int usec = ualarm (0,0); 			// start of critical section
		while(xque[currxid].size > 0 ){		// loop till Queue size becomes Zero
			dqid = dequeue(&xque[currxid]); // dequeue xthread id
			xtab[dqid].jstatus = status;	// Put status of currxid to jstatus of dqid
			xtab[dqid].xstate = XREADY;  	// And put dqid thread to the XREADY state
		}
		xtab[currxid].xstate = XZOMBIE;		// As currxid exiting, changing state to XZOMBIE
		xtab[currxid].status = status;		// Also storing its current status to its status varibale
		ualarm(usec,0);  					// end of critical section
	}
	/* if Queue is already empty */
	else{    			
		int usec1 = ualarm (0,0);			// start of critical section
		xtab[currxid].xstate = XZOMBIE;		// As currxid exiting, changing state to XZOMBIE
		xtab[currxid].status = status;		// Also storing its current status to its status varibale
		ualarm(usec1,0);					// end of critical section
	}
}

