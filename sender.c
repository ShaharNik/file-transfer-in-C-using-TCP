#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <netinet/tcp.h> 
#include <unistd.h> 
#define MAX 80 
#define PORT 8080 
#define SA struct sockaddr 

// Function designed for chat between client and server. 
void func(int sockfd, int i) 
{ 
 char buff[MAX];       // for read operation from file and used to sent operation
	// printf("%d",sizeof(buff)); 
if (i>5) // change to "Reno"
{ 
 strcpy(buff, "reno"); 
// printf("%d",sizeof(buff)); 
 int len = strlen(buff);
 if (setsockopt(sockfd, IPPROTO_TCP, TCP_CONGESTION, buff, len) != 0) {
	perror("setsockopt"); 
	return -1;
 }
}
 // create file 
 FILE *fp;
 fp=fopen("create_large_file.py","r");  // open file uses both stdio and stdin header files
           // file should be present at the program directory
if( fp == NULL ){
  printf("Error IN Opening File .. \n");
  return ;
 }
 
 while ( fgets(buff,MAX,fp) != NULL ) // fgets reads upto MAX character or EOF 
  write(sockfd,buff,sizeof(buff));  // sent the file data to stream
 
 fclose (fp);       // close the file 
 
 printf("File %d Sent successfully !!! \n",i);
} 

// Driver function 
int main() 
{ 
	int sockfd, connfd; 
	struct sockaddr_in servaddr, cli; 

	// socket create and varification 
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
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
	servaddr.sin_port = htons(PORT); 

	// connect the client socket to server socket 
	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) { 
		printf("connection with the server failed...\n"); 
		exit(0); 
	} 
	else
		printf("connected to the server..\n"); 

	// function for chat 
	for (int i=1; i<=10; ++i)
		func(sockfd, i);


	// close the socket 
	close(sockfd); 
} 

