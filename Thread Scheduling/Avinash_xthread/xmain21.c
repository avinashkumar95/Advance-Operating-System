/* xmain21.c */
/*
Name: Avinash Kumar, Shivam Joshi
Test case 1 of part 2 of our project assignment. It will perform the xthread_join() and xthread_exit() and print accordingly. 
*/
#include <stdio.h>


int xidfoo, xidbar,xidoof;
int x=0;

int foo(int f)
{
   int i;
   int status,ret;
   printf("*: Entering Foo :*\n");
   ret = xthread_join(xidoof,&status);
   printf("*: Foo Join oof %d, ret %d :*\n",status,ret);
}

int bar(int p, int q)
{
   int j;
   int status,ret;
   printf("*: Entering Bar :*\n");
   ret = xthread_join(xidoof,&status);
   printf("*: Bar Join oof %d, ret %d :*\n",status,ret);
}

int oof(int g)
{
   printf("*: Oof Exit With %d :*\n",g);
   xthread_exit(g);
}
xmain(int argc, char* argv[])
{
   xidfoo = xthread_create(foo, 1, 7);
   xidbar = xthread_create(bar, 2, 32, 12);
   xidoof = xthread_create(oof,1,12345);
   //xthread_yield(xidfoo);
   printf("\nFinish in Xmain\n");
}



