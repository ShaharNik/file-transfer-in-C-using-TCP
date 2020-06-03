#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <netinet/tcp.h> 
#include <unistd.h> 
#include <arpa/inet.h>
#define MAX 256 
#define PORT 8080 
#define SA struct sockaddr 

// send file from client to server 
void sendFile(int sockfd, int i) 
{ 
	char buff[80];       // for read operation from file and used to sent operation 
	 	int counter =0;
	memset(buff,'0',sizeof(buff));
 	// create file 
 	FILE *fp;
 	fp=fopen("1gb.txt","r");  // open file uses both stdio and stdin header files
           // file should be present at the program directory
	if( fp == NULL ){
  		printf("Error IN Opening File .. \n");
  		return ;
 	}

 	while ( fgets(buff,80,fp) != NULL ) // fgets reads upto MAX character or EOF
 	{
		counter++;
  		write(sockfd,buff,sizeof(buff));  // sent the file data to stream
		
 	}
	printf("Sent %d chunks using ",counter);
	if (i < 5)
		printf("cubic\n");
	else
        	printf("reno \n");
 	fclose (fp);       // close the file 
} 



int main() 
{ 
	int sockfd, connfd; 
	struct sockaddr_in servaddr, cli; 
	// assign IP, PORT 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
	servaddr.sin_port = htons(PORT); 
	// connect the client socket to server socket 
	

	// socket create and varification 


	for (int i=0; i<10; i++)
	{
		sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if(i>=5)
		{
		char buf[MAX];  
		strcpy(buf, "reno");  
 		int len = strlen(buf);
 		if (setsockopt(sockfd, IPPROTO_TCP, TCP_CONGESTION, buf, len) != 0) {
			perror("setsockopt"); 
			return ;
 		}
		}
// connect the client socket to server socket 
	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) { 
		printf("connection with the server failed...\n"); 
		exit(0); 
	} 
	else
		printf("connected to the server..\n"); 
	

		// socket create and varification 
		if (sockfd == -1) { 
			printf("socket creation failed...\n"); 
			return 0; 
		} 
		else
			printf("Socket successfully created..\n"); 
		//bzero(&servaddr, sizeof(servaddr)); 
		
		sendFile(sockfd, i); // Function for sending file
		int OKAY = 1;
        	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &OKAY, sizeof(int));
		
		// close the socket 
		close(sockfd); 
	}
	return 0;
} 

