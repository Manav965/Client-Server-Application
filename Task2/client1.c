#include <stdio.h>	//printf
#include <string.h>	//strlen
#include <sys/socket.h>	//socket
#include <arpa/inet.h>	//inet_addr
#include <unistd.h>
#include<stdlib.h>

int main(int argc , char *argv[])
{
	int sock;
	struct sockaddr_in server;
	char message[1024] , server_reply[1024];
	
	//Create socket
	sock = socket(AF_INET , SOCK_STREAM , 0);
	if (sock == -1)
	{
		printf("Could not create socket");
	}
	puts("Socket created");
	
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons( 22000 );

	//Connect to remote server
	if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
	{
		perror("connect failed. Error");
		return 1;
	}
	puts("Connected\n");
	
	//keep communicating with server
	while(1)
	{
		printf("Please enter the message to the server: ");
        //for doing standard input
		fgets(message,1024,stdin); /*stdin = 0 , for standard input */
		//scanf("%s",&message[0]);
		//int write_size=write(sock , message , strlen(message));
		int write_size=send(sock, message, strlen(message), 0);
		//Send some data
		if( write_size < 0)
		{
			puts("Send failed");
			return 1;
		}
        //inorder to close the socket on special string :exit
                if(strncmp(message, "exit", 4) == 0){
			close(sock);
			printf("Disconnected from server.\n");
			exit(1);
		}
		
		//Receive a reply from the server
		if( recv(sock , server_reply , 1024 , 0) < 0)
		{
			puts("recv failed");
			break;
		}
		
		printf("Server reply: \t%s\n", server_reply);
		bzero(server_reply, sizeof(server_reply));
	}
	
	close(sock);
	return 0;
}
