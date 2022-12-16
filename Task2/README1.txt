Task -2 

For this there are three files
server1.c
client1.c
server_records.txt

This task is more or less similiar to the task-1 .However , in this task the server can handle more than one client or multiple clients simaultaneously.I have handled the multiple clients case using multithreading in which the server assigns each incoming client a new thread and a client-id so that the each client can be tracked and the we can see that which client entered the query.Once the client types "exit" the client is disconnected from the server and server will show this by displaying the client as well the client-id which is disconnected.So in order to simulate mulitple clients case one needs to open multiple terminals and compile and execute the client1.c file .But before running the client1 code it is important to make the server running.

Terminal commands to run the code
Open one terminal for server and multiple terminals can be opened for clients and type following commands to 
run the code.

For server:
gcc -o server1 server1.c -lpthread
./server1

To stop the server press ctrl+c.

For client:
gcc -o client1 client1.c -lpthread
./client1

Also to disconnect client type "exit"(without quotes)
 
Few other pointers:
1.The result will be floating point number irrespective of the ans being an integer or float (i.e if the ans is 3 the result will be 3.0000).
2.The time elapsed is taken to be time between , calling the function for evaluting the expression and writing the server_records.txt file.
3.I have included some results from sample test cases in the server_records.txt
4.To handle the race conditions I have added mutex lock around the piece of code where the result is written to the server_records.txt. This lock will prevent any other client from writing to the file until one client has finished writing i.e one client will not interfere while other client is writing to the text file. 
