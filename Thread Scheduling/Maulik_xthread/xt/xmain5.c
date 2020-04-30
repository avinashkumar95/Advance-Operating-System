/*
*  Custom test case for Part 2
*  Test case : One thead [foo] waiting for itself > must return -1 as per the join.c
*/

#include <stdio.h>

int xidfoo, xidoof;

void foo(int f)
{
   int status;
   int ret;
   printf("*** Entering foo *** \n");
   ret = xthread_join(xidfoo, &status);
   printf("*** foo join oof - %d, ret - %d *** \n", status, ret);
}

void oof(int g)
{
   printf("*** oof exits with status %d. *** \n", g);
   xthread_exit(g);
}

xmain(int argc, char* argv[])
{
   xidoof = xthread_create(oof, 1, 12345);
   xidfoo = xthread_create(foo, 1, 7);
}


