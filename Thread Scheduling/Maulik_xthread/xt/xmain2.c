/*
* Custom test case for Part 1
* Here I wrote random funciton which will genereate random number between 0 to 5-million and -
* that amount time the loop will run. To get dynamicity in the number of print statement. We did this.
*/

#include <stdio.h>
#include <stdlib.h>

int xidfoo, xidbar;

int randomNumber(){
	int value = rand()%5000000; //generates random number between 0 to 5-million
	return value;
}

int foo(int f)
{
   int i,z,stop;
   for(i=0;i<100;i++){
      printf("Hi from foo %d, %d ->>>", f, i);
	stop = randomNumber(); // getting the randon number
	printf("#of times loop ran - %d \n",stop);
	while(stop>0){
		stop--;
	}
   }
}

int bar(int p, int q)
{
   int j,k,stop;
   for(j=0;j<100;j++){
      printf("Hi from bar %d, %d ---- ", p-q, j);
	stop = randomNumber();  // getting the randon number
	printf("#of times loop ran - %d \n",stop);
	while(stop>0){
		stop--;
	}
   }
}

xmain(int argc, char* argv[])
{
   xidfoo = xthread_create(foo, 1, 7);
   xidbar = xthread_create(bar, 2, 32, 12);
   printf("Finished xmain \n");
}


