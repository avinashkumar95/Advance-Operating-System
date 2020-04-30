#include <stdio.h>
#include <string.h>
#include <math.h>
#include <rpc/rpc.h>
#include "rcalc.h"

struct list *retList = NULL;

//Server method for sumsqrt range
list		  *
sumsqrt_range_1_svc(struct list *lst, struct svc_req *rqp)
{
	retList= (struct list*) malloc(sizeof(struct list));
	double start,end;
	double sum = 0;
	
	start=lst->data;
	lst=lst->next;
	end=lst->data;
	//printf("%lf\t%lf\n",start,end);
	
	int i;
	for(i = start ; i <= end ; i++)
	{
		sum=sum+sqrt(i);		//sqrt is from math.h header
	}		
	retList->data = sum;
	retList->next = NULL;
	
	return ((list *) retList);
}

//Server method for update_list
list        *
update_list_1_svc(struct list *lst, struct svc_req *rqp)
{
	retList= (struct list*) malloc(sizeof(struct list));
	struct list *tmp;
	double value;
	
	tmp = lst;
	while(tmp->next != NULL)
	{
		value = tmp->data;
		value = pow(value,2.0)/10.0;
		tmp->data = value;
		tmp = tmp->next;
	}
	value = tmp->data;
	value = pow(value,2.0)/10.0;		//Last data from the linked list
	tmp->data = value;
	retList = lst;		//Providing Head of Linked-List to Return Linked-List
	
	return ((list *) retList);
}

