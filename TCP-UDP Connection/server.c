#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/un.h>
#include <signal.h>
#include <math.h>
#include <wait.h>

#define QUERY 1001
#define UPDATE 1002
#define BUFMAX 2048
#define UDPSOCKET 28911

struct record {
	int acctnum;
	char name[20];
	float value;
	int age;
}rec;

void signal_catcher(int the_sig){
	wait(0);
}

/* Method to search the file db20 and return account details */
 
void fileRead(int accNo, struct record *recordData)
{
	int fd;
	int recordFound = 0;
	int readSize;
	fd = open("db20",O_RDWR);
	int sizeOfRecord = sizeof(struct record);
	lseek(fd,sizeOfRecord,1);
	do{
		readSize = read(fd,&rec,sizeOfRecord);
		if(accNo == rec.acctnum)
		{
			recordFound = 1;
			stpcpy(recordData->name,rec.name);
			recordData->acctnum=rec.acctnum;
			recordData->value=rec.value;
		}
	}while(readSize > 0 && recordFound == 0);
	close(fd);
}

/* Method to Update the file db20 and return account details */

void fileUpdate(int accNo, float value, struct record *recordData)
{
	int fd;
	int recordFound = 0;
	int readSize;
	fd = open("db20",O_RDWR);
	int sizeOfRecord = sizeof(struct record);
	lseek(fd,sizeOfRecord,1);
	do{
		readSize = read(fd,&rec,sizeOfRecord);
		if(accNo == rec.acctnum)
		{
			lseek(fd,-sizeOfRecord,1);
			if(lockf(fd,F_LOCK,sizeOfRecord) == -1){
				printf("Cannot lock record");
				recordFound = 1;
				break;
			}
			read(fd,&rec,sizeOfRecord);			// RE-READ the record.
			rec.value = rec.value + value;
			lseek(fd,-sizeOfRecord,1);
			write(fd,&rec,sizeOfRecord);
			lockf(fd,F_ULOCK,-sizeOfRecord); //unlock the record
			
			strcpy(recordData->name,rec.name);
			recordData->acctnum=rec.acctnum;
			recordData->value=rec.value;
			recordFound = 1;
		}
	}while(readSize > 0 && recordFound == 0); 	//loop till EOF or account is found
	close(fd);
}

/* method to replace a string with its old character to new character*/

void replace(char *str,char old,char new){
	int len = strlen(str);
	int i;
	for(i=0;i<len;i++)
	{
		if(str[i]==old){
			str[i] = new;
		}
	}
}

int main(int argc, char* argv[]){
	char buf[BUFMAX];
	char Message[50] = "PUT CISBANK ";
	
	struct sockaddr_in remoteUDP,localUDP,clientTCP,serverTCP;
	socklen_t clientTCP_len;
	int sk, rlen=sizeof(remoteUDP),len=sizeof(localUDP),tcp_sock,new_sock;
	struct hostent *hp;
	
	/*Create an Internet domain datagram socket*/
	sk = socket(AF_INET, SOCK_DGRAM, 0);
	
	localUDP.sin_family = AF_INET; /*Define the socket domain*/
	localUDP.sin_addr.s_addr = INADDR_ANY; 
	localUDP.sin_port = 0;
	if(bind(sk,(struct sockaddr *)&localUDP, sizeof(localUDP)) < 0){
		printf("bind fails");
		exit(1);
	}
	/*For Getting Local IP address*/
	char hostbuffer[16]; 
	char *IPbuffer; 
	struct hostent *host_entry;
	int hostname;
	hostname = gethostname(hostbuffer, sizeof(hostbuffer));
	host_entry = gethostbyname(hostbuffer);
	IPbuffer = inet_ntoa(*((struct in_addr*)host_entry->h_addr_list[0]));
	char serverString[30]="";
	strcpy(serverString,IPbuffer);
	
	/*Get the port number assigned by the system*/
	getsockname(sk,(struct sockaddr *)&localUDP,&len); 
	int serverPort = htons(localUDP.sin_port);
	int qu = serverPort / 256;
	int re = serverPort % 256;
	
	char socketQuotient[4];
	char socketRemainder[4];
	sprintf(socketQuotient, "%u", qu);
	sprintf(socketRemainder, "%u", re);
	strcat(serverString,".");
	strcat(serverString,socketQuotient);
	strcat(serverString,".");
	strcat(serverString,socketRemainder);
	replace(serverString,'.',',');
	strcat(Message, serverString);
	//printf("%s \n",Message);
	
	remoteUDP.sin_family = AF_INET;
	remoteUDP.sin_addr.s_addr = inet_addr("137.148.205.255"); // broadcast IP address of the network
	remoteUDP.sin_port = ntohs(UDPSOCKET);
	
	setsockopt(sk,SOL_SOCKET,SO_BROADCAST,(struct sockaddr *)&remoteUDP,sizeof(remoteUDP));
	sendto(sk,Message,strlen(Message)+1,0,(struct sockaddr *)&remoteUDP,sizeof(remoteUDP));
	recvfrom(sk,buf,BUFMAX,0,(struct sockaddr *)&remoteUDP,&rlen); //Waiting for the reply
	char serverIp[16]="";
	strcpy(serverIp,inet_ntoa(remoteUDP.sin_addr));
	printf("Registration %s from %s\n", buf,  serverIp);
	close(sk); // Close the UDP connection to servicemap as it is no longer needed.
	
	/* Start of TCP Connection with client  */
	if(signal(SIGCHLD,signal_catcher)==SIG_ERR){
		perror("SIGCHLD");
		return 1;
	}
	
	if((tcp_sock = socket(AF_INET,SOCK_STREAM,0)) < 0){
		perror("generate error");
		return 2;
	}
	
	serverTCP.sin_family = AF_INET;		//Set Address
	serverTCP.sin_addr.s_addr = INADDR_ANY;	//Any Interface
	serverTCP.sin_port = ntohs(serverPort);
	//printf("%d\n",serverPort);
	if(bind(tcp_sock,(struct sockaddr *)&serverTCP,sizeof(serverTCP)) < 0)
	{
		close(tcp_sock);
		perror("bind error");
		return 3;
	}
	if(listen(tcp_sock,5)<0)
	{
		close(tcp_sock);
		perror("listen error");
		return 4;
	}
	
	do{
		clientTCP_len = sizeof(clientTCP);
		if((new_sock = accept(tcp_sock,(struct sockaddr *)&clientTCP,&clientTCP_len)) < 0){
			close(tcp_sock);
			perror("accept error");
			return 5;
		}
		char clientIp[20]="";
		strcpy(clientIp,inet_ntoa(clientTCP.sin_addr));
		printf("Service Requested from %s\n",inet_ntoa(clientTCP.sin_addr));
		
		int length;
		if(fork()==0){ // Create child
			int db_Operation;
			recv(new_sock,&db_Operation,sizeof(int),0);
			db_Operation = ntohl(db_Operation);
			//printf("db_Operation:::: %d\n",db_Operation);
			if(db_Operation == QUERY){  //query/search
				int accntNum;
				read(new_sock,&accntNum,sizeof(accntNum));
				accntNum = ntohl(accntNum);
				struct record returnRecord;
				fileRead(accntNum,&returnRecord);
				char accountDetails[BUFMAX];
				sprintf(accountDetails,"%s %d %0.1f",returnRecord.name,returnRecord.acctnum,returnRecord.value);
				send(new_sock,accountDetails,BUFMAX,0);
			}
			else if(db_Operation == UPDATE){ //update
				int accntNum;
				read(new_sock, &accntNum, sizeof(accntNum));
				accntNum = ntohl(accntNum);
				uint32_t value;
				read(new_sock, &value, sizeof(value));
				value = ntohl(value);
				float val = *((float*)&value);
				struct record returnRecord;
				fileUpdate(accntNum,val,&returnRecord);
				char accountDetails[BUFMAX];
				sprintf(accountDetails,"%s %d %0.1f",returnRecord.name,returnRecord.acctnum,returnRecord.value);
				send(new_sock,accountDetails,BUFMAX,0);
			} 
			close(new_sock);
			return 0;
		}
		else{
			close(new_sock);
		}
	}while(1); //Infinite loop
}
 


