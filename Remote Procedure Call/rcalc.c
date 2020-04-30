#include <stdio.h>
#include <ctype.h>
#include <rpc/rpc.h>
#include <string.h>
#include "rcalc.h"

//Macro to print for SumSqrt Range Function
#define PRINTRECORD(pR) {\
	printf("%lf\n",pR->data);\
	}
  
//Macro to Print for UpdateList as we need 1 decimal point precision
#define PRINTRECORD1(pR) {\
    struct list *t;\
	t=pR;\
	while(t->next!=NULL){\
		printf("%.1lf ",t->data);\
		t=t->next;\
		}\
	printf("%.1lf\n",t->data);\
	}
  
struct list *head = NULL;

//Insert Data to the linked-list
void list_insert(double val)
{
	  struct list *node, *tmp;
	  
	  node = (struct list*) malloc(sizeof(struct list));
	  node->data = val;

	  if(head==NULL){
		head =node;
		head->next=NULL;
		return;
	  }
	  
	  tmp=head;
	  while(tmp->next!=NULL){
		tmp=tmp->next;		  
	  }

	  tmp->next=node;
	  node->next=NULL;
}

//Print the entire linked-list
/*void traverse()
{
	struct list *t;
	t=head;
	while(t->next!=NULL){
		printf("%lf\t",t->data);
		t=t->next;
	}
	printf("%lf\n",t->data);
}*/

int main(argc, argv)
int argc;
char *argv[];
{
	CLIENT *cl;    /* a client handle */
	int key = 1000;
	
	if ((argc < 5) || ((!ispunct(argv[2][0]) && (!islower(argv[2][1]))))) {
		fprintf(stderr, "Usage: %s server key value\n", argv[0]);
		exit(1);
	}
	if (!(cl = clnt_create(argv[1], RCALCPROG, RCALCVERS, "tcp"))) {
    /*	CLIENT handle couldn't be created, server not there.   */
    clnt_pcreateerror(argv[1]);
    exit(1);
	}
	
	//Key value is for switch case condition
	if(strcmp(argv[2],"-r")==0)
		key = 1;
	else if(strcmp(argv[2],"-u")==0)
		key = 2;
	
	//To retrive the total number of argument which need to be send to server
	int i;
	for(i=3;i<argc;i++)
		list_insert((double)atof(argv[i]));
	//traverse();

	switch (key) 
	{
	case SUMSQRT_RANGE:
		//printf("IN RANGE -r \n");
		PRINTRECORD(sumsqrt_range_1(head, cl));
		clnt_destroy(cl);
		break;
	case UPDATE_LIST:
		//printf("IN UPDATE -u \n");
		PRINTRECORD1(update_list_1(head, cl));
		clnt_destroy(cl);
		break;
	default:
		fprintf(stderr, "%s: unknown key\n", argv[0]);
		exit(1);
	}
	return 0;
}
