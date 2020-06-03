#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <netinet/tcp.h> 
#include <unistd.h> 
#include <arpa/inet.h>
#include <time.h>
#include <sys/time.h>
#define MAX 256 
#define PORT 8080 
#define SA struct sockaddr 
void recvFile(int sockfd, int i) 
{ 
	char buff[80];  // to store message from client
 	printf("%d \n",i);
 	int counter = 0;
 	while( read(sockfd,buff,80) > 0 )
  		counter++;
 	int len = sizeof(buff); 
	 
 printf("File %d received, chunks: %d using: %s\n",i,counter, buff);
if(i<5)
{
printf("cubic \n");
}
else 
printf("Renu \n");
} 

int main() 
{ 
	int sockfd, connfd, len; 
	struct sockaddr_in servaddr, cli; 

	// socket create and verification 
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd < 0) { 
		printf("socket creation failed...\n"); 
		return 0; 
	} 
	else
		printf("Socket successfully created..\n"); 

	bzero(&servaddr, sizeof(servaddr)); 

	// assign IP, PORT 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
	servaddr.sin_port = htons(PORT); 

	// Binding newly created socket to given IP and verification 
	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) { 
		printf("socket bind failed...\n"); 
		return 0; 
	} 
	else
		printf("Socket successfully binded..\n"); 

	// Now server is ready to listen and verification 
	if ((listen(sockfd, 5)) != 0) { 
		printf("Listen failed...\n"); 
		return 0; 
	} 
	else
		printf("Server listening..\n"); 
	len = sizeof(cli); 

	// Measure Time

	long times[10];
	// Function for send-recv file between client and server
	for (int i = 0; i <10; i++) 
	{
	if(i>=5){
	char bif[256];
	socklen_t len;
	strcpy(bif,"reno");
	len = strlen(bif);
	if (setsockopt(sockfd, IPPROTO_TCP, TCP_CONGESTION, bif, len) != 0) {
			perror("setsockopt"); 
			return 0;
		}
}
		// Accept the data packet from client and verification 
		connfd = accept(sockfd, (SA*)&cli, &len); 
		clock_t before = clock();
	time_t timeBefore;
	time(&timeBefore);
double msec=0;
		if (connfd < 0) { 
			printf("server acccept failed...\n"); 
			exit(0); 
		} 
		else
			printf("server acccept the client...\n"); 
		
		recvFile(connfd, i); // recv func
		before = clock()-before; 		
		msec = ((double)before / CLOCKS_PER_SEC);
		//printf("Time taken %lf seconds \n",msec);
		time_t timeAfter;
    		time(&timeAfter);
    		printf("time for this file is %ld second \n",timeAfter-timeBefore);
    		times[i] = timeAfter-timeBefore;
		close(connfd); 
	}
	long count=0;
    	for(int i =0;i<5;i++)
    	{
        	count = count+times[i];
    	}
   	printf("average time for Cubic is : %ld",count/5);
    	count=0;
    	for(int i =5;i<10;i++)
    	{
        	count = count+times[i];
    	}
    	printf("average time for reno is : %ld",count/5);
    	int OKAY =1;
    	setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&OKAY,sizeof(int));
	close(sockfd);
	return 0;
} 

