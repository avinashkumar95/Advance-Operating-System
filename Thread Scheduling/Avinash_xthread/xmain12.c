/* xmain24.c */
/*
Name: Avinash Kumar, Shivam Joshi
Test case 4 of part 2 of our project assignment. If current thread is waiting for some thread which is not created or state as XFREE then it will return -2. 
*/
#include <stdio.h>

int xidfoo,xidoof;
int xidbar=9;	//xidbar=9 to explicitly giving and checking if thread tid
		//doesn't exists
int x=0;

int foo(int f)
{
   int i;
   int status,ret;
   printf("*: Entering Foo :*\n");
   ret = xthread_join(xidbar,&status);
   printf("*: Foo Join thread which is not exists %d, ret %d :*\n",status,ret);
}

int oof(int g)
{
   printf("*: Oof Exit With %d :*\n",g);
   xthread_exit(g);
}
xmain(int argc, char* argv[])
{
   xidoof = xthread_create(oof,1, 12345);
   xidfoo = xthread_create(foo, 1, 7);
   printf("\nFinish in Xmain\n");
}

