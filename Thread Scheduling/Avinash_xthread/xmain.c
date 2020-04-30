/* combo test */
#include <stdio.h>

int xidfoo, xidgoo, xidhoo;

int y=0;

void delay(int d)
{
   float w=0,x=2.0;
   int i,j;

   for(i=0; i< 1000*d; i++) 
      for(j=0; j< 1000*d; j++) 
           w = w + x; 
}   


int foo(int f)
{
   int j;

   xthread_join(xidgoo, &j);
   printf("*** The last word from goo: %d\n",j);
   xthread_join(xidhoo, &j);
   printf("*** The last word from hoo: %d\n",j);
}

int goo(int p, int q)
{
   int j;

   for(j=0;j<3;j++){
        delay(2);
	printf("*** This is goo %d, %d\n", p-q, ++y);
   }
   xthread_exit(y);
}

int hoo(int a, int b, int c)
{
   int j;

   for(j=0;j<3;j++){
        delay(2);
	printf("*** This is hoo %d, %d\n", a+b+c, ++y);
   }
   delay(2);
   xthread_exit(y);
}


xmain(int argc, char* argv[])
{
   	xidfoo = xthread_create(foo, 1, 7);
   	xidgoo = xthread_create(goo, 2, 32, 12);
   	xidhoo = xthread_create(hoo, 3, 1, 2, 3);
}


