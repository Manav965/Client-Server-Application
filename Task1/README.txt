Task -1 

For this there are two files 
server.c
client.c

As soon as the server listening a message is displayed.Whenver the client sends a string server returns the reversed string.Futhermore,the string can be space seperated as well.
If the client types "exit" (without quotes )client is dissconnected from the server.
 
Also before running client it is necessary to start the server

Terminal commands to run the code
Open one terminal for server and another terminal for client to run the code.

For server:
gcc -o server server.c 
./server

To stop the server press ctrl+c.

For client:
gcc -o client client.c 
./client
   
Few pointers:
1.The client can send as many queries it want's to i.e. the server will not get disconnected until the client types exit.

