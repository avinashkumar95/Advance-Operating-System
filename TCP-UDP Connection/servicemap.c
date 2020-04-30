#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include<errno.h>
#include<unistd.h>

#define SIZE 50
#define SERV_NT_FOUND "INVALID Service name!"
#define BUFMAX 2048
#define UDPSOCKET 28911
#define MSGSERVER "OK"

struct serviceData
{
	char name[15];
	char data[30];
};

struct serviceData* serviceTable[SIZE];
struct serviceData* item;

/* Method to search data from the table respect with Service name. */
struct serviceData *search(char serviceName[15])
{
	int index= 0;
	while(serviceTable[index]!= NULL)
	{
		if(strcmp(serviceTable[index]->name,serviceName)==0)
			return serviceTable[index];
		index++;
	}
	return NULL;
}

/* Method to insert data into the table(Structure ServiceTable)*/
void insert(char serviceName[15],char data[30])
{
	struct serviceData *item=(struct serviceData*) malloc(sizeof(struct serviceData));
	strcpy(item->data,data);
	strcpy(item->name,serviceName);
	int index= 0;
	while(serviceTable[index]!= NULL && strcmp(serviceTable[index]->name,"")!=0)
		index++;
	serviceTable[index]= item;
}

int main()
{
	while(1)
	{
		struct sockaddr_in local, remote;
		int sk, rlen= sizeof(remote),len= sizeof(local);
		char buf[BUFMAX];
		sk= socket(AF_INET,SOCK_DGRAM,0); //for UDP, socket creation
		local.sin_family= AF_INET;
		local.sin_addr.s_addr= INADDR_ANY;
		local.sin_port = ntohs(UDPSOCKET);
		if(bind(sk,(struct sockaddr *)&local,sizeof(local))< 0)
		{
			printf("Bind Failed!!!\n");
			exit(1);
		}
		recvfrom(sk,buf,BUFMAX,0,(struct sockaddr*)&remote,&rlen);
		
		char *ch;
		int count= 0;
		char queryType[10]= "";
		char serviceName[15]= "";
		char serverData[20]= "";
		char serverIp[20]= "";
		
		char finalValue[30]= "";
		//char colon[1] = ":";
		strcpy(serverIp,inet_ntoa(remote.sin_addr));
		strcat(serverIp,":");
		printf("Received From %s %s \n",serverIp,buf);
		
		/*below loop is to tokenize the MSG from server/client to store/search it in the Service Table*/
		ch= strtok(buf," ");
		while(ch!= NULL)
		{
			//printf("%s\n",ch);
			if(count==0)
				strcpy(queryType,ch);
			else if(count==1)
				strcpy(serviceName,ch);
			else if(count==2)
				strcpy(serverData,ch);
			ch= strtok(NULL," ");
			count++;
		}
		if(strcmp(queryType,"PUT")==0)
		{
			strcat(finalValue,serverData);
			insert(serviceName,finalValue);
			//printf("%s %s\n",serviceName,finalValue);
			sendto(sk,MSGSERVER,strlen(MSGSERVER)+1,0,(struct sockaddr *) &remote, sizeof(remote));
		}
		else if(strcmp(queryType,"GET")==0)
		{
			item= search(serviceName);
			if(item!=NULL)
			{
				sendto(sk,item->data,strlen(item->data)+1,0,(struct sockaddr *) &remote, sizeof(remote));
			}
			// if service doesn't exist in the table
			else
			{
				sendto(sk,SERV_NT_FOUND,strlen(SERV_NT_FOUND)+1,0,(struct sockaddr *) &remote, sizeof(remote));
			}
		}
		close(sk);
	}
	return 0;
}