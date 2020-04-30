/*    proc.h    
Name: Avinash Kumar, Shivam Joshi
*/
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
#define XZOMBIE  3
#define XJOIN    4

/* Structure for Queue of each xthread */
typedef struct Queue
{
        int capacity;
        int size;
        int front;
        int rear;
        int *elements;
}Queue;
struct xentry  {
       int  xid;
       WORD xregs[PNREGS];    /* save SP */
       WORD xbase;
       WORD xlimit;
       int  xstate;
       int status;
       int jstatus;
       struct Queue Q;
};

#define STKSIZE 8192 

extern struct xentry xtab[];
extern int currxid;


