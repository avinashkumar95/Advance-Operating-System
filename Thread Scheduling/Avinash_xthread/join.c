/*
Name: Avinash Kumar, Shivam Joshi
Code Description: When a thread is waiting for the another thread to complete/exit/finish, current thread will call join with the id of the waiting for thread. If the current thread is waiting for itself, then join() will return -1, if current thread is waiting for an thread which is not created (doesn't exists) then it will return -2. For other state if the tid is not finished (not ZOMBIE state) then current thread will be enqueue to the queue of tid else if it tid is in xombie state, then the status will be fetched from tid's status and it will be initialize as 0, and the state will be as XFREE, also the function will return 0.
The function xthread_exit() is called when a current thread tried to exit/end and the threads tid who are waiting for the current thread to finish, will be dequeue from the current thread queue and initialised as a ready state. Also if there is no thread to run in the queue of current thread then the current state will be as zombie and its status will be in the status block of current thread.

Change Log: 2/21/2020: Created join function in join.c
	    2/24/2020: Created exit function in exit.c
	    2/25/2020: Implementation of Queue
	    2/27/2020: Debugging and Testing
	    3/3/2020:  Combined join function and exit function in join.c

*/
#include <proc.h>

int xthread_join(int tid,int *tid_retval)
{
	int usec = ualarm(0,0);		//Lock
	if(tid==currxid)
	{
		ualarm(usec,0);		//Unlock
		return -1;
	}
	if(xtab[tid].xstate==XFREE)
	{
		ualarm(usec,0);		//Unlock
		return -2;
	}
	if(xtab[tid].xstate!=XZOMBIE)
	{
		xtab[currxid].xstate = XJOIN;
		Enqueue(&xtab[tid].Q,currxid);	//Enqueue Function defined in queue.c
		resched();
		*tid_retval = xtab[currxid].jstatus;
		ualarm(usec,0);		//Unlock
		return 0;
	}
	else
	{
		*tid_retval = xtab[tid].status;
		xtab[tid].status = 0;
		xtab[tid].xstate = XFREE;
		ualarm(usec,0);		//Unlock
		return 0;
	}
}

void xthread_exit(int status)
{
	int usec = ualarm(0,0);		//Lock
	int jid;
	if(xtab[currxid].Q.size!=0)
	{
		while(xtab[currxid].Q.size>0)
		{
			Dequeue(&xtab[currxid].Q,&jid);	//Dequeue function defined at queue.c
			xtab[jid].jstatus = status;
			xtab[jid].xstate = XREADY;
		}
	}
	else
	{
		xtab[currxid].xstate = XZOMBIE;
		xtab[currxid].status = status;
	}
	ualarm(usec,0);			//Unlock

}

