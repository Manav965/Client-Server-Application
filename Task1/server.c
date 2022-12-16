#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include<string.h>
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include<unistd.h>
int main()
{
 
    char str[1024];
    int listen_fd, comm_fd;
 
    struct sockaddr_in servaddr;
    struct sockaddr_in server , client;
    //creating a socket
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
 
    memset( &servaddr,0,sizeof(servaddr));
 
    //Preparing the sockaddr_in structure
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htons(INADDR_ANY);
    servaddr.sin_port = htons(22000);
 
    //Binding the socket
    bind(listen_fd, (struct sockaddr *) &servaddr, sizeof(servaddr));
 
    //Listen
    if(listen(listen_fd,10)==0)
    {
        printf("Listening\n");
    }
    else
    {
        printf("Error\n");
    }
    
    int addr_size=sizeof(struct sockaddr_in);
    
    //Accept call creates a new socket for the incoming connection
    comm_fd = accept(listen_fd, (struct sockaddr*) &client, (socklen_t*)&addr_size);
    
     //If it does not accept's it
        if (comm_fd < 0)
        {
	      perror("accept failed");
	      return 1;
        }
   while(1){
        memset(str,0,sizeof(str));
        int read_size=recv(comm_fd, str, 1024, 0);
        
        if(read_size == -1)
	  {
		  perror("recv failed");
	  }
	  //will help to disconnect from client
	if(strncmp(str, "exit", 4) == 0){
	printf("Disconnected from client.\n");
	break;
	}
	
	//code to reverse the string received from the client.
	int i=0;
	int j=strlen(str)-2;
	while(i<=j)
	{
	char c=str[i];
	str[i]=str[j];
	str[j]=c;
	i++;
	j--;
	}
	
	printf("Reversed String - %s",str);
	write(comm_fd, str, strlen(str)+1);

}
close(listen_fd);
}
