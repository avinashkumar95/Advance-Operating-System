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

#define SERV_NT_FOUND "INVALID Service name!"
#define BUFMAX 2048
#define QUERY 1001
#define UPDATE 1002
#define UDPSOCKET 28911
#define Message "GET CISBANK"

int main(void)
{
	
	char buf[BUFMAX];
	struct sockaddr_in remoteUDP,localUDP,serverTCP;
	int sk,len = sizeof(localUDP),orig_sock;
	struct hostent *host;
	struct in_addr ipv4addr;
	
	/*Create an Internet domain datagram socket */
	sk = socket(AF_INET, SOCK_DGRAM, 0);
	localUDP.sin_family = AF_INET;  /* Define the socket domain */
	localUDP.sin_addr.s_addr =  INADDR_ANY;  
	localUDP.sin_port = 0; /* Let the system assign a port */
	if( bind(sk,(struct sockaddr *)&localUDP,sizeof(localUDP)) < 0){
		printf("Bind Fails! \n");
		exit(1);
	}
	
	remoteUDP.sin_family = AF_INET;
	remoteUDP.sin_addr.s_addr = inet_addr("137.148.205.255");
	remoteUDP.sin_port = ntohs(UDPSOCKET);
	setsockopt(sk,SOL_SOCKET,SO_BROADCAST,(struct sockaddr *)&remoteUDP,sizeof(remoteUDP));
	
	sendto(sk, Message, strlen(Message)+1,0,(struct sockaddr *)&remoteUDP, sizeof(remoteUDP)); /*Send the message*/
	read(sk,buf,BUFMAX); /*Get the reply*/
	
	/* if service is not registered in servicemap, then exit*/
	if(strcmp(buf,SERV_NT_FOUND)==0){
		printf("%s \n",buf);
		exit(1);
	}
	
	char serverPort[10] = "";
	char qo[4];
	char re[4];
	char serverIp[20] = "";
	char *tableData;
	int count = 0;
	
	/*Below loop is to tokenize response MSG from servicemap in to the IP address and port number */
	tableData = strtok(buf,",");
	while(tableData != NULL){
		if(count == 0 || count == 1 || count == 2){
			strcat(serverIp,tableData); //Get the Server IP 
			strcat(serverIp,".");
		}
		else if(count == 3){
			strcat(serverIp,tableData);
		}
		else if(count == 4){
			strcpy(qo,tableData); //Get the Server Port quotient
		}
		else if(count == 5){
			strcpy(re,tableData); //Get the Server Port remainder
		}
		tableData = strtok(NULL,",");
		count++;
	}
	int quotient = atoi(qo);
	int remainder = atoi(re);
	int pNumber = (quotient*256)+remainder;
	sprintf(serverPort, "%u", pNumber); /* Convert socket number to string */
	printf("Service Provided by %s at port %s\n",serverIp,serverPort); //Print Server Details
	close(sk); //Close connection with serviceMap as it is no longer needed.
	
	/* Start of TCP connection with Server  */
	inet_pton(AF_INET,serverIp,&ipv4addr); //Convert string IP to IPv4 address format
	host = gethostbyaddr(&ipv4addr,sizeof(ipv4addr),AF_INET); //Get host by address
	serverTCP.sin_family = AF_INET;
	memcpy(&serverTCP.sin_addr,host->h_addr,host->h_length);
	char *ptr;
	long ret;
	ret = strtoul(serverPort,&ptr,10);
	serverTCP.sin_port = ntohs(ret);
	
	char userMessage[50];
	do{ 						// Loop until user quits
		printf("> ");
		fgets(userMessage,50,stdin); //Get input from user
		userMessage[strcspn(userMessage,"\n")] = '\0';
		if(strcmp(userMessage, "quit")!=0){
			if((orig_sock = socket(AF_INET,SOCK_STREAM,0))<0){
				perror("generate error");
				return 3;
			}
			if(connect(orig_sock,(struct sockaddr *)&serverTCP,sizeof(serverTCP))<0){
				perror("Connect error");
				return 4;
			}
			char *queryToken;
			char queryType[10] = "";
			char accountNumber[10] = "";
			char accountBalance[10] = "";
			int count = 0;
			queryToken = strtok(userMessage," ");
			/* below loop is to tokenize the Command, Account number and Balance */
			while(queryToken != NULL)
			{
				if(count == 0){
					strcpy(queryType,queryToken);
				}
				else if(count == 1){
					strcpy(accountNumber,queryToken);
				}
				else if(count == 2){
					strcpy(accountBalance,queryToken);
				}
				queryToken = strtok(NULL," ");
				count++;
			}
			if(strcmp(queryType,"query") == 0)
			{
				int queryNumber =  QUERY;
				queryNumber = htonl(queryNumber);
				write(orig_sock,&queryNumber,sizeof(queryNumber));  //write db operation to the server
				int accNo = atoi(accountNumber);		//convert the account number to integer
				accNo = htonl(accNo);
				write(orig_sock,&accNo,sizeof(accNo));  //write account number to server
				send(orig_sock,userMessage,len,0);
				char accountDetails[BUFMAX];
				recv(orig_sock,accountDetails,BUFMAX,0);
				printf("%s\n",accountDetails);
			}
			else if(strcmp(queryType,"update") == 0)
			{
				int queryNumber =  UPDATE;
				queryNumber = htonl(queryNumber);
				write(orig_sock,&queryNumber,sizeof(queryNumber));  //write db operation to the server
				int accNo = atoi(accountNumber);
				accNo = htonl(accNo);
				write(orig_sock,&accNo,sizeof(accNo));	//write account number to server
				float accBal = atof(accountBalance);
				uint32_t updateBalance;
				memcpy(&updateBalance, &accBal, sizeof(updateBalance));
				updateBalance = htonl(updateBalance);
				write(orig_sock,&updateBalance,sizeof(updateBalance));  //write to be updated value to the server
				send(orig_sock,userMessage,len,0);
				char accountDetails[BUFMAX];
				recv(orig_sock,accountDetails,BUFMAX,0);
				printf("%s\n",accountDetails);
			}
			close(orig_sock);
		}
	}while(strcmp(userMessage,"quit")!=0);
	return 0;	
}
