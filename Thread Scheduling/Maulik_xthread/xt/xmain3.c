/*
*  Custom test case for Part 2
*  Test case : Two theads [foo] & [oof] waiting for single thread [oof].
*/

#include <stdio.h>

int xidfoo, xidbar, xidoof;

void foo(int f)
{
   int status;
   int ret;
   printf("*** Entering foo *** \n");
   ret = xthread_join(xidoof, &status);
   printf("*** foo join oof - %d, ret - %d *** \n", status, ret);
}

void bar(int p, int q)
{
   int status;
   int ret;
   printf("*** Entering bar *** \n");
   ret = xthread_join(xidoof, &status);
   printf("*** bar join oof - %d, ret - %d *** \n", status, ret);
}

void oof(int g)
{
   printf("*** oof exits with status %d. *** \n", g);
   xthread_exit(g);
}

xmain(int argc, char* argv[])
{
   xidfoo = xthread_create(foo, 1, 7);
   xidbar = xthread_create(bar, 2, 32, 12);
   xidoof = xthread_create(oof, 1, 12345);
   //printf("*** foo id - %d Bar id - %d oof id - %d ***\n",xidfoo,xidbar,xidoof);
}


