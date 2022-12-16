#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include<string.h>
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include<unistd.h>
#include<stdlib.h>
int main(int argc,char **argv)
{
    int sockfd,n;
    char sendline[1024];
    char recvline[1024];
    struct sockaddr_in servaddr;
 
    //Create socket
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    memset(&servaddr,0,sizeof(servaddr));
 
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(22000);
 
    inet_pton(AF_INET,"127.0.0.1",&(servaddr.sin_addr));
 
    //Connect to remote server
    if(connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr))<0){
        printf("Couldn't connect to server\n");
        exit(-1);
    }
 
    
    memset( sendline,0,sizeof(sendline));
    memset( recvline,0,sizeof(recvline));
   
 //keep communicating with server
 while(1)
	{
		printf("Please enter the string: ");
        //for doing standard input
		fgets(sendline,1024,stdin); /*stdin = 0 , for standard input */
		int write_size=send(sockfd, sendline, strlen(sendline), 0);
		
		//Send some data
		if( write_size < 0)
		{
			puts("Send failed");
			return 1;
		}
        //inorder to close the socket on special string :exit
                if(strncmp(sendline, "exit", 4) == 0){
			close(sockfd);
			printf("Disconnected from server.\n");
			exit(1);
		}
		
		//Receive a reply from the server
		if( recv(sockfd , recvline , 1024 , 0) < 0)
		{
			puts("recv failed");
			break;
		}
		
		 printf("Server reply: \t%s\n", recvline);
		 bzero(recvline, sizeof(recvline));
	}
	
	close(sockfd);
	return 0;
}

