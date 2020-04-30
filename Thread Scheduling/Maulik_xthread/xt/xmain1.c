/*
* Custom test case for Part1
*/

#include <stdio.h>

int xidfoo, xidbar;

int foo(int f)
{
   int i,z;
   for(i=0;i<20;i++){
      printf("Hi from foo %d, %d\n", f, i);
      for(z=0;z<5000000;z++);
   }
}

int bar(int p, int q)
{
   int j,k;
   for(j=0;j<20;j++){
      printf("Hi from bar %d, %d\n", p-q, j);
      for(k=0;k<5000000;k++);
   }
}

xmain(int argc, char* argv[])
{
   xidfoo = xthread_create(foo, 1, 7);
   xidbar = xthread_create(bar, 2, 32, 12);
   printf("Finished xmain \n");
}


