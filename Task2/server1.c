#include<stdio.h>
#include<math.h>
#include<string.h>	//strlen
#include<stdlib.h>	//strlen
#include<sys/socket.h>
#include<arpa/inet.h>	//inet_addr
#include<unistd.h>	//write
#include<pthread.h> //for threading , link with lpthread
#include <time.h>

//Global variables which are accessible by all threads
pthread_t client_threads[1001];
//the threads which are occupied 
int  occ_threads[1001];
struct thread_handler_inp{
    int client_sock_id;
    int client_id;
};

struct thread_handler_inp func_inp[1001];

//for locking the file I/O
pthread_mutex_t mutex;

//function which will be handling the thread
void *thread_handler(void*);

//POSTFIX EVALUATION PART

float stack[1024];
int top=-1;
void push(float value)
{
  if(top==1023)return;
  top++;
  stack[top]=value;
}
float pop()
{
  if(top==-1){
    return -1;
  }
  return stack[top--];
}

float postfixEvaluation(char *query)
{
  //iterating on the characters onw by one
  int i=0;
  int n=strlen(query);
  //printf("%d\n",n);
  for(;i<n-1;i++){
  if(query[i]==' ' || query[i]=='\0')continue;
  else if(query[i]-'0'>=0 && query[i]-'0'<=9){
    float num=(float)0;
    float ten=(float)10;
    //to get the full number in case of numbers bigger than one digit
    char arr[1024];
    int k=0;
    while(query[i]-'0'>=0 && query[i]-'0'<=9 && i<n)
    {
      num=num*ten+(float)(query[i]-'0');
      arr[k++]=query[i];
      i++;
    }
    int flag=0;
    //to check for the floating point numbers in the query
    if(query[i]=='.')
    {
      arr[k++]='.';
      i++;
      flag=1;
      while(i<n && query[i]!=' '){
        arr[k++]=query[i];
        i++;
      }
    }
    i--;
    //push the element in the stack
    if(flag)
    {
      float number=strtof(arr,NULL);
      //printf("%f\n",number);
      push(number);
      memset(arr, 0, 1024);
    }
    else {push(num);}
  }
  else{
    float val1=pop();
    float val2=pop();
    //for degugging
    //printf("%f\n",val1);
    //printf("%f\n",val2);
    if(query[i]=='+')
    {
      push(val2+val1);
    }
    else if(query[i]=='-')
    {
      push(val2-val1);
    }
    else if(query[i]=='/')
    {
      push(val2/val1);
    }
    else if(query[i]=='*')
    {
      push(val2*val1);
    }
}
if(top==1 && i==n-1)break;
}
float value=(float)pop();
//printf("%f\n",value);
return value;
}

//COMPLETES PART

int main(int argc, char *argv[])
{
    
    //initialising the variables and the mutex lock
    pthread_mutex_init(&mutex,NULL);
    int socket_desc , client_sock , c , *new_sock;
	struct sockaddr_in server , client;

    //creating a socket
    socket_desc=socket(AF_INET, SOCK_STREAM, 0);
    if(socket_desc == -1){
        printf("Socket could not be created");
    }

    //Preparing the sockaddr_in structure
    server.sin_family = AF_INET;
	server.sin_addr.s_addr = htons(INADDR_ANY);
  server.sin_port = htons( 22000 );//22000 is the port number

    //Binding the socket
    if( bind(socket_desc,(struct sockaddr *)&server,sizeof(server))<0)
    {
        //printing the error message if binding fails
        perror("Bind Failed.Error");
        return 1;
    }

    //Listen
    if(listen(socket_desc,50)==0)
    {
        printf("Listening\n");
    }
    else
    {
        printf("Error\n");
    }
    int addr_size=sizeof(struct sockaddr_in);
    int i=0;
    while(1)
    {
        //Accept call creates a new socket for the incoming connection
        client_sock = accept(socket_desc, (struct sockaddr *) &client, (socklen_t*)&addr_size);
        
        //If it does not accept's it
        if (client_sock < 0)
        {
		      perror("accept failed");
		      return 1;
        }

        //for each client request creates a thread and assign the client request to it to process
       //so the main thread can entertain next request
        int client_thread_id;
        for(int k=0;k<1000;k++){
          if(!occ_threads[k]){
            occ_threads[k]=1;
            client_thread_id=k;
            break;
          }
        }
        struct thread_handler_inp instance;
        instance.client_id=client_thread_id;
        instance.client_sock_id=client_sock;
        if( pthread_create(&client_threads[i], NULL, thread_handler,&instance) != 0 )
          printf("Failed to create thread\n");  
        
       
        printf("Handler assigned to client %d\n",client_thread_id+1);
        i++;
    }    
    //destroying the mutex
    pthread_mutex_destroy(&mutex);
    return 0;
}

//This will handle connection for each client

void *thread_handler(void *arg){
    struct thread_handler_inp instance=*(struct thread_handler_inp*)arg;
    int sock=instance.client_sock_id;
    int client_idx=instance.client_id+1;
    char client_message[1024],result[1024];
    bzero(result, sizeof(result));
    clock_t start, end;
    

  //Recieve an equation from client
  while(1){
    int read_size=recv(sock, client_message, 1024, 0);
    //for debugging
    if(read_size == -1)
	  {
		  perror("recv failed");
	  }
	  if(strncmp(client_message, "exit", 4) == 0){
		printf("Disconnected from client:%d\n",client_idx);
		break;
  	}
    else{
		printf("Client %d: %s\n",client_idx,client_message);
		//CALCULATION OF RESULT OF EQUATION
		char *query = malloc(sizeof client_message);
		memmove(query, client_message, sizeof client_message);
		//starting the clock before evaulating the query
		start=clock();
		double value=(double)postfixEvaluation(query);
		
		printf ("%f\n", value);
		sprintf(result, "%f",value);
		int send_size=send(sock, result, sizeof(result), 0);
		query = malloc(strlen(client_message)-1);
		memmove(query, client_message,strlen(client_message)-1);

		if(send_size>0)
	        {       
	                pthread_mutex_lock(&mutex);
	                end=clock();
			double timeElapsed=((double)(end-start))/  CLOCKS_PER_SEC;
			FILE * fptr;
			fptr=fopen("server_records.txt", "a");
			fprintf(fptr, "<%d><%s><%s><%f>\n", client_idx, query, result, (float)timeElapsed);
			fflush(fptr);
			fclose(fptr);
			pthread_mutex_unlock(&mutex);
		}
		else
		{
			break;
		}	
		bzero(client_message, sizeof(client_message));
		bzero(result, sizeof(result));
	}
}    
//Free the occupied thread so that next time a client connects the id can be allotted to that client
occ_threads[client_idx-1]=0;
//Free the socket pointer
close(sock);
pthread_exit(NULL);
}

