#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <netinet/tcp.h> 
#include <unistd.h> 
#include <time.h>
#define MAX 256 
#define PORT 8080 
#define SA struct sockaddr 
void recvFile(int sockfd, int i) 
{ 
 char buff[MAX];  // to store message from client
 if (i>5) // change to "Reno"
{ 
 	strcpy(buff, "reno"); 
 	int len = strlen(buff);
 	if (setsockopt(sockfd, IPPROTO_TCP, TCP_CONGESTION, buff, len) != 0) {
		perror("setsockopt"); 
		return ;
	}

}
int len = sizeof(buff); 
if (getsockopt(sockfd, IPPROTO_TCP, TCP_CONGESTION, buff, &len) != 0) { 
	perror("getsockopt");
	return ;
} 
printf("Current: %s\n", buff);

 int counter = 0;
 while( read(sockfd,buff,MAX) > 0 )
  counter++;
 
 printf("File %d received successfully !! counter is:%d \n",i,counter);
} 

int main() 
{ 
	int sockfd, connfd, len; 
	struct sockaddr_in servaddr, cli; 

	// socket create and verification 
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd == -1) { 
		printf("socket creation failed...\n"); 
		exit(0); 
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
		exit(0); 
	} 
	else
		printf("Socket successfully binded..\n"); 

	// Now server is ready to listen and verification 
	if ((listen(sockfd, 5)) != 0) { 
		printf("Listen failed...\n"); 
		exit(0); 
	} 
	else
		printf("Server listening..\n"); 
	len = sizeof(cli); 

	// Accept the data packet from client and verification 
	connfd = accept(sockfd, (SA*)&cli, &len); 
	if (connfd < 0) { 
		printf("server acccept failed...\n"); 
		exit(0); 
	} 
	else
		printf("server acccept the client...\n"); 

	// Measure Time
     	clock_t start, end;
     	double cpu_time_used;
	double sum = 0;
	// Function for send-recv file between client and server
	for (int i = 1; i <= 10; ++i) 
	{
		start = clock();
		recvFile(connfd, i); // recv func
 		end = clock();
     		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		sum += cpu_time_used;
		printf("transfer file %d took %f seconds to execute \n",i, cpu_time_used); 
		if (i == 5)
		{
			sum /= 5;
			printf("the 1st cubic avg is: %f \n",sum);
			sum = 0;
		}
	}
	sum /= 5; // reno avg 
	printf("the 2st reno avg is: %f \n",sum);
	// After send-recv close the socket 
	close(sockfd); 
} 

